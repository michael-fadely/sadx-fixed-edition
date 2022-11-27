#include "stdafx.h"
#include <SADXModLoader.h>

void __cdecl ItemBox_Display_Destroyed_Rotate(task* tp)
{
	auto v1 = tp->twp;
	SetTextureToCommon();
	njPushMatrix(nullptr);
	njTranslateV(nullptr, &v1->pos);

	// Rotate
	njRotateEx((Angle*)&v1->ang, 0);

	dsDrawModel(&model_itembox_boxbody_boxbody);
	njPopMatrix(1u);
}

void __cdecl ItemBox_Display_Water_Rotate(task* tp)
{
	auto v1 = tp->twp;
	if (!loop_count)
	{
		item_info[6].texture_id = gu8flgPlayingMetalSonic ? 98 : levelup_texture[GetPlayerNumber()];
		if (dsCheckViewV(&v1->pos, 20.0f))
		{
			SetTextureToCommon();
			njPushMatrixEx();

			auto model = (NJS_MODEL_SADX *)late_alloca(44);
			auto material = (NJS_MATERIAL *)late_alloca(20);

			if (model && material)
			{
				njTranslateEx(&v1->pos);

				// Rotate
				njRotateEx((Angle*)&v1->ang, 0);

				if (v1->mode != 2)
				{
					auto scale = v1->scl.z * 0.2f;
					njScale(nullptr, scale, scale, scale);
				}
				njPushMatrixEx();
				njTranslate(nullptr, 0.0f, 7.5f, 0.0f);
				auto v6 = (Uint16)(v1->scl.y * 65536.0f * 0.002777777777777778f);
				if (v6)
				{
					njRotateY(nullptr, v6);
				}
				memcpy(model, &model_itembox_boxbody_item_panel, 0x2Cu);
				memcpy(material, model_itembox_boxbody_item_panel.mats, 0x14u);
				model->mats = material;
				auto texId = item_info[(int)tp->twp->scl.x].texture_id;
				item_kind = (int)tp->twp->scl.x;
				material->attr_texId = texId;
				dsDrawModel(model);
				njPopMatrixEx();
				late_z_ofs___ = -27952.0f;
				dsDrawModel(&model_itembox_boxbody_boxbody); // Bottom
				dsDrawModel(&model_itembox_boxbody_cyl3); // Top
				late_DrawModel(&model_itembox_boxbody_cyl2, LATE_LIG); // Glass
				late_z_ofs___ = 0;
			}
			njPopMatrixEx();
		}
	}
}

void __cdecl ItemBox_Display_Rotate(task* tp)
{
	auto v1 = tp->twp;
	if (!loop_count)
	{
		item_info[6].texture_id = gu8flgPlayingMetalSonic ? 98 : levelup_texture[GetPlayerNumber()];
		if (dsCheckViewV(&v1->pos, 20.0f))
		{
			SetTextureToCommon();
			njPushMatrix(nullptr);
			njTranslateV(nullptr, &v1->pos);

			// Rotate
			njRotateEx((Angle*)&v1->ang, 0);

			if (v1->mode != 2)
			{
				auto scale = v1->scl.z * 0.2f;
				njScale(nullptr, scale, scale, scale);
			}
			auto model = (NJS_MODEL_SADX *)late_alloca(44);
			auto material = (NJS_MATERIAL *)late_alloca(20);

			if (model && material)
			{
				njPushMatrixEx();
				njTranslate(nullptr, 0.0f, 7.5f, 0.0f);
				auto v6 = (Uint16)(v1->scl.y * 65536.0f * 0.002777777777777778f);
				if (v6)
				{
					njRotateY(nullptr, v6);
				}
				memcpy(model, &model_itembox_boxbody_item_panel, 0x2Cu);
				memcpy(material, model_itembox_boxbody_item_panel.mats, 0x14u);

				model->mats = material;
				auto v7 = item_info[(int)tp->twp->scl.x].texture_id;
				item_kind = (int)tp->twp->scl.x;
				material->attr_texId = v7;

				dsDrawModel(model); // Item icon
				njPopMatrixEx();

				dsDrawModel(&model_itembox_boxbody_boxbody); // Bottom
				dsDrawModel(&model_itembox_boxbody_cyl3); // Top
				late_z_ofs___ = 1000.0f;
				late_DrawModel(&model_itembox_boxbody_cyl2, LATE_LIG); // Glass
				late_z_ofs___ = 0.0f;
				
			}
			njPopMatrixEx();
		}
	}
}