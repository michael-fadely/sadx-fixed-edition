#pragma once

#include <SADXStructsNew.h>

DataArray(CCL_INFO, itembox_colli_info0, 0x009BF0C0, 2); // Item box collision shape 1
DataArray(CCL_INFO, itembox_colli_info1, 0x009BF120, 2); // Item box collision shape 2
DataPointer(NJS_MODEL_SADX, model_itembox_boxbody_boxbody, 0x8BF260);
DataPointer(NJS_MODEL_SADX, model_itembox_boxbody_item_panel, 0x8BE270);
DataPointer(NJS_MODEL_SADX, model_itembox_boxbody_cyl2, 0x8BEAA8);
DataPointer(NJS_MODEL_SADX, model_itembox_boxbody_cyl3, 0x8BE620);

void __cdecl ItemBox_Display_Destroyed_Rotate(task* tp);
void __cdecl ItemBox_Display_Water_Rotate(task* tp);
void __cdecl ItemBox_Display_Rotate(task* tp);