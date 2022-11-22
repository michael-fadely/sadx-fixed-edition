#include "stdafx.h"
#include <SADXModLoader.h>
#include <IniFile.hpp>
#include <Trampoline.h>
#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"
#include "Barrel.h"
#include "MR_FinalEggFix.h"
#include "StretchyFeet.h"
#include "PerfectChaosFixes.h"
#include "HotShelterWaterfallFix.h"
#include "DataPointers.h"

static bool DLLLoaded_DCMods   = false;
static bool DLLLoaded_DLCs     = false;
static bool DLLLoaded_SA1Chars = false;

static const Uint8 MT_KUSA_NOP[] = { 0x90, 0x90 };
static const float CASINO_SPAWN_Y = -200.0f;
static const float KUSA_DISTANCE = 25000.0f;

// Trampoline to make badniks stay when the shadow function fails to find the ground - only used by the Sweep badnik
static Trampoline* GetShadowPosOnWater_t = nullptr;
static float __cdecl GetShadowPosOnWater_r(float x, float y, float z, Rotation3* rotation)
{
	const auto original = TARGET_DYNAMIC(GetShadowPosOnWater);
	float result = original(x, y, z, rotation);
	if (result == -1000000.0f)
	{
		result = y;
	}
	return result;
}

// Calls GetShadowPos and corrects the return value to make badniks stay when the shadow function fails to find the ground
float GetShadowPos_r(float x, float y, float z, Angle3* rotation)
{
	float result = GetShadowPos(x, y, z, rotation);
	if (result == -1000000.0f)
	{
		result = y;
	}
	return result;
}

// Hook to draw the bubble version of the ripple with depth
void RippleHack_Bubble(NJS_OBJECT* a1)
{
	late_z_ofs___ = -17952.0f;
	late_DrawObjectClip(a1, LATE_MAT, 2.0f);
	late_z_ofs___ = 0.0f;
}

// Trampoline to fix broken welds after playing as Metal Sonic
static Trampoline* SetAdvaCharMdlTask_t = nullptr;
static void __cdecl SetAdvaCharMdlTask_r()
{
	const auto original = TARGET_DYNAMIC(SetAdvaCharMdlTask);

	gu8flgPlayingMetalSonic = 0;
	original();
}

static PatchInfo patches[] = {
	// MT_KUSA. Also found at 0x0082F216 in SADX 2010
	{ reinterpret_cast<void*>(0x00608380 + 0x1D), _arrayptrandlength(MT_KUSA_NOP) },
};

extern "C"
{
	EXPORT PatchList Patches[]   = { { _arrayptrandlength(patches) } };
	EXPORT ModInfo   SADXModInfo = { ModLoaderVer, nullptr, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0 };

	EXPORT void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// Check which DLLs are loaded.
		DLLLoaded_SA1Chars = GetModuleHandle(TEXT("SA1_Chars.dll")) != nullptr;
		DLLLoaded_DCMods   = GetModuleHandle(TEXT("DCMods_Main.dll")) != nullptr;
		DLLLoaded_DLCs     = GetModuleHandle(TEXT("DLCs_Main.dll")) != nullptr;

		// Config stuff.
		IniFile config(std::string(path) + "\\config.ini");
		SegaVoiceLanguage = static_cast<SegaVoiceLanguage_t>(config.getInt("General settings", "SegaVoiceLanguage", 1));

		// Implements the SEGA and Sonic Team voices played at the logo screens.
		if (!DLLLoaded_DLCs && SegaVoiceLanguage != SegaVoiceLanguage_t::Off)
		{
			PlaySegaSonicTeamVoice_init();
			WriteJump(reinterpret_cast<void*>(0x0042CCC7), PlaySegaSonicTeamVoice_asm);
			WriteJump(reinterpret_cast<void*>(0x0042CD2F), PlaySegaSonicTeamVoice_asm);
		}

		// Item box fixes.
		if (!DLLLoaded_DCMods)
		{
			WriteJump(Draw, ItemBox_Display_Rotate);
			WriteJump(DrawInWater, ItemBox_Display_Water_Rotate);
			WriteJump(Draw_Break, ItemBox_Display_Destroyed_Rotate);
			// Enables rotation in item box collision.
			// Changes collision shape from CYLINDER1 to CYLINDER2 that supports rotation.
			itembox_colli_info0[0].form = CI_FORM_CYLINDER2;
			itembox_colli_info1[0].form = CI_FORM_CYLINDER2;
			itembox_colli_info1[1].form = CI_FORM_CYLINDER2;
			// Sets the "use rotation" flag.
			itembox_colli_info0[0].attr |= 0x200;
			itembox_colli_info0[1].attr |= 0x200;
			itembox_colli_info1[0].attr |= 0x200;
			itembox_colli_info1[1].attr |= 0x200;
		}

		// Fixes Sonic's broken stretchy feet models.
		FixStretchyFeet();

		// Fixes welds after playing as Metal Sonic.
		SetAdvaCharMdlTask_t = new Trampoline(0x00512BC0, 0x00512BC6, SetAdvaCharMdlTask_r);

		// Fixes the rotation of the second outcrop on your way out of Emerald Coast 1.
		NJS_OBJECT* obj = objLandTable0100.pLandEntry[110].pObject;

		obj->ang[0] = 0;
		obj->ang[1] = 0;
		obj->ang[2] = 0;

		// Fixes the inverted water in Emerald Coast 2.
		obj = objLandTable0101.pLandEntry[1].pObject;

		obj->ang[0] = 32768;
		obj->pos[1] = -3.0f;
		obj->pos[2] = -5850.0f;
		
		// Fixes Rhino Tank treads.
		matlist_rhino_catarpl[0].attrflags &= ~NJD_FLAG_CLAMP_MASK;
		matlist_rhino_catarpl[1].attrflags &= ~NJD_FLAG_CLAMP_MASK;

		// Fixes a rendering issue with the red moving platform in Speed Highway.
		matlist_crane_cage_cage[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Casinopolis spawn point Y offset fix.
		WriteData(reinterpret_cast<float*>(0x005C0D5D), CASINO_SPAWN_Y);

		// Fixes Hot Shelter waterfall animation by swapping meshsets.
		meshset_suisou_taki_taki_taki[0].type_matId = NJD_MESHSET_MASK | 0x1;
		meshset_suisou_taki_taki_taki[0].nbMesh = 4;
		meshset_suisou_taki_taki_taki[0].meshes = pgS_1_suisou_taki_taki_taki;
		meshset_suisou_taki_taki_taki[0].vertuv = vuvS_1_suisou_taki_taki_taki;
		meshset_suisou_taki_taki_taki[1].type_matId = NJD_MESHSET_MASK;
		meshset_suisou_taki_taki_taki[1].nbMesh = 2;
		meshset_suisou_taki_taki_taki[1].meshes = pgS_0_suisou_taki_taki_taki;
		meshset_suisou_taki_taki_taki[1].vertuv = vuvS_0_suisou_taki_taki_taki;

		// Fixes Leon not appearing at higher resolutions.
		WriteData(reinterpret_cast<float**>(0x004CD75A), &_nj_screen_.w);
		WriteData(reinterpret_cast<float**>(0x004CD77C), &_nj_screen_.h);
		// Fixes Leon attack timer.
		WriteData(reinterpret_cast<char*>(0x004A6B88 + 4), 15i8);

		// Makes the animated MtKusa model show up at larger distances.
		WriteData(reinterpret_cast<const float**>(0x00608331), &KUSA_DISTANCE);

		// Fixes Sweeps and other badniks not spawning.
		if (!DLLLoaded_DCMods)
		{
			WriteCall(reinterpret_cast<void*>(0x0049EFE7), GetShadowPos_r); // Egg Keeper
			WriteCall(reinterpret_cast<void*>(0x007A05EF), GetShadowPos_r); // Rhinotank
			WriteCall(reinterpret_cast<void*>(0x004C9012), GetShadowPos_r); // Snowman
			GetShadowPosOnWater_t = new Trampoline(0x0049EAD0, 0x0049EAD7, GetShadowPosOnWater_r); // Sweep
		}

		// Eggman texture list fixes.
		if (!DLLLoaded_SA1Chars)
		{
			// Replace the old Eggmobile NPC model with a high-poly one to resolve a texture issue.
			object_gm_gm0000_eggmoble_eggmoble = _object_gm_gm0000_eggmoble_eggmoble_;
			// Replace the texlist used by the old model in the Egg Carrier NPC array.
			WriteData(reinterpret_cast<NJS_TEXLIST**>(0x007D2B22), &texlist_ev_eggmoble0);
		}
		// Replace the texlist used by the old Eggman model in the Super Sonic Knuckles' cutscene PVM list.
		WriteData(reinterpret_cast<NJS_TEXLIST**>(0x02BD5FE4), &texlist_ev_eggmoble0);

		// Unflips the Twinkle Park barrel texture and makes the vertices not pop out of the model.
		for (int i = 0; i < 102; i++)
		{
			// Reverses Us to flip the texture.
			vuvS_2_boom_boomtaru_boomtaru[i].u = 255 - vuvS_2_boom_boomtaru_boomtaru[i].u;
			// Edits vertices from 27 to 89 to match the Gamecube model.
			if (i < 63)
				point_boom_boomtaru_boomtaru[i + 27] = BarrelReplacementVertices[i];
		}

		// E101 Beta lighting fix.
		object_e101old_kihon0_e101old_e_reye.basicdxmodel->mats[1].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		object_e101old_kihon0_e101old_e_reye.basicdxmodel->mats[2].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		object_e101old_kihon0_e101old_e_leye.basicdxmodel->mats[1].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		object_e101old_kihon0_e101old_e_head.basicdxmodel->mats[7].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		object_e101old_kihon0_e101old_e_lf6.basicdxmodel->mats[0].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		object_e101old_kihon0_e101old_e_rf6.basicdxmodel->mats[0].attrflags |= NJD_FLAG_IGNORE_LIGHT;

		// Zero holding Amy lighting fix.
		object_er_9000_eggmanrobo_er_head.basicdxmodel->mats[11].attrflags &= ~NJD_FLAG_IGNORE_LIGHT;

		// Eggman model lighting fix.
		object_g_g0000_eggman_r_a.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		object_g_g0000_eggman_r_b.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		object_g_g0000_eggman_l_b.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		object_g_g0000_eggman_l_a.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Eggman model (Eggmobile) lighting fix.
		object_gm_gm0000_eggmoble_r_a.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		object_gm_gm0000_eggmoble_r_b.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		object_gm_gm0000_eggmoble_l_b.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		object_gm_gm0000_eggmoble_l_a.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Tikal lighting fixes.
		object_tikal_a_l_a4.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		object_tikal_a_l_a7.basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Fixes the look of the Final Egg base in the Mystic Ruins.
		if (!DLLLoaded_DCMods)
		{
			FixMRBase_Apply(path, helperFunctions);
		}

		// Fixes Egg Carrier Garden ocean animation by adding the "disable meshset buffer" flag to the COL item.
		objLandTableGarden01.pLandEntry[74].slAttribute |= 0x4000000;

		// Fixes the water ripple created by air bubbles.
		WriteCall(reinterpret_cast<void*>(0x007A822B), RippleHack_Bubble);

		// Fixes Perfect Chaos damage animation.
		if (!DLLLoaded_DCMods)
		{
			PerfectChaosDamageFix_Init();
		}
	}

}