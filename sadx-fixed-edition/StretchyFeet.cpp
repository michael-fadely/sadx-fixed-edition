#include "stdafx.h"

static HMODULE CHRMODELS = GetModuleHandle(L"CHRMODELS_orig");

// Fixes Sonic's broken stretchy feet models by replacing their mesh data with data from the full Sonic model.
// This fix patches the original meshsets directly to ensure compatibility with mods that alter these models.
void __cdecl FixStretchyFeet()
{
	// Stretchy foot meshset pointers: unstretched, middle, full stretch
	NJS_MESHSET_SADX* meshset_left_0 = (NJS_MESHSET_SADX*)((size_t)CHRMODELS + 0x005730D8);
	NJS_MESHSET_SADX* meshset_left_1 = (NJS_MESHSET_SADX*)((size_t)CHRMODELS + 0x00573928);
	NJS_MESHSET_SADX* meshset_left_2 = (NJS_MESHSET_SADX*)((size_t)CHRMODELS + 0x00574178);
	NJS_MESHSET_SADX* meshset_right_0 = (NJS_MESHSET_SADX*)((size_t)CHRMODELS + 0x00574D88);
	NJS_MESHSET_SADX* meshset_right_1 = (NJS_MESHSET_SADX*)((size_t)CHRMODELS + 0x005755E0);
	NJS_MESHSET_SADX* meshset_right_2 = (NJS_MESHSET_SADX*)((size_t)CHRMODELS + 0x00575E38);

	// Pointers to correct poly and UV arrays in the full model
	Sint16* poly_left_0 = (Sint16*)((size_t)CHRMODELS + 0x0055EB88);
	Sint16* poly_left_1 = (Sint16*)((size_t)CHRMODELS + 0x0055EC70);
	Sint16* poly_right_0 = (Sint16*)((size_t)CHRMODELS + 0x00560638);
	Sint16* poly_right_1 = (Sint16*)((size_t)CHRMODELS + 0x0056071C);
	NJS_TEX* uv_left_0 = (NJS_TEX*)((size_t)CHRMODELS + 0x0055EC80);
	NJS_TEX* uv_left_1 = (NJS_TEX*)((size_t)CHRMODELS + 0x0055EE1C);
	NJS_TEX* uv_right_0 = (NJS_TEX*)((size_t)CHRMODELS + 0x00560728);
	NJS_TEX* uv_right_1 = (NJS_TEX*)((size_t)CHRMODELS + 0x005608BC);

	// Replace pointers and mesh count: left foot
	meshset_left_0[0].meshes = meshset_left_1[0].meshes = meshset_left_2[0].meshes = poly_left_0;
	meshset_left_0[1].meshes = meshset_left_1[1].meshes = meshset_left_2[1].meshes = poly_left_1;
	meshset_left_0[0].vertuv = meshset_left_1[0].vertuv = meshset_left_2[0].vertuv = uv_left_0;
	meshset_left_0[1].vertuv = meshset_left_1[1].vertuv = meshset_left_2[1].vertuv = uv_left_1;
	meshset_left_0[0].nbMesh = meshset_left_1[0].nbMesh = meshset_left_2[0].nbMesh = 13;
	meshset_left_0[1].nbMesh = meshset_left_1[1].nbMesh = meshset_left_2[1].nbMesh = 1;

	// Replace pointers and mesh count: right foot
	meshset_right_0[0].meshes = meshset_right_1[0].meshes = meshset_right_2[0].meshes = poly_right_0;
	meshset_right_0[1].meshes = meshset_right_1[1].meshes = meshset_right_2[1].meshes = poly_right_1;
	meshset_right_0[0].vertuv = meshset_right_1[0].vertuv = meshset_right_2[0].vertuv = uv_right_0;
	meshset_right_0[1].vertuv = meshset_right_1[1].vertuv = meshset_right_2[1].vertuv = uv_right_1;
	meshset_right_0[0].nbMesh = meshset_right_1[0].nbMesh = meshset_right_2[0].nbMesh = 12;
	meshset_right_0[1].nbMesh = meshset_right_1[1].nbMesh = meshset_right_2[1].nbMesh = 1;
}