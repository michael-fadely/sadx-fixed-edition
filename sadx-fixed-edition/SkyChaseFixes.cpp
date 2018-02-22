#include "stdafx.h"
#include <SADXModLoader.h>
#include "SkyChaseFixes.h"

DataArray(SkyboxScale, SkyboxScale_SkyChase1, 0x027D6CE0, 3);
DataArray(DrawDistance, DrawDist_SkyChase1, 0x027D6D58, 3);

static float HorizontalResolution_float = 640.0f;
static float VerticalResolution_float   = 480.0f;

static float float_one                        = 1.0f;
static float tornado_speed                    = 1.0f;
static float tornado_target_size              = 1.0f;
static float tornado_reticle_speed_multiplier = 2.0f;
static float VerticalResolutionHalf_float     = 240.0f;
static float SkyChaseLimit_Right              = 560.0f;
static float SkyChaseLimit_Left               = 80.0f;
static float SkyChaseLimit_Top                = 400.0f;
static float SkyChaseLimit_Bottom             = 80.0f;
static float widescreenthing                  = 103.0f;

static double SkyChaseSkyRotationMultiplier = -0.5f;

void SkyChaseFix_UpdateBounds()
{
	// Resolution related fixes
	HorizontalResolution_float = static_cast<float>(HorizontalResolution);
	VerticalResolution_float = static_cast<float>(VerticalResolution);
	VerticalResolutionHalf_float = VerticalResolution_float / 2.0f;

	if (HorizontalResolution_float / VerticalResolution_float > 1.4f)
	{
		if (HorizontalResolution_float / VerticalResolution_float > 2.2f)
		{
			widescreenthing = 240.0f;
		}

		SkyChaseLimit_Left = 80.0f + widescreenthing;
		SkyChaseLimit_Right = 560.0f + widescreenthing;
	}

	// Sky Chase reticle and multiplier fixes
	tornado_reticle_speed_multiplier = VerticalResolution / 480.0f;
	tornado_target_size = pow(VerticalResolution / 15.0f, 2);
}

void SkyChaseFix_Init()
{
	SkyChaseFix_UpdateBounds();

	WriteData(reinterpret_cast<float**>(0x00627F4D), &tornado_speed); // Tornado Speed (always 1)
	WriteData(reinterpret_cast<float**>(0x00627F60), &float_one);     // Horizontal limit
	WriteData(reinterpret_cast<float**>(0x00627F72), &float_one);     // Vertical limit

	WriteJump(reinterpret_cast<void*>(0x628D50), TornadoCalculateCenterPoint); // Calculate center for bullets

	// Hodai fixes
	WriteData(reinterpret_cast<float**>(0x0043854D), &HorizontalResolution_float);
	WriteData(reinterpret_cast<float**>(0x00438571), &VerticalResolutionHalf_float);
	WriteData(reinterpret_cast<float**>(0x0043857F), &VerticalResolutionHalf_float);
	WriteCall(reinterpret_cast<void*>(0x0062C764), SetSkyChaseRocketColor);
	WriteCall(reinterpret_cast<void*>(0x0062C704), RenderSkyChaseRocket);

	WriteData(reinterpret_cast<float**>(0x628AF7), &tornado_target_size);                // Target size
	WriteData(reinterpret_cast<float**>(0x00629472), &tornado_reticle_speed_multiplier); // Target speed

	// Limits for reticle
	WriteData(reinterpret_cast<float**>(0x00628994), &tornado_reticle_speed_multiplier); // right
	WriteData(reinterpret_cast<float**>(0x006289B6), &tornado_reticle_speed_multiplier); // left
	WriteData(reinterpret_cast<float**>(0x006289F1), &tornado_reticle_speed_multiplier); // top
	WriteData(reinterpret_cast<float**>(0x00628A13), &tornado_reticle_speed_multiplier); // bottom
	WriteData(reinterpret_cast<float**>(0x0062899A), &SkyChaseLimit_Right);
	WriteData(reinterpret_cast<float**>(0x006289BC), &SkyChaseLimit_Left);
	WriteData(reinterpret_cast<float**>(0x006289F7), &SkyChaseLimit_Top);
	WriteData(reinterpret_cast<float**>(0x00628A19), &SkyChaseLimit_Bottom);

	// Visual stuff
	WriteCall(reinterpret_cast<void*>(0x00629004), TornadoTarget_Render);
	WriteCall(reinterpret_cast<void*>(0x00628FE5), TornadoTarget_Render);
	WriteJump(reinterpret_cast<void*>(0x00628DB0), TornadoTargetSprite_TargetLock_DisplayX);
	WriteData(reinterpret_cast<double**>(0x00627D14), &SkyChaseSkyRotationMultiplier);

	// Rotate the sky in the opposite direction
	WriteData(reinterpret_cast<float*>(0x00628951), VerticalResolution / 480.0f); // Reticle scale X
	WriteData(reinterpret_cast<float*>(0x0062895B), VerticalResolution / 480.0f); // Reticle scale Y

	reinterpret_cast<NJS_OBJECT*>(0x028DFD34)->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF; // Sky materials in Act 1
	reinterpret_cast<NJS_OBJECT*>(0x028175F4)->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF; // Sky materials in Act 1

	SkyboxScale_SkyChase1->Far.x    = 4.0f;
	SkyboxScale_SkyChase1->Far.y    = 4.0f;
	SkyboxScale_SkyChase1->Far.z    = 4.0f;
	SkyboxScale_SkyChase1->Near.x   = 4.0f;
	SkyboxScale_SkyChase1->Near.y   = 4.0f;
	SkyboxScale_SkyChase1->Near.z   = 4.0f;
	SkyboxScale_SkyChase1->Normal.x = 4.0f;
	SkyboxScale_SkyChase1->Normal.y = 4.0f;
	SkyboxScale_SkyChase1->Normal.z = 4.0f;

	WriteData(reinterpret_cast<char*>(0x0062751B), nullptr, 1); // Force Tornado light type
	WriteData(reinterpret_cast<char*>(0x0062AC1F), nullptr, 1); // Force Tornado light type (transformation cutscene)

	for (int i = 0; i < 3; i++)
	{
		DrawDist_SkyChase1[i].Maximum = -60000.0f;
	}
}

void __cdecl TornadoTargetSprite_TargetLock_DisplayX(ObjectMaster* a1)
{
	NJS_POINT2 position; // [esp+4h] [ebp-8h]

	EntityData1* v1 = a1->Data1;

	if (!MissedFrames && !CheckSkyChaseActionThing())
	{
		njPushMatrix(nullptr);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_ONE);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		SetMaterialAndSpriteColor_Float(1.0f, 1.0f, 1.0f, 1.0f);
		njSetTexture(&TARGET_TEXLIST);
		njProjectScreen(nullptr, &v1->Position, &position);
		*reinterpret_cast<NJS_POINT2*>(&TornadoTarget_SPRITE.p) = position;
		TornadoTarget_SPRITE.sy = v1->Scale.y * (VerticalResolution / 480.0f);
		njTextureShadingMode(1);

		if (GameState != 16)
		{
			njDrawSprite2D_ForcePriority(&TornadoTarget_SPRITE, 2, 1000.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
		}

		njPopMatrix(1u);
		njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_INVSRCALPHA);
		ClampGlobalColorThing_Thing();
		njTextureShadingMode(2);
	}
}

void TornadoTarget_Render(NJS_SPRITE* sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags)
{
	njTextureShadingMode(1);
	njDrawSprite2D_Queue(sp, n, pri, attr, queue_flags);
	njTextureShadingMode(2);
}

void RenderSkyChaseRocket(NJS_POINT3COL* a1, int texnum, NJD_DRAW n, QueuedModelFlagsB a4)
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

void __cdecl TornadoCalculateCenterPoint(ObjectMaster* a1)
{
	EntityData1* data1        = a1->Data1;
	EntityData1* parent_data1 = a1->Parent->Data1;
	NJS_VECTOR*  position     = &data1->Position;

	data1->Position.x = parent_data1->Position.x - (HorizontalResolution_float * 0.5f);
	data1->Position.z = 1000.0f * (VerticalResolution_float / 480.0f);
	data1->Position.y = parent_data1->Position.y - (VerticalResolution_float * 0.5f);

	njPushMatrix(nullptr);
	njInvertMatrix(nullptr);
	njCalcPoint(nullptr, position, position);
	njPopMatrix(1u);
}
