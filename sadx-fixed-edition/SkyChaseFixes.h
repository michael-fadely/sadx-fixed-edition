#pragma once

void SkyChaseFix_UpdateBounds();
void SkyChaseFix_Init();

void __cdecl TornadoTargetSprite_TargetLock_DisplayX(ObjectMaster *a1);
void TornadoTarget_Render(NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags);
void RenderSkyChaseRocket(NJS_POINT3COL *a1, int texnum, NJD_DRAW n, QueuedModelFlagsB a4);
void SetSkyChaseRocketColor(float a, float r, float g, float b);
void __cdecl TornadoCalculateCenterPoint(ObjectMaster *a1);
