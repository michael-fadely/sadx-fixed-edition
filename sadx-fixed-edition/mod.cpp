#include "stdafx.h"

// Because ain't nobody got time for compiler warnings
#define _arrayptrandlength(data) data, (int)LengthOfArray(data)
#define _arraylengthandptr(data) (int)LengthOfArray(data), data

#define EXPORT __declspec(dllexport)

#include <SADXModLoader.h>

#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"

DataArray(NJS_MATERIAL, matlist_022710E0, 0x026710E0, 5);

// Replaces:	mov	camerathing,	80000004h
// With:		or	camerathing,	80000004h
// Based on information from VeritasDL and code from SADX Steam.
// Note that SADX Steam seems to use 0x8000000C and not 0x80000004, but it doesn't seem to make a difference.
static Uint8 freecam_fix[] = { 0x81, 0x0D, /*0xA8, 0xCB, 0xB2, 0x03, 0x0C, 0x00, 0x00, 0x80*/ }; // Uncomment to change 0x80000004 to 0x8000000C
static Uint8 mt_kusa_nop[] = { 0x90, 0x90 };
static Uint32 CasinoSpawnY = 0xC3480001;	// Secretly a float of about -200.0

static PatchInfo patches[] = {
	// MT_KUSA. Also found at 0x0082F216 in SADX 2010
	{ (void*)(0x00608380 + 0x1D), _arrayptrandlength(mt_kusa_nop) },
	// Action stage load and restart
	{ (void*)0x00438330, _arrayptrandlength(freecam_fix) },
	// Adventure field scene change
	{ (void*)0x00434870, _arrayptrandlength(freecam_fix) },
	// Casinopolis spawn point Y offset fix
	{ (void*)0x005C0D5D, &CasinoSpawnY, sizeof(float) }
};

static PointerInfo jumps[] = {
	// ItemBox
	{ ItemBox_Display_Destroyed,	ItemBox_Display_Destroyed_Rotate },
	{ ItemBox_Display_Unknown,		ItemBox_Display_Unknown_Rotate },
	{ ItemBox_Display,				ItemBox_Display_Rotate },
	{ (void*)0x0042CCC7,			PlaySegaSonicTeamVoice_asm },
	{ (void*)0x0042CD2F,			PlaySegaSonicTeamVoice_asm },
};

extern "C"
{
	EXPORT PatchList Patches[] = { { _arrayptrandlength(patches) } };
	EXPORT PointerList Jumps[] = { { _arrayptrandlength(jumps) } };
	EXPORT ModInfo SADXModInfo = { ModLoaderVer };

	EXPORT void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// Fixes the rotation of the second outcrop on your way out of Emerald Coast 1.
		// This will be done via landtable once animated textures are implemented.
		LandTable* ecmesh110 = (LandTable*)0xE99CB8;
		NJS_OBJECT* obj = ecmesh110->COLList[110].OBJECT;

		obj->ang[0] = 0;
		obj->ang[1] = 0;
		obj->ang[2] = 0;

		// Fixes the inverted water in Emerald Coast 2.
		LandTable* ec2mesh = (LandTable*)0x01039E9C;
		obj = ec2mesh->COLList[1].OBJECT;
		obj->ang[0] = 32768;
		obj->pos[1] = -3.0f;
		obj->pos[2] = -5850.0f;

		// Fixes a rendering issue with the red moving platform in Speed Highway.
		// Thanks PkR!
		matlist_022710E0[0].attrflags &= ~NJD_FLAG_USE_ALPHA;

		// Leon fix
		WriteData((Float**)0x004CD75A, &_nj_screen_.w);
		WriteData((Float**)0x004CD77C, &_nj_screen_.h);

		// Fixes bounds for Sky Chase target reticle
		WriteData((Float**)0x00628994, &HorizontalStretch);
		WriteData((Float**)0x00628A13, &VerticalStretch);

		PlaySegaSonicTeamVoice_init();
	}
}
