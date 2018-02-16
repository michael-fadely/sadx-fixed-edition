#include "stdafx.h"
#include <SADXModLoader.h>

// TODO: Use trampolines
// TODO: Ensure collision is rotated

void __cdecl ItemBox_Display_Destroyed_Rotate(ObjectMaster* _this)
{
	auto v1 = _this->Data1;
	SetTextureToCommon();
	njPushMatrix(nullptr);
	njTranslateV(nullptr, &v1->Position);

	// Rotate
	njRotateEx((Angle*)&v1->Rotation, 0);

	DrawModel(&ItemBox_Base_MODEL);
	njPopMatrix(1u);
}

void __cdecl ItemBox_Display_Unknown_Rotate(ObjectMaster* _this)
{
	auto v1 = _this->Data1;
	if (!MissedFrames)
	{
		ItemBoxPowerups[6].Texture = MetalSonicFlag ? 98 : LifeTextures[GetCurrentCharacterID()];
		if (IsVisible(&v1->Position, 20.0f))
		{
			SetTextureToCommon();
			njPushMatrixEx();

			auto model = (NJS_MODEL_SADX *)late_alloca(44);
			auto material = (NJS_MATERIAL *)late_alloca(20);

			if (model && material)
			{
				njTranslateEx(&v1->Position);

				// Rotate
				njRotateEx((Angle*)&v1->Rotation, 0);

				if (v1->Action != 2)
				{
					auto scale = v1->Scale.z * 0.2f;
					njScale(nullptr, scale, scale, scale);
				}
				njPushMatrixEx();
				njTranslate(nullptr, 0.0f, 7.5f, 0.0f);
				auto v6 = (Uint16)(v1->Scale.y * 65536.0f * 0.002777777777777778f);
				if (v6)
				{
					njRotateY(nullptr, v6);
				}
				memcpy(model, &ItemBox_Item_MODEL, 0x2Cu);
				memcpy(material, ItemBox_Item_MODEL.mats, 0x14u);
				model->mats = material;
				auto texId = ItemBoxPowerups[(int)_this->Data1->Scale.x].Texture;
				ItemBox_CurrentItem = (int)_this->Data1->Scale.x;
				material->attr_texId = texId;
				DrawModel(model);
				njPopMatrixEx();
				DrawQueueDepthBias = 0xC68C4000;

				// This was originally DrawModelIGuess_N, but that's wrong.
				DrawModel(&ItemBox_Base_MODEL);

				DrawModel_Queue(&ItemBox_Capsule_MODEL, QueuedModelFlagsB_EnableZWrite);

				// This was originally DrawModelIGuess_N, but that's wrong.
				DrawModel(&ItemBox_Top_MODEL);

				DrawQueueDepthBias = 0;
			}
			njPopMatrixEx();
		}
	}
}

void __cdecl ItemBox_Display_Rotate(ObjectMaster* _this)
{
	auto v1 = _this->Data1;
	if (!MissedFrames)
	{
		ItemBoxPowerups[6].Texture = MetalSonicFlag ? 98 : LifeTextures[GetCurrentCharacterID()];
		if (IsVisible(&v1->Position, 20.0f))
		{
			SetTextureToCommon();
			njPushMatrix(nullptr);
			njTranslateV(nullptr, &v1->Position);

			// Rotate
			njRotateEx((Angle*)&v1->Rotation, 0);

			if (v1->Action != 2)
			{
				auto scale = v1->Scale.z * 0.2f;
				njScale(nullptr, scale, scale, scale);
			}
			auto model = (NJS_MODEL_SADX *)late_alloca(44);
			auto material = (NJS_MATERIAL *)late_alloca(20);

			if (model && material)
			{
				njPushMatrixEx();
				njTranslate(nullptr, 0.0f, 7.5f, 0.0f);
				auto v6 = (Uint16)(v1->Scale.y * 65536.0f * 0.002777777777777778f);
				if (v6)
				{
					njRotateY(nullptr, v6);
				}
				memcpy(model, &ItemBox_Item_MODEL, 0x2Cu);
				memcpy(material, ItemBox_Item_MODEL.mats, 0x14u);

				model->mats = material;
				auto v7 = ItemBoxPowerups[(int)_this->Data1->Scale.x].Texture;
				ItemBox_CurrentItem = (int)_this->Data1->Scale.x;
				material->attr_texId = v7;

				DrawModel(model);
				njPopMatrixEx();

				// This was originally DrawModelIGuess_N, but that's wrong.
				DrawModel(&ItemBox_Base_MODEL);

				DrawModel_Queue(&ItemBox_Capsule_MODEL, QueuedModelFlagsB_EnableZWrite);

				// This was originally DrawModelIGuess_N, but that's wrong.
				DrawModel(&ItemBox_Top_MODEL);
			}
			njPopMatrixEx();
		}
	}
}