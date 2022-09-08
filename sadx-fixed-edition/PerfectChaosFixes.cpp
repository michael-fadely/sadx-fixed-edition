#include "stdafx.h"
#include <SADXModLoader.h>

DataPointer(NJS_ARGB, argb_18, 0x1494114);
DataPointer(NJS_ARGB, argb_19, 0x1494124);
DataPointer(NJS_SPRITE, sprite_Chaos7Damage, 0x1494030);
DataPointer(NJS_SPRITE, sprite_Chaos7SDamage, 0x1494064);
DataPointer(NJS_TEXANIM, anim_Chaos7Damage, 0x149401C);
DataPointer(NJS_TEXANIM, anim_Chaos7SDamage, 0x1494050);
NJS_TEXANIM anim_Chaos7Damage_copy[32];
NJS_TEXANIM anim_Chaos7SDamage_copy[32];

// Big explosion sprite
void __cdecl Chaos7Explosion_DisplayX(task* a1)
{
	taskwk* twp = a1->twp;
	int id = (int)twp->counter.f;
	if (!loop_count)
	{
		DisableFog();
		SetMaterialAndSpriteColor(&argb_18);
		njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_ONE);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		njPushMatrix(nullptr);
		njTranslateV(nullptr, &twp->pos);
		sprite_Chaos7Damage.tanim = anim_Chaos7Damage_copy;
		late_DrawSprite3D(&sprite_Chaos7Damage, id, NJD_SPRITE_ALPHA | NJD_SPRITE_SCALE, LATE_LIG);
		njPopMatrix(1u);
		ToggleStageFog();
	}
}

// Small sparks sprite
void __cdecl Chaos7Damage_DisplayX(task* a1)
{
	taskwk* twp = a1->twp;
	__int16 id = twp->timer.w[1] + 16;
	if (!loop_count)
	{
		DisableFog();
		SetMaterialAndSpriteColor(&argb_19);
		njColorBlendingMode(NJD_SOURCE_COLOR, NJD_COLOR_BLENDING_ONE);
		njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
		njPushMatrix(nullptr);
		njTranslateV(nullptr, &twp->pos);
		sprite_Chaos7SDamage.tanim = anim_Chaos7SDamage_copy;
		late_DrawSprite3D(&sprite_Chaos7SDamage, id, NJD_SPRITE_ALPHA | NJD_SPRITE_SCALE, LATE_LIG);
		njPopMatrix(1u);
		ToggleStageFog();
	}
}

void __cdecl PerfectChaosDamageFix_Init()
{
	for (int i = 0; i < 32; i++)
	{
		memcpy(&anim_Chaos7Damage_copy[i], &anim_Chaos7Damage, sizeof(NJS_TEXANIM));
		memcpy(&anim_Chaos7SDamage_copy[i], &anim_Chaos7SDamage, sizeof(NJS_TEXANIM));
		anim_Chaos7Damage_copy[i].texid = i;
		anim_Chaos7SDamage_copy[i].texid = i;
	}
	WriteJump(reinterpret_cast<void*>(0x005632F0), Chaos7Explosion_DisplayX);
	WriteJump(reinterpret_cast<void*>(0x005633C0), Chaos7Damage_DisplayX);
}