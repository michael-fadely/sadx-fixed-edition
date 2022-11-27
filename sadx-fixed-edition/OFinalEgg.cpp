#include "stdafx.h"
#include "OFinalEggModel.h"

// Renders Eggman's base object (OFinalEgg) with the pathway (OFinalWay) by section.
void __cdecl FinalEggDisplayer_r(task* tp)
{
	taskwk* twp = tp->twp;
	___njFogDisable();
	njSetTexture(ADV02_TEXLISTS[1]);
	njPushMatrix(0);
	njTranslateV(0, &twp->pos);
	late_z_ofs___ = -30000.0f;
	Angle rot_y = twp->ang.y;
	if (rot_y)
		njRotateY(0, (unsigned __int16)rot_y);
	// Render the animation without the lights
	late_DrawMotionClip(&OFinalEggModel_A, ADV02_ACTIONS[0]->motion, twp->scl.x, LATE_WZ, 1.0f);
	// Render the transparent part of the animation without the lights
	late_DrawMotionClip(&OFinalEggModel_B, ADV02_ACTIONS[0]->motion, twp->scl.x, LATE_WZ, 1.0f);
	// Render the colored dots
	late_DrawMotionClip(&OFinalEggModel_C, ADV02_ACTIONS[0]->motion, twp->scl.x, LATE_WZ, 1.0f);
	// Render the OFinalWay car
	late_ActionEx(ADV02_ACTIONS[30], twp->scl.y, LATE_WZ);
	// Render the FinalWay without the car
	njPushMatrix(0);
	njTranslateV(0, (NJS_VECTOR*)ADV02_ACTIONS[30]->object->pos);
	late_z_ofs___ = -20000.0f;
	late_DrawModelEx(ADV02_ACTIONS[30]->object->basicdxmodel, LATE_WZ);
	late_DrawModelEx(ADV02_ACTIONS[30]->object->child->basicdxmodel, LATE_WZ);
	njPopMatrix(1u);
	// Render the lights
	late_z_ofs___ = -30000.0f;
	late_DrawMotionClipMesh(&OFinalEggModel_D, ADV02_ACTIONS[0]->motion, twp->scl.x, LATE_LIG, 1.0f);
	// Render the background light
	late_DrawModel(OFinalEggModel_D.child->sibling->sibling->basicdxmodel, LATE_LIG);
	njPopMatrix(1u);
	___njFogEnable();
	late_z_ofs___ = 0.0f;
}