#include "stdafx.h"

#define EXPORT __declspec(dllexport)

#include <SADXModLoader.h>
#include <chrono>
#include <thread>

#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"

// Replaces:	mov	camerathing,	80000004h
// With:		or	camerathing,	80000004h
// Based on information from VeritasDL and code from SADX Steam.
// Note that SADX Steam seems to use 0x8000000C and not 0x80000004, but it doesn't seem to make a difference.
Uint8 freecam_fix[]	= { 0x81, 0x0D, /*0xA8, 0xCB, 0xB2, 0x03, 0x0C, 0x00, 0x00, 0x80*/ }; // Uncomment to change 0x80000004 to 0x8000000C
Uint8 mt_kusa_nop[]	= { 0x90, 0x90 };
Uint32 CasinoSpawnY = 0xC3480001;	// Secretly a float of about -200.0

PatchInfo patches[] = {
	// MT_KUSA. Also found at 0x0082F216 in SADX 2010
	{ (void*)(0x00608380 + 0x1D), arrayptrandlength(mt_kusa_nop) },
	// Action stage load and restart
	{ (void*)0x00438330, arrayptrandlength(freecam_fix) },
	// Adventure field scene change
	{ (void*)0x00434870, arrayptrandlength(freecam_fix) },
	// Casinopolis spawn point Y offset fix
	{ (void*)0x005C0D5D, &CasinoSpawnY, sizeof(float) }
};

PointerInfo jumps[] = {
	// ItemBox
	{ ItemBox_Display_Destroyed,	ItemBox_Display_Destroyed_Rotate },
	{ ItemBox_Display_Unknown,		ItemBox_Display_Unknown_Rotate },
	{ ItemBox_Display,				ItemBox_Display_Rotate },
	{ (void*)0x0042CCC7,			PlaySegaSonicTeamVoice_asm },
	{ (void*)0x0042CD2F,			PlaySegaSonicTeamVoice_asm },
};

using namespace std;
using namespace chrono;
using FrameRatio = duration<long, ratio<1, 60>>;

static auto frame_start = high_resolution_clock::now();
static auto frame_time  = FrameRatio{ 1 };
static int last_multi   = 0;

static void __cdecl SetFrameMultiplier(int a1)
{
	if (a1 != last_multi)
	{
		last_multi = a1;
		frame_time = FrameRatio{ a1 };
	}
}

static void __cdecl CustomDeltaSleep()
{
	while (high_resolution_clock::now() - frame_start < frame_time)
		this_thread::yield();

	frame_start = high_resolution_clock::now();
}

extern "C"
{
	EXPORT PatchList Patches[] = { { arrayptrandlength(patches) } };
	EXPORT PointerList Jumps[] = { { arrayptrandlength(jumps) } };
	EXPORT ModInfo SADXModInfo = { ModLoaderVer };

	EXPORT void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
#ifdef _DEBUG
		WriteJump((void*)0x007899E0, CustomDeltaSleep);
		WriteJump((void*)0x007899A0, SetFrameMultiplier);
#endif

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

		// Leon fix
		WriteData((Float**)0x004CD75A, &_nj_screen_.w);
		WriteData((Float**)0x004CD77C, &_nj_screen_.h);

		PlaySegaSonicTeamVoice_init();

		// Fixes bounds for Sky Chase target reticle
		WriteData((Float**)0x00628994, &HorizontalStretch);
		WriteData((Float**)0x00628A13, &VerticalStretch);
	}
}
