#include "stdafx.h"
#include <SADXModLoader.h>
#include <IniFile.hpp>
#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"
#include "barrel.h"
#include "SkyChaseFixes.h"
#include "MR_FinalEggFix.h"
#include "PerfectChaosFixes.h"
#include "HotShelterWaterfallFix.h"

DataArray(NJS_MATERIAL, matlist_022710E0, 0x026710E0, 5);
DataPointer(NJS_ARGB, stru_3D0B7C8, 0x3D0B7C8);
DataPointer(NJS_OBJECT, stru_8B22F4, 0x8B22F4);
FunctionPointer(double, sub_49EAD0, (float a1, float a2, float a3, int a4), 0x49EAD0);
FunctionPointer(float, sub_49E920, (float x, float y, float z, Rotation3 *rotation), 0x49E920);

static bool DLLLoaded_DCMods   = false;
static bool DLLLoaded_DLCs     = false;
static bool DLLLoaded_SA1Chars = false;

// Replaces: mov    camerathing,    80000004h
// With:     or     camerathing,    80000004h
// Based on information from VeritasDL and code from SADX Steam.
// Note that SADX Steam seems to use 0x8000000C and not 0x80000004, but it doesn't seem to make a difference.
// Uncomment to change 0x80000004 to 0x8000000C
static const Uint8 freecam_fix[] = { 0x81, 0x0D, /*0xA8, 0xCB, 0xB2, 0x03, 0x0C, 0x00, 0x00, 0x80*/ };
static const Uint8 mt_kusa_nop[] = { 0x90, 0x90 };
static Uint32 CasinoSpawnY = 0xC3480001; // Secretly a float of about -200.0

static float KusaDistance = 50000.0f;
int SegaVoiceLanguage = 1;

double __cdecl AmenboFix(float a1, float a2, float a3, int a4)
{
	double u = sub_49EAD0(a1, a2, a3, a4);

	if (u == -1000000)
	{
		u = a2;
	}

	return u;
}

float __cdecl EggKeeperFix(float x, float y, float z, Rotation3* rotation)
{
	float result = sub_49E920(x, y, z, rotation);

	if (result == -1000000)
	{
		result = y;
	}

	return result;
}

void __cdecl FixedBubbleRipple(ObjectMaster* a1)
{
	auto v1 = static_cast<NJS_VECTOR*>(a1->UnknownB_ptr);
	if (!MissedFrames)
	{
		SetTextureToCommon();
		njPushMatrix(nullptr);
		njTranslateV(nullptr, v1);
		BackupConstantAttr();
		AddConstantAttr(0, NJD_FLAG_USE_ALPHA);
		float v2 = v1[1].z;
		stru_3D0B7C8.g = v1[1].z;
		stru_3D0B7C8.b = v2;
		stru_3D0B7C8.a = v2;
		SetMaterialAndSpriteColor(&stru_3D0B7C8);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		njScale(nullptr, v1[2].z, 1.0f, v1[2].z);
		DrawQueueDepthBias = -17952;                                     // Copied from sub_4B9290
		ProcessModelNode_A_WrapperB(&stru_8B22F4, (QueuedModelFlagsB)0); // Replaced DrawModel_Callback
		DrawQueueDepthBias = 0;                                          // Copied from sub_4B9290
		ClampGlobalColorThing_Thing();
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
		RestoreConstantAttr();
		njPopMatrix(1u);
	}
}

// Because ain't nobody got time for compiler warnings
#define _arrayptrandlength(data) data, (int)LengthOfArray(data)
#define _arraylengthandptr(data) (int)LengthOfArray(data), data

static PatchInfo patches[] = {
	// MT_KUSA. Also found at 0x0082F216 in SADX 2010
	{ reinterpret_cast<void*>(0x00608380 + 0x1D), _arrayptrandlength(mt_kusa_nop) },
	// Action stage load and restart
	{ reinterpret_cast<void*>(0x00438330), _arrayptrandlength(freecam_fix) },
	// Adventure field scene change
	{ reinterpret_cast<void*>(0x00434870), _arrayptrandlength(freecam_fix) },
	// Casinopolis spawn point Y offset fix
	{ reinterpret_cast<void*>(0x005C0D5D), &CasinoSpawnY, sizeof(float) }
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
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

		SegaVoiceLanguage = config->getInt("General settings", "SegaVoiceLanguage", 1);

		// SEGA/Sonic Team voice
		if (!DLLLoaded_DLCs && SegaVoiceLanguage > 0)
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
		auto ec2mesh = reinterpret_cast<LandTable*>(0x01039E9C);
		obj = ec2mesh->Col[1].Model;

		obj->ang[0] = 32768;
		obj->pos[1] = -3.0f;
		obj->pos[2] = -5850.0f;

		// Fixes Rhino Tank treads
		reinterpret_cast<NJS_MATERIAL*>(0x038C9DF8)[0].attrflags &= ~NJD_FLAG_CLAMP_MASK;
		reinterpret_cast<NJS_MATERIAL*>(0x038CA220)[0].attrflags &= ~NJD_FLAG_CLAMP_MASK;

		// Fixes a rendering issue with the red moving platform in Speed Highway.
		matlist_022710E0[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Leon fix
		WriteData(reinterpret_cast<float**>(0x004CD75A), &_nj_screen_.w);
		WriteData(reinterpret_cast<float**>(0x004CD77C), &_nj_screen_.h);

		// Makes the animated MtKusa model show up at larger distances
		WriteData(reinterpret_cast<float**>(0x00608331), &KusaDistance);

		// Fixes missing Sweep badniks in Emerald Coast 2 and Twinkle Park 2
		if (!DLLLoaded_DCMods)
		{
			WriteCall(reinterpret_cast<void*>(0x007AA9F9), AmenboFix);
		}

		// Fixes a missing Egg Keeper in Final Egg 1
		if (!DLLLoaded_DCMods)
		{
			WriteCall(reinterpret_cast<void*>(0x0049EFE7), EggKeeperFix);
		}

		// Sky Chase fixes
		if (!DLLLoaded_DCMods)
		{
			SkyChaseFix_Init();
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

		// Twinkle Park barrel model (requires a different "boo" texture in OBJ_TWINKLE!)
		*reinterpret_cast<NJS_OBJECT*>(0x0279D364) = object_000A0E58;

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
		WriteJump(reinterpret_cast<void*>(0x7A81A0), FixedBubbleRipple);

		// Fix Perfect Chaos damage animation
		if (!DLLLoaded_DCMods)
		{
			WriteJump(reinterpret_cast<void*>(0x005632F0), Chaos7Explosion_DisplayX);
			WriteJump(reinterpret_cast<void*>(0x005633C0), Chaos7Damage_DisplayX);
		}
	}

	EXPORT void __cdecl OnRenderDeviceReset()
	{
		SkyChaseFix_UpdateBounds();
	}

	EXPORT void __cdecl OnFrame()
	{
		// Fix broken welds after playing as Metal Sonic
		if (GameMode == GameModes_CharSel && static_cast<bool>(MetalSonicFlag))
		{
			MetalSonicFlag = false;
		}

		if (!DLLLoaded_DLCs)
		{
			if (CurrentLevel == 12 && CurrentAct == 0 && GameState != 16)
			{
				if (HotShelterWaterThing < 65.0f && HotShelterWaterThing > 0.0f)
				{
					WaterThing_VShift = (WaterThing_VShift - 16 * FramerateSetting) % 255;

					for (int i = 0; i < 56; i++)
					{
						uv_014107E0[i].v = uv_014107E0_0[i].v + WaterThing_VShift;
					}

					for (int i = 0; i < 20; i++)
					{
						uv_01410790[i].v = uv_01410790_0[i].v + WaterThing_VShift * 2;
					}
				}
			}
		}
	}
}
