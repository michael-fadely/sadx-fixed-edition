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
DataArray(SkyboxScale, SkyboxScale_SkyChase1, 0x027D6CE0, 3);
DataArray(DrawDistance, DrawDist_SkyChase1, 0x027D6D58, 3);
DataPointer(float, CurrentDrawDistance, 0x03ABDC74);
DataPointer(D3DCOLORVALUE, stru_3D0B7C8, 0x3D0B7C8);
DataPointer(NJS_OBJECT, stru_8B22F4, 0x8B22F4);
FunctionPointer(double, sub_49EAD0, (float a1, float a2, float a3, int a4), 0x49EAD0);

// Replaces: mov    camerathing,    80000004h
// With:     or     camerathing,    80000004h
// Based on information from VeritasDL and code from SADX Steam.
// Note that SADX Steam seems to use 0x8000000C and not 0x80000004, but it doesn't seem to make a difference.
// Uncomment to change 0x80000004 to 0x8000000C
static const Uint8  freecam_fix[] = { 0x81, 0x0D, /*0xA8, 0xCB, 0xB2, 0x03, 0x0C, 0x00, 0x00, 0x80*/ };
static const Uint8  mt_kusa_nop[] = { 0x90, 0x90 };
static Uint32 CasinoSpawnY  = 0xC3480001; // Secretly a float of about -200.0

static float KusaDistance = 50000.0f;
static bool EnableSegaVoice = true;
std::string SegaVoiceLanguage = "English";

static float float_one = 1.0f;
static float float_tornadospeed = 1.0f;
static float float_targetsize = 1;
static float float_reticlespeedmultiplier = 2.0f;
float HorizontalResolution_float = 640.0f;
float VerticalResolution_float = 480.0f;
float VerticalResolutionHalf_float = 240.0f;
static double SkyChaseSkyRotationMultiplier = -0.5f;
static float SkyChaseLimit_Right = 560.0f;
static float SkyChaseLimit_Left = 80.0f;
static float SkyChaseLimit_Top = 400.0f;
static float SkyChaseLimit_Bottom = 80.0f;
static float widescreenthing = 103.0f;

double __cdecl AmenboFix(float a1, float a2, float a3, int a4)
{
	double u;
	u = sub_49EAD0(a1, a2, a3, a4);
	if (u == -1000000) u = a2;
	return u;
}

void __cdecl FixedBubbleRipple(ObjectMaster *a1)
{
	NJS_VECTOR *v1; // esi@1
	double v2; // st7@2
	v1 = (NJS_VECTOR *)a1->UnknownB_ptr;
	if (!MissedFrames)
	{
		SetTextureToCommon();
		njPushMatrix(0);
		njTranslateV(0, v1);
		BackupConstantAttr();
		AddConstantAttr(0, NJD_FLAG_USE_ALPHA);
		v2 = v1[1].z;
		stru_3D0B7C8.g = v1[1].z;
		stru_3D0B7C8.b = v2;
		stru_3D0B7C8.a = v2;
		SetMaterialAndSpriteColor((NJS_ARGB *)&stru_3D0B7C8);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		njScale(0, v1[2].z, 1.0f, v1[2].z);
		DrawQueueDepthBias = -17952; //Copied from sub_4B9290
		ProcessModelNode_A_WrapperB(&stru_8B22F4, (QueuedModelFlagsB)0); //Replaced DrawModel_Callback
		DrawQueueDepthBias = 0; //Copied from sub_4B9290
		ClampGlobalColorThing_Thing();
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
		RestoreConstantAttr();
		njPopMatrix(1u);
	}
}

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
	EXPORT PatchList   Patches[]   = { { _arrayptrandlength(patches) } };
	EXPORT ModInfo     SADXModInfo = { ModLoaderVer };
	EXPORT void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		//Config stuff
		const IniFile *config = new IniFile(std::string(path) + "\\config.ini");
		EnableSegaVoice = config->getBool("General settings", "EnableSegaVoice", true);
		SegaVoiceLanguage = config->getString("General settings", "SegaVoiceLanguage", "English");
		// SEGA/Sonic Team voice
		if (GetModuleHandle(TEXT("DLCs_Main.dll")) == nullptr && EnableSegaVoice == true)
		{

			WriteJump(reinterpret_cast<void*>(0x0042CCC7), PlaySegaSonicTeamVoice_asm);
			WriteJump(reinterpret_cast<void*>(0x0042CD2F), PlaySegaSonicTeamVoice_asm);
		}

		// Item box fixes
		if (GetModuleHandle(TEXT("DCMods_Main.dll")) == nullptr)
		{
			WriteJump(ItemBox_Display_Destroyed, ItemBox_Display_Destroyed_Rotate);
			WriteJump(ItemBox_Display_Unknown, ItemBox_Display_Unknown_Rotate);
			WriteJump(ItemBox_Display, ItemBox_Display_Rotate);
		}

		if (GetModuleHandle(TEXT("SA1_Chars.dll")) == nullptr)
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
		if (GetModuleHandle(TEXT("DCMods_Main.dll")) == nullptr) WriteCall(reinterpret_cast<void*>(0x007AA9F9), AmenboFix);

		// Sky Chase fixes
		if (GetModuleHandle(TEXT("DCMods_Main.dll")) == nullptr)
		{
			// Resolution related fixes
			HorizontalResolution_float = HorizontalResolution;
			VerticalResolution_float = VerticalResolution;
			VerticalResolutionHalf_float = VerticalResolution_float / 2.0f;
			WriteJump((void*)0x628D50, TornadoCalculateCenterPoint); //Calculate center for bullets
			if (HorizontalResolution_float / VerticalResolution_float > 1.4f)
			{
				if (HorizontalResolution_float / VerticalResolution_float > 2.2f) widescreenthing = 240.0f;
				SkyChaseLimit_Left = 80.0f + widescreenthing;
				SkyChaseLimit_Right = 560.0f + widescreenthing;
			}
			WriteData((float**)0x00627F4D, &float_tornadospeed); //Tornado Speed (always 1)
			WriteData((float**)0x00627F60, &float_one); //Horizontal limit
			WriteData((float**)0x00627F72, &float_one); //Vertical limit
			// Hodai fixes
			WriteData((float**)0x0043854D, &HorizontalResolution_float);
			WriteData((float**)0x00438571, &VerticalResolutionHalf_float);
			WriteData((float**)0x0043857F, &VerticalResolutionHalf_float);
			WriteCall((void*)0x0062C764, SetSkyChaseRocketColor);
			WriteCall((void*)0x0062C704, RenderSkyChaseRocket);
			// Sky Chase reticle and multiplier fixes
			float_reticlespeedmultiplier = VerticalResolution / 480.0f;
			float_targetsize = pow(VerticalResolution / 15.0f, 2);
			WriteData((float**)0x628AF7, &float_targetsize); //Target size
			WriteData((float**)0x00629472, &float_reticlespeedmultiplier); //Target speed
			// Limits for reticle
			WriteData((float**)0x00628994, &float_reticlespeedmultiplier); //right
			WriteData((float**)0x006289B6, &float_reticlespeedmultiplier); //left
			WriteData((float**)0x006289F1, &float_reticlespeedmultiplier); //top
			WriteData((float**)0x00628A13, &float_reticlespeedmultiplier); //bottom
			WriteData((float**)0x0062899A, &SkyChaseLimit_Right);
			WriteData((float**)0x006289BC, &SkyChaseLimit_Left);
			WriteData((float**)0x006289F7, &SkyChaseLimit_Top);
			WriteData((float**)0x00628A19, &SkyChaseLimit_Bottom);
			// Visual stuff
			WriteCall((void*)0x00629004, TornadoTarget_Render);
			WriteCall((void*)0x00628FE5, TornadoTarget_Render);
			WriteJump((void*)0x00628DB0, TornadoTargetSprite_TargetLock_DisplayX);
			WriteData((double**)0x00627D14, &SkyChaseSkyRotationMultiplier); //Rotate the sky in the opposite direction
			WriteData((float*)0x00628951, VerticalResolution / 480.0f); //Reticle scale X
			WriteData((float*)0x0062895B, VerticalResolution / 480.0f); //Reticle scale Y
			((NJS_OBJECT*)0x028DFD34)->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF; //Sky materials in Act 1
			((NJS_OBJECT*)0x028175F4)->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF; //Sky materials in Act 1
			SkyboxScale_SkyChase1->Far.x = 4.0f;
			SkyboxScale_SkyChase1->Far.y = 4.0f;
			SkyboxScale_SkyChase1->Far.z = 4.0f;
			SkyboxScale_SkyChase1->Near.x = 4.0f;
			SkyboxScale_SkyChase1->Near.y = 4.0f;
			SkyboxScale_SkyChase1->Near.z = 4.0f;
			SkyboxScale_SkyChase1->Normal.x = 4.0f;
			SkyboxScale_SkyChase1->Normal.y = 4.0f;
			SkyboxScale_SkyChase1->Normal.z = 4.0f;
			WriteData((char*)0x0062751B, 0x00, 1); //Force Tornado light type
			WriteData((char*)0x0062AC1F, 0x00, 1); //Force Tornado light type (transformation cutscene)
			for (int i = 0; i < 3; i++)
			{
				DrawDist_SkyChase1[i].Maximum = -60000.0f;
			}
		}
		if (GetModuleHandle(TEXT("SA1_Chars.dll")) == nullptr)
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
		if (GetModuleHandle(TEXT("DCMods_Main.dll")) == nullptr) FixMRBase_Apply(path, helperFunctions);

		// Fix Egg Carrier Garden ocean animation
		((LandTable *)0x3405E54)->Col[74].Flags = 0x84000002;
		((NJS_MATERIAL*)0x033FE3F8)->diffuse.color = 0x7FB2B2B2;

		// Fix the water ripple created by air bubbles
		WriteJump(reinterpret_cast<void*>(0x7A81A0), FixedBubbleRipple);

		// Fix Perfect Chaos damage animation
		if (GetModuleHandle(TEXT("DCMods_Main.dll")) == nullptr)
		{
			WriteJump(reinterpret_cast<void*>(0x005632F0), Chaos7Explosion_DisplayX);
			WriteJump(reinterpret_cast<void*>(0x005633C0), Chaos7Damage_DisplayX);
		}
	}
	EXPORT void __cdecl OnFrame()
	{
		if (GetModuleHandle(TEXT("DLCs_Main.dll")) == nullptr)
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
