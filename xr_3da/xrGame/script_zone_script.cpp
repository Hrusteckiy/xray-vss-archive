////////////////////////////////////////////////////////////////////////////
//	Module 		: script_zone_script.cpp
//	Created 	: 10.10.2003
//  Modified 	: 11.10.2004
//	Author		: Dmitriy Iassenev
//	Description : Script zone object script export
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "script_zone.h"
#include "script_space.h"

using namespace luabind;

void CScriptZone::script_register(lua_State *L)
{
	module(L)
	[
		class_<CScriptZone,DLL_Pure>("ce_script_zone")
			.def(constructor<>())
	];
}
