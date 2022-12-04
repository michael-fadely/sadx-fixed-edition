#include "stdafx.h"
#include <SADXModLoader.h>

// Various fixes for landtable items

DataArray(_OBJ_LANDENTRY, objLandEntry_10, 0x00E95938, 480); // Emerald Coast Act 1 COL list
DataArray(_OBJ_LANDENTRY, objLandEntry_49, 0x010386D8, 169); // Emerald Coast Act 2 COL list
DataArray(_OBJ_LANDENTRY, objLandEntry_44, 0x026ADBF0, 474); // Speed Highway Act 1 COL list
DataArray(_OBJ_LANDENTRY, objLandEntry_9, 0x0223B408, 582); // Red Mountain Act 1 COL list
DataArray(_OBJ_LANDENTRY, objLandEntry_37, 0x024C3980, 381); // Red Mountain Act 3 COL list
DataArray(_OBJ_LANDENTRY, objLandEntry_6, 0x03405290, 83); // Egg Carrier Garden COL list

void __cdecl LandtableFixes_Init()
{
	// Fixes the rotation of the second outcrop on your way out of Emerald Coast 1.
	NJS_OBJECT* obj = objLandEntry_10[110].pObject;

	obj->ang[0] = 0;
	obj->ang[1] = 0;
	obj->ang[2] = 0;

	// Fixes the inverted water in Emerald Coast 2.
	obj = objLandEntry_49[1].pObject;

	obj->ang[0] = 32768;
	obj->pos[1] = -3.0f;
	obj->pos[2] = -5850.0f;

	// Fixes Egg Carrier Garden ocean animation by adding the "disable meshset buffer" flag to the COL item.
	objLandEntry_6[74].slAttribute |= 0x4000000;

	// Fixes buildings in Speed Highway being not solid.
	objLandEntry_44[91].slAttribute = 0x80000001;
	objLandEntry_44[154].slAttribute = 0x80000001;
	objLandEntry_44[157].slAttribute = 0x80000001;
	objLandEntry_44[164].slAttribute = 0x80000001;
	objLandEntry_44[165].slAttribute = 0x80000001;
	objLandEntry_44[184].slAttribute = 0x80000001;
	objLandEntry_44[185].slAttribute = 0x80000001;
	objLandEntry_44[186].slAttribute = 0x80000001;
	objLandEntry_44[187].slAttribute = 0x80000001;
	objLandEntry_44[198].slAttribute = 0x80000001;
	objLandEntry_44[208].slAttribute = 0x80000021;
	objLandEntry_44[209].slAttribute = 0x80000021;
	objLandEntry_44[210].slAttribute = 0x80000001;
	objLandEntry_44[211].slAttribute = 0x80000001;
	objLandEntry_44[212].slAttribute = 0x80000001;
	objLandEntry_44[213].slAttribute = 0x80000001;
	objLandEntry_44[214].slAttribute = 0x80000001;
	objLandEntry_44[215].slAttribute = 0x80000001;
	objLandEntry_44[216].slAttribute = 0x80000001;
	objLandEntry_44[217].slAttribute = 0x80000001;
	objLandEntry_44[218].slAttribute = 0x80000001;
	objLandEntry_44[219].slAttribute = 0x80000001;
	objLandEntry_44[220].slAttribute = 0x80000001;
	objLandEntry_44[221].slAttribute = 0x80000001;
	objLandEntry_44[222].slAttribute = 0x80000001;
	objLandEntry_44[223].slAttribute = 0x80000001;
	objLandEntry_44[224].slAttribute = 0x80000001;
	objLandEntry_44[225].slAttribute = 0x80000001;
	objLandEntry_44[227].slAttribute = 0x80000001;
	objLandEntry_44[228].slAttribute = 0x80000001;
	objLandEntry_44[229].slAttribute = 0x80000001;
	objLandEntry_44[230].slAttribute = 0x80000001;
	objLandEntry_44[231].slAttribute = 0x80000001;
	objLandEntry_44[232].slAttribute = 0x80000001;
	objLandEntry_44[233].slAttribute = 0x80000001;
	objLandEntry_44[234].slAttribute = 0x80000001;
	objLandEntry_44[235].slAttribute = 0x80000001;
	objLandEntry_44[236].slAttribute = 0x80000001;
	objLandEntry_44[237].slAttribute = 0x80000001;
	objLandEntry_44[238].slAttribute = 0x80000001;
	objLandEntry_44[239].slAttribute = 0x80000001;
	objLandEntry_44[240].slAttribute = 0x80000001;
	objLandEntry_44[241].slAttribute = 0x80000001;
	objLandEntry_44[242].slAttribute = 0x80000001;
	objLandEntry_44[243].slAttribute = 0x80000021;
	objLandEntry_44[244].slAttribute = 0x80000001;
	objLandEntry_44[245].slAttribute = 0x80000001;
	objLandEntry_44[246].slAttribute = 0x80000001;
	objLandEntry_44[247].slAttribute = 0x80000001;
	objLandEntry_44[248].slAttribute = 0x80000001;
	objLandEntry_44[249].slAttribute = 0x80000001;
	objLandEntry_44[250].slAttribute = 0x80000001;
	objLandEntry_44[251].slAttribute = 0x80000001;
	objLandEntry_44[252].slAttribute = 0x80000001;
	objLandEntry_44[253].slAttribute = 0x80000001;
	objLandEntry_44[256].slAttribute = 0x80000001;
	objLandEntry_44[257].slAttribute = 0x80000001;
	objLandEntry_44[258].slAttribute = 0x80000001;
	objLandEntry_44[260].slAttribute = 0x80000001;
	objLandEntry_44[261].slAttribute = 0x80000001;
	objLandEntry_44[262].slAttribute = 0x80000001;
	
	// Fixes ground in Red Mountain Act 1.
	objLandEntry_9[18].slAttribute = 0x20000000;
	objLandEntry_9[19].slAttribute = 0x40000000;
	objLandEntry_9[20].slAttribute = 0x20000000;
	objLandEntry_9[38].slAttribute = 0x00000000;
	objLandEntry_9[40].slAttribute = 0x20000000;
	objLandEntry_9[41].slAttribute = 0x00000000;
	objLandEntry_9[45].slAttribute = 0x20004000;
	objLandEntry_9[48].slAttribute = 0x80000001;
	objLandEntry_9[49].slAttribute = 0x80000001;
	objLandEntry_9[50].slAttribute = 0x80000001;
	objLandEntry_9[54].slAttribute = 0x80000001;
	objLandEntry_9[55].slAttribute = 0x80000001;
	objLandEntry_9[56].slAttribute = 0x80000001;
	objLandEntry_9[57].slAttribute = 0x80000001;
	objLandEntry_9[64].slAttribute = 0x80000001;
	objLandEntry_9[65].slAttribute = 0x80000001;
	objLandEntry_9[66].slAttribute = 0x80000001;
	objLandEntry_9[67].slAttribute = 0x80000001;
	objLandEntry_9[70].slAttribute = 0x80000001;
	objLandEntry_9[74].slAttribute = 0x80000001;
	objLandEntry_9[75].slAttribute = 0x80000001;
	objLandEntry_9[76].slAttribute = 0x80000001;
	objLandEntry_9[79].slAttribute = 0x80000001;
	objLandEntry_9[86].slAttribute = 0x80000001;
	objLandEntry_9[87].slAttribute = 0x80000001;
	objLandEntry_9[88].slAttribute = 0x80000001;
	objLandEntry_9[89].slAttribute = 0x80000001;
	objLandEntry_9[90].slAttribute = 0x80000001;
	objLandEntry_9[91].slAttribute = 0x80000001;
	objLandEntry_9[92].slAttribute = 0x80000001;
	objLandEntry_9[99].slAttribute = 0x80000001;
	objLandEntry_9[101].slAttribute = 0x80000001;
	objLandEntry_9[108].slAttribute = 0x80000001;
	objLandEntry_9[109].slAttribute = 0x80000001;
	objLandEntry_9[111].slAttribute = 0x80000001;
	objLandEntry_9[112].slAttribute = 0x80000001;
	objLandEntry_9[113].slAttribute = 0x80000001;
	objLandEntry_9[114].slAttribute = 0x80000001;
	objLandEntry_9[115].slAttribute = 0x80000001;
	objLandEntry_9[116].slAttribute = 0x80000001;
	objLandEntry_9[118].slAttribute = 0x80000001;
	objLandEntry_9[120].slAttribute = 0x80000001;
	objLandEntry_9[121].slAttribute = 0x80000001;
	objLandEntry_9[122].slAttribute = 0x80000001;
	objLandEntry_9[123].slAttribute = 0x80000001;
	objLandEntry_9[127].slAttribute = 0x80000001;
	objLandEntry_9[128].slAttribute = 0x80000001;
	objLandEntry_9[129].slAttribute = 0x80000001;
	objLandEntry_9[130].slAttribute = 0x80000001;
	objLandEntry_9[131].slAttribute = 0x80000001;
	objLandEntry_9[132].slAttribute = 0x80000001;
	objLandEntry_9[133].slAttribute = 0x80000001;
	objLandEntry_9[134].slAttribute = 0x80000001;
	objLandEntry_9[135].slAttribute = 0x80000001;
	objLandEntry_9[136].slAttribute = 0x80000001;
	objLandEntry_9[138].slAttribute = 0x80000001;
	objLandEntry_9[139].slAttribute = 0x80000001;
	objLandEntry_9[141].slAttribute = 0x80000001;
	objLandEntry_9[143].slAttribute = 0x80000001;
	objLandEntry_9[144].slAttribute = 0x80000001;
	objLandEntry_9[145].slAttribute = 0x80000001;
	objLandEntry_9[146].slAttribute = 0x80000001;
	objLandEntry_9[149].slAttribute = 0x80000001;
	objLandEntry_9[150].slAttribute = 0x80000001;
	objLandEntry_9[151].slAttribute = 0x80000001;
	objLandEntry_9[152].slAttribute = 0x80000001;
	objLandEntry_9[154].slAttribute = 0x80000001;
	objLandEntry_9[173].slAttribute = 0x80000001;
	objLandEntry_9[180].slAttribute = 0x80000001;
	objLandEntry_9[182].slAttribute = 0x80000001;
	objLandEntry_9[185].slAttribute = 0x80000001;
	objLandEntry_9[188].slAttribute = 0x80000001;
	objLandEntry_9[189].slAttribute = 0x80000001;
	objLandEntry_9[190].slAttribute = 0x80000001;
	objLandEntry_9[192].slAttribute = 0x80000001;
	objLandEntry_9[197].slAttribute = 0x80000001;
	objLandEntry_9[200].slAttribute = 0x80000001;
	objLandEntry_9[201].slAttribute = 0x80000001;
	objLandEntry_9[202].slAttribute = 0x80000001;
	objLandEntry_9[203].slAttribute = 0x80000001;
	objLandEntry_9[204].slAttribute = 0x80000001;
	objLandEntry_9[205].slAttribute = 0x80000001;
	objLandEntry_9[215].slAttribute = 0x80000001;
	objLandEntry_9[216].slAttribute = 0x80000001;
	objLandEntry_9[217].slAttribute = 0x80000001;
	objLandEntry_9[218].slAttribute = 0x80000001;
	objLandEntry_9[219].slAttribute = 0x80000001;
	objLandEntry_9[220].slAttribute = 0x80000001;
	objLandEntry_9[223].slAttribute = 0x80000001;
	objLandEntry_9[224].slAttribute = 0x80000001;
	objLandEntry_9[227].slAttribute = 0x80000001;
	objLandEntry_9[229].slAttribute = 0x80000001;
	objLandEntry_9[231].slAttribute = 0x80000001;
	objLandEntry_9[232].slAttribute = 0x80000001;
	objLandEntry_9[234].slAttribute = 0x80000001;
	objLandEntry_9[237].slAttribute = 0x80000001;
	objLandEntry_9[238].slAttribute = 0x80000001;
	objLandEntry_9[241].slAttribute = 0x80000001;
	objLandEntry_9[242].slAttribute = 0x80000001;
	objLandEntry_9[244].slAttribute = 0x80000001;
	objLandEntry_9[248].slAttribute = 0x80000001;
	objLandEntry_9[253].slAttribute = 0x80000001;
	objLandEntry_9[254].slAttribute = 0x80000001;
	objLandEntry_9[255].slAttribute = 0x80000001;
	objLandEntry_9[256].slAttribute = 0x80000001;
	objLandEntry_9[257].slAttribute = 0x80000001;
	objLandEntry_9[258].slAttribute = 0x80000001;
	objLandEntry_9[259].slAttribute = 0x80000001;
	objLandEntry_9[261].slAttribute = 0x80000001;
	objLandEntry_9[262].slAttribute = 0x80000001;
	objLandEntry_9[263].slAttribute = 0x80000001;
	objLandEntry_9[264].slAttribute = 0x80000001;
	objLandEntry_9[265].slAttribute = 0x80000001;
	objLandEntry_9[266].slAttribute = 0x80000001;
	objLandEntry_9[267].slAttribute = 0x80000001;
	objLandEntry_9[268].slAttribute = 0x80000001;
	objLandEntry_9[269].slAttribute = 0x80000001;
	objLandEntry_9[270].slAttribute = 0x80000001;
	objLandEntry_9[271].slAttribute = 0x80000001;
	objLandEntry_9[272].slAttribute = 0x80000001;
	objLandEntry_9[273].slAttribute = 0x80000001;
	objLandEntry_9[274].slAttribute = 0x80000001;
	objLandEntry_9[275].slAttribute = 0x80000001;
	objLandEntry_9[276].slAttribute = 0x80000001;
	objLandEntry_9[277].slAttribute = 0x80000001;
	objLandEntry_9[278].slAttribute = 0x80000001;
	objLandEntry_9[279].slAttribute = 0x80000001;

	// Fixes ground in Red Mountain Act 3.
	objLandEntry_37[123].slAttribute = 0x80000001;
	objLandEntry_37[124].slAttribute = 0x80000001;
	objLandEntry_37[125].slAttribute = 0x80000001;
	objLandEntry_37[126].slAttribute = 0x80000001;
	objLandEntry_37[127].slAttribute = 0x80000001;
	objLandEntry_37[128].slAttribute = 0x80000001;
	objLandEntry_37[129].slAttribute = 0x80000001;
	objLandEntry_37[130].slAttribute = 0x80000001;
	objLandEntry_37[131].slAttribute = 0x80000001;
	objLandEntry_37[132].slAttribute = 0x80000001;
	objLandEntry_37[133].slAttribute = 0x80000001;
	objLandEntry_37[154].slAttribute = 0x80000001;
	objLandEntry_37[158].slAttribute = 0x80000001;
	objLandEntry_37[172].slAttribute = 0x80000001;
	objLandEntry_37[174].slAttribute = 0x80000001;
	objLandEntry_37[175].slAttribute = 0x80000001;
	objLandEntry_37[176].slAttribute = 0x80000001;
	objLandEntry_37[177].slAttribute = 0x80000001;
	objLandEntry_37[178].slAttribute = 0x80000001;
	objLandEntry_37[367].slAttribute = 0x80000001;
}