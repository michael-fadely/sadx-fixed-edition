#include "stdafx.h"
#include <SADXModLoader.h>

DataPointer(int, WhatIsThisAndWhyIsItAlmostAlwaysZero, 0x3B1117C);

DataPointer(NJS_MODEL, ItemBox_Base_MODEL, 0x8BF260);
DataPointer(NJS_MODEL, stru_8BE270, 0x8BE270);
DataPointer(float, dword_3ABD9C0, 0x3ABD9C0);
DataPointer(NJS_MODEL, stru_8BEAA8, 0x8BEAA8);
DataPointer(NJS_MODEL, stru_8BE620, 0x8BE620);

// TODO: Do this with code patches instead.

void __cdecl ItemBox_Display_Destroyed_Rotate(ObjectMaster* thing)
{
	//PrintDebug("ItemBox_Display_Destroyed\n");

	// esi@1
	auto entity = thing->Data1;
	SetCurrentTextureToCommon();
	njPushMatrix(nullptr);
	njTranslateV(nullptr, &entity->Position);
	njRotateZYX(nullptr, entity->Rotation.x, (ControllerPointers[0]->HeldButtons & Buttons_C) ? entity->Rotation.y : 0, entity->Rotation.z);

	DrawModelIGuess(&ItemBox_Base_MODEL);
	njPopMatrix(1);
}

void __cdecl ItemBox_Display_Unknown_Rotate(ObjectMaster* thing)
{
	// esi@1
	auto entity = thing->Data1;
	if (!WhatIsThisAndWhyIsItAlmostAlwaysZero)
	{
		ItemBoxPowerups[6].Texture = MetalSonicFlag ? 98 : LifeTextures[GetCurrentCharacterID()];

		if (CheckModelDistance_v(&entity->Position, 20.0))
		{
			SetCurrentTextureToCommon();
			PushMatrix();

			// ebx@6
			auto attach = (NJS_MODEL*)late_alloca(44);

			// eax@6
			auto material = (NJS_MATERIAL*)late_alloca(20);

			if (attach && material)
			{
				njTranslateEx(&entity->Position);
				njRotateZYX(nullptr, entity->Rotation.x, (ControllerPointers[0]->HeldButtons & Buttons_C) ? entity->Rotation.y : 0, entity->Rotation.z);

				if (entity->Action != 2)
				{
					// ST28_4@9
					float scale = entity->Scale.z * 0.2000000029802322f;
					njScale(nullptr, scale, scale, scale);
				}

				PushMatrix();
				njTranslate(nullptr, 0.0, 7.5, 0.0);

				// st7@10
				double v6 = entity->Scale.y * 65536.0 * 0.002777777777777778;
				if ((unsigned int)(unsigned __int64)v6)
					njRotateY(nullptr, (unsigned __int16)(unsigned __int64)v6);

				memcpy(attach, &stru_8BE270, 0x2Cu);
				memcpy(material, stru_8BE270.mats, 0x14u);

				attach->mats = material;
				// ecx@12
				int v7 = ItemBoxPowerups[(unsigned __int64)thing->Data1->Scale.x].Texture;
				ItemBoxPowerups_Index = (unsigned __int64)thing->Data1->Scale.x;
				material->attr_texId = v7;
				DrawModelIGuess(attach);
				PopMatrix();

				dword_3ABD9C0 = -17952;
				DrawModelIGuess_N(&ItemBox_Base_MODEL, 5);
				DrawModelIGuess_N(&stru_8BEAA8, 5);
				DrawModelIGuess_N(&stru_8BE620, 5);
				dword_3ABD9C0 = 0;
			}

			PopMatrix();
		}
	}
}

void __cdecl ItemBox_Display_Rotate(ObjectMaster* thing)
{
	//PrintDebug("ItemBox_Display\n");

	if (!WhatIsThisAndWhyIsItAlmostAlwaysZero)
	{
		ItemBoxPowerups[6].Texture = MetalSonicFlag ? 98 : LifeTextures[GetCurrentCharacterID()];

		// esi@1
		auto entity = thing->Data1;
		if (CheckModelDistance_v(&entity->Position, 20.0))
		{
			SetCurrentTextureToCommon();
			njPushMatrix(nullptr);
			njTranslateV(nullptr, &entity->Position);
			njRotateZYX(nullptr, entity->Rotation.x, (ControllerPointers[0]->HeldButtons & Buttons_C) ? entity->Rotation.y : 0, entity->Rotation.z);

			if (entity->Action != 2)
			{
				// ST28_4@7
				float v2 = entity->Scale.z * 0.2000000029802322f;
				njScale(nullptr, v2, v2, v2);
			}

			// ebx@8
			auto attach = (NJS_MODEL*)late_alloca(44);
			// eax@8
			auto material = (NJS_MATERIAL*)late_alloca(20);

			if (attach && material)
			{
				PushMatrix();
				njTranslate(nullptr, 0.0, 7.5, 0.0);

				// st7@10
				double v6 = entity->Scale.y * 65536.0 * 0.002777777777777778;
				if ((unsigned int)(unsigned __int64)v6)
					njRotateY(nullptr, (unsigned __int16)(unsigned __int64)v6);

				memcpy(attach, &stru_8BE270, 0x2Cu);
				memcpy(material, stru_8BE270.mats, 0x14u);
				attach->mats = material;

				// ecx@12
				int v7 = ItemBoxPowerups[(unsigned __int64)thing->Data1->Scale.x].Texture;
				ItemBoxPowerups_Index = (unsigned __int64)thing->Data1->Scale.x;

				material->attr_texId = v7;
				DrawModelIGuess(attach);
				PopMatrix();

				DrawModelIGuess_N(&ItemBox_Base_MODEL, 5);
				DrawModelIGuess_N(&stru_8BEAA8, 5);
				DrawModelIGuess_N(&stru_8BE620, 5);
			}

			PopMatrix();
		}
	}
}