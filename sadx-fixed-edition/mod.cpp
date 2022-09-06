#include "stdafx.h"
#include <SADXModLoader.h>
#include <IniFile.hpp>
#include <Trampoline.h>
#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"
#include "barrel.h"
#include "MR_FinalEggFix.h"
#include "PerfectChaosFixes.h"

DataArray(NJS_TEX, vuvS_2_boom_boomtaru_boomtaru, 0x279C710, 102);
DataArray(NJS_VECTOR, point_boom_boomtaru_boomtaru, 0x279C9A8, 102);

static bool DLLLoaded_DCMods   = false;
static bool DLLLoaded_DLCs     = false;
static bool DLLLoaded_SA1Chars = false;

// Replaces: mov    camerathing,    80000004h
// With:     or     camerathing,    80000004h
// Based on information from VeritasDL and code from SADX Steam.
// Note that SADX Steam seems to use 0x8000000C and not 0x80000004, but it doesn't seem to make a difference.
// Uncomment to change 0x80000004 to 0x8000000C
static const Uint8 FREECAM_FIX[] = { 0x81, 0x0D, /*0xA8, 0xCB, 0xB2, 0x03, 0x0C, 0x00, 0x00, 0x80*/ };
static const Uint8 MT_KUSA_NOP[] = { 0x90, 0x90 };
static const Uint32 CASINO_SPAWN_Y = 0xC3480001; // Secretly a float of about -200.0

static const float KUSA_DISTANCE = 50000.0f;

// Trampoline to make badniks stay when it fails to find the ground - only used by the Sweep badnik
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

// Calls GetShadowPos and corrects the return value to make badniks stay when it fails to find the ground
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

static void __cdecl CharSel_LoadA_r();
static Trampoline CharSel_LoadA_t(0x00512BC0, 0x00512BC6, CharSel_LoadA_r);
static void __cdecl CharSel_LoadA_r()
{
	auto original = reinterpret_cast<decltype(CharSel_LoadA_r)*>(CharSel_LoadA_t.Target());

	// Fix broken welds after playing as Metal Sonic
	MetalSonicFlag = 0;
	original();
}

static PatchInfo patches[] = {
	// MT_KUSA. Also found at 0x0082F216 in SADX 2010
	{ reinterpret_cast<void*>(0x00608380 + 0x1D), _arrayptrandlength(MT_KUSA_NOP) },
	// Action stage load and restart
	{ reinterpret_cast<void*>(0x00438330), _arrayptrandlength(FREECAM_FIX) },
	// Adventure field scene change
	{ reinterpret_cast<void*>(0x00434870), _arrayptrandlength(FREECAM_FIX) },
	// Casinopolis spawn point Y offset fix
	{ reinterpret_cast<void*>(0x005C0D5D), &CASINO_SPAWN_Y, sizeof(float) }
};

extern "C"
{
	EXPORT PatchList Patches[]   = { { _arrayptrandlength(patches) } };
	EXPORT ModInfo   SADXModInfo = { ModLoaderVer, nullptr, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0 };

	EXPORT void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// Check which DLLs are loaded
		DLLLoaded_SA1Chars = GetModuleHandle(TEXT("SA1_Chars.dll")) != nullptr;
		DLLLoaded_DCMods   = GetModuleHandle(TEXT("DCMods_Main.dll")) != nullptr;
		DLLLoaded_DLCs     = GetModuleHandle(TEXT("DLCs_Main.dll")) != nullptr;

		// Config stuff
		IniFile config(std::string(path) + "\\config.ini");
		SegaVoiceLanguage = static_cast<SegaVoiceLanguage_t>(config.getInt("General settings", "SegaVoiceLanguage", 1));

		// SEGA/Sonic Team voice
		if (!DLLLoaded_DLCs && SegaVoiceLanguage != SegaVoiceLanguage_t::Off)
		{
			WriteJump(reinterpret_cast<void*>(0x0042CCC7), PlaySegaSonicTeamVoice_asm);
			WriteJump(reinterpret_cast<void*>(0x0042CD2F), PlaySegaSonicTeamVoice_asm);
		}

		// Item box fixes
		if (!DLLLoaded_DCMods)
		{
			WriteJump(ItemBox_Display_Destroyed, ItemBox_Display_Destroyed_Rotate);
			WriteJump(ItemBox_Display_Unknown, ItemBox_Display_Unknown_Rotate);
			WriteJump(ItemBox_Display, ItemBox_Display_Rotate);
		}

		if (!DLLLoaded_SA1Chars)
		{
			// Disable stretchy feet, as in vanilla SADX it just uses a broken model.
			// This is not the most ideal fix as it could hinder mods that want to
			// take advantage of the shoe morph system.
			WriteData<uint8_t>(reinterpret_cast<uint8_t*>(0x00493500), 0xC3);
		}

		// Fixes the rotation of the second outcrop on your way out of Emerald Coast 1.
		// This will be done via landtable once animated textures are implemented.
		auto ecmesh110 = reinterpret_cast<LandTable*>(0xE99CB8);
		NJS_OBJECT* obj = ecmesh110->Col[110].Model;

		obj->ang[0] = 0;
		obj->ang[1] = 0;
		obj->ang[2] = 0;

		// Fixes the inverted water in Emerald Coast 2.
		auto ec2_mesh = reinterpret_cast<LandTable*>(0x01039E9C);
		obj = ec2_mesh->Col[1].Model;

		obj->ang[0] = 32768;
		obj->pos[1] = -3.0f;
		obj->pos[2] = -5850.0f;

		// Fixes Rhino Tank treads
		reinterpret_cast<NJS_MATERIAL*>(0x038C9DF8)[0].attrflags &= ~NJD_FLAG_CLAMP_MASK;
		reinterpret_cast<NJS_MATERIAL*>(0x038CA220)[0].attrflags &= ~NJD_FLAG_CLAMP_MASK;

		// Fixes a rendering issue with the red moving platform in Speed Highway.
		((NJS_MATERIAL*)0x026710E0)->attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Leon fix
		WriteData(reinterpret_cast<float**>(0x004CD75A), &_nj_screen_.w);
		WriteData(reinterpret_cast<float**>(0x004CD77C), &_nj_screen_.h);
		// Fixes Leon attacking
		WriteData(reinterpret_cast<char*>(0x004A6B88 + 4), 15i8);

		// Makes the animated MtKusa model show up at larger distances
		WriteData(reinterpret_cast<const float**>(0x00608331), &KUSA_DISTANCE);

		// Fixes Sweeps and other badniks not spawning
		if (!DLLLoaded_DCMods)
		{
			WriteCall(reinterpret_cast<void*>(0x0049EFE7), GetShadowPos_r); // Egg Keeper
			WriteCall(reinterpret_cast<void*>(0x007A05EF), GetShadowPos_r); // Rhinotank
			WriteCall(reinterpret_cast<void*>(0x004C9012), GetShadowPos_r); // Snowman
			GetShadowPosOnWater_t = new Trampoline(0x0049EAD0, 0x0049EAD7, GetShadowPosOnWater_r); // Sweep
		}

		if (!DLLLoaded_SA1Chars)
		{
			// Replace the non-updated Eggmobile NPC model with a high-poly one to resolve a texture issue
			*reinterpret_cast<NJS_OBJECT *>(0x010FEF74) = *reinterpret_cast<NJS_OBJECT *>(0x02EEB524);
			// Replace the texlist for the above model in the NPC data array
			WriteData(reinterpret_cast<NJS_TEXLIST**>(0x007D2B22), reinterpret_cast<NJS_TEXLIST*>(0x02EE0AA4));
		}

		// Texlist fix for Eggman in Super Sonic story
		*reinterpret_cast<NJS_TEXLIST**>(0x02BD5FE4) = reinterpret_cast<NJS_TEXLIST*>(0x02EE0AA4);

		// E101 Beta lighting fix
		reinterpret_cast<NJS_OBJECT*>(0x014D857C)->basicdxmodel->mats[1].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		reinterpret_cast<NJS_OBJECT*>(0x014D857C)->basicdxmodel->mats[2].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		reinterpret_cast<NJS_OBJECT*>(0x014D887C)->basicdxmodel->mats[1].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		reinterpret_cast<NJS_OBJECT*>(0x014D943C)->basicdxmodel->mats[7].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		reinterpret_cast<NJS_OBJECT*>(0x014DC25C)->basicdxmodel->mats[0].attrflags |= NJD_FLAG_IGNORE_LIGHT;
		reinterpret_cast<NJS_OBJECT*>(0x014DD4A4)->basicdxmodel->mats[0].attrflags |= NJD_FLAG_IGNORE_LIGHT;

		// Zero holding Amy lighting fix
		reinterpret_cast<NJS_OBJECT*>(0x31A4DFC)->basicdxmodel->mats[11].attrflags &= ~NJD_FLAG_IGNORE_LIGHT;

		// Twinkle Park barrel fix
		for (int i = 0; i < 102; i++)
			vuvS_2_boom_boomtaru_boomtaru[i].u = 255 - vuvS_2_boom_boomtaru_boomtaru[i].u;
		for (int v = 27; v < 90; v++)
			point_boom_boomtaru_boomtaru[v] = BarrelReplacementVertices[v - 27];

		// Eggman model lighting fix
		reinterpret_cast<NJS_OBJECT*>(0x008961E0)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		reinterpret_cast<NJS_OBJECT*>(0x008964CC)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		reinterpret_cast<NJS_OBJECT*>(0x008980DC)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		reinterpret_cast<NJS_OBJECT*>(0x00897DE0)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Eggman model (Eggmobile) lighting fix
		reinterpret_cast<NJS_OBJECT*>(0x02EE22C0)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		reinterpret_cast<NJS_OBJECT*>(0x02EE25AC)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		reinterpret_cast<NJS_OBJECT*>(0x02EE4194)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		reinterpret_cast<NJS_OBJECT*>(0x02EE3E98)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Tikal lighting fixes
		reinterpret_cast<NJS_OBJECT*>(0x008CE058)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;
		reinterpret_cast<NJS_OBJECT*>(0x008CC658)->basicdxmodel->mats[0].attrflags &= ~NJD_FLAG_USE_ALPHA;


		PlaySegaSonicTeamVoice_init();

		// Fix Mystic Ruins base
		if (!DLLLoaded_DCMods)
		{
			FixMRBase_Apply(path, helperFunctions);
		}

		// Fix Egg Carrier Garden ocean animation
		reinterpret_cast<LandTable*>(0x3405E54)->Col[74].Flags = 0x84000002;
		reinterpret_cast<NJS_MATERIAL*>(0x033FE3F8)->diffuse.color = 0x7FB2B2B2;

		// Fix the water ripple created by air bubbles
		WriteCall(reinterpret_cast<void*>(0x007A822B), RippleHack_Bubble);

		// Fix Perfect Chaos damage animation
		if (!DLLLoaded_DCMods)
		{
			PerfectChaosDamageFix_Init();
		}
	}

}
