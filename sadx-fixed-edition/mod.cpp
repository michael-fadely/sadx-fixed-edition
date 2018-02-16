#include "stdafx.h"

#include <SADXModLoader.h>

#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"
#include "barrel.h"

DataArray(NJS_MATERIAL, matlist_022710E0, 0x026710E0, 5);
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

double __cdecl AmenboFix(float a1, float a2, float a3, int a4)
{
	double u;
	u = sub_49EAD0(a1, a2, a3, a4);
	if (u == -1000000) u = a2;
	return u;
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

static PointerInfo jumps[] = {
	// ItemBox
	{ ItemBox_Display_Destroyed,           ItemBox_Display_Destroyed_Rotate },
	{ ItemBox_Display_Unknown,             ItemBox_Display_Unknown_Rotate },
	{ ItemBox_Display,                     ItemBox_Display_Rotate },
	{ reinterpret_cast<void*>(0x0042CCC7), PlaySegaSonicTeamVoice_asm },
	{ reinterpret_cast<void*>(0x0042CD2F), PlaySegaSonicTeamVoice_asm },
};

extern "C"
{
	EXPORT PatchList   Patches[]   = { { _arrayptrandlength(patches) } };
	EXPORT PointerList Jumps[]     = { { _arrayptrandlength(jumps) } };
	EXPORT ModInfo     SADXModInfo = { ModLoaderVer };

	EXPORT void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
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
		// Thanks PkR!
		matlist_022710E0[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Leon fix
		WriteData(reinterpret_cast<float**>(0x004CD75A), &_nj_screen_.w);
		WriteData(reinterpret_cast<float**>(0x004CD77C), &_nj_screen_.h);

		// Fixes bounds for Sky Chase target reticle
		WriteData(reinterpret_cast<float**>(0x00628994), &HorizontalStretch);
		WriteData(reinterpret_cast<float**>(0x00628A13), &VerticalStretch);

		// Makes the animated MtKusa model show up at larger distances
		WriteData(reinterpret_cast<float**>(0x00608331), &KusaDistance);

		// Fixes missing Sweep badniks in Emerald Coast 2 and Twinkle Park 2
		if (GetModuleHandle(TEXT("DCMods_Main.dll")) == nullptr) WriteCall(reinterpret_cast<void*>(0x007AA9F9), AmenboFix);

		// Replace the non-updated Eggmobile NPC model with a high-poly one to resolve a texture issue
		*reinterpret_cast<NJS_OBJECT *>(0x010FEF74) = *reinterpret_cast<NJS_OBJECT *>(0x02EEB524);

		// Replace the texlist for the above model in the NPC data array
		WriteData(reinterpret_cast<NJS_TEXLIST**>(0x007D2B22), reinterpret_cast<NJS_TEXLIST*>(0x02EE0AA4));

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
	}
}
