#include "stdafx.h"
#include <SADXModLoader.h>
#include "SkyChaseFixes.h"

void __cdecl TornadoTargetSprite_TargetLock_DisplayX(ObjectMaster *a1)
{
	EntityData1 *v1; // esi
	NJS_POINT2 position; // [esp+4h] [ebp-8h]

	v1 = a1->Data1;
	if (!MissedFrames && !CheckSkyChaseActionThing())
	{
		njPushMatrix(0);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_ONE);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		SetMaterialAndSpriteColor_Float(1.0f, 1.0f, 1.0f, 1.0f);
		njSetTexture(&TARGET_TEXLIST);
		njProjectScreen(0, &v1->Position, &position);
		*(NJS_POINT2 *)&TornadoTarget_SPRITE.p.x = position;
		TornadoTarget_SPRITE.sy = v1->Scale.y * (VerticalResolution / 480.0f);
		njTextureShadingMode(1);
		if (GameState != 16) njDrawSprite2D_ForcePriority(&TornadoTarget_SPRITE, 2, 1000.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
		njPopMatrix(1u);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
		ClampGlobalColorThing_Thing();
		njTextureShadingMode(2);
	}
}

void TornadoTarget_Render(NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags)
{
	njTextureShadingMode(1);
	njDrawSprite2D_Queue(sp, n, pri, attr, queue_flags);
	njTextureShadingMode(2);
}

void RenderSkyChaseRocket(NJS_POINT3COL *a1, int texnum, NJD_DRAW n, QueuedModelFlagsB a4)
{
	DrawQueueDepthBias = 20000.0f;
	DrawTriFanThing_Queue(a1, texnum, n, QueuedModelFlagsB_SomeTextureThing);
	DrawQueueDepthBias = 0;
}

void SetSkyChaseRocketColor(float a, float r, float g, float b)
{
	AddConstantAttr(0, NJD_FLAG_IGNORE_LIGHT);
	SetMaterialAndSpriteColor_Float(1.0f, 1.0f, 1.0f, 1.0f);
}

void __cdecl TornadoCalculateCenterPoint(ObjectMaster *a1)
{
	EntityData1 *data1; // eax
	EntityData1 *parent_data1; // ecx
	NJS_VECTOR *position; // esi
	double y; // st7

	data1 = a1->Data1;
	parent_data1 = a1->Parent->Data1;
	position = &data1->Position;
	data1->Position.x = parent_data1->Position.x - (HorizontalResolution_float * 0.5f);
	y = parent_data1->Position.y - (VerticalResolution_float * 0.5f);
	data1->Position.z = 1000.0f * (VerticalResolution_float / 480.0f);
	data1->Position.y = y;
	njPushMatrix(0);
	njInvertMatrix(0);
	njCalcPoint(0, position, position);
	njPopMatrix(1u);
}
