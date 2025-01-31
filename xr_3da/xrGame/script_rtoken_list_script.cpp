////////////////////////////////////////////////////////////////////////////
//	Module 		: script_rtoken_list_script.cpp
//	Created 	: 21.05.2004
//  Modified 	: 21.05.2004
//	Author		: Dmitriy Iassenev
//	Description : Script rtoken list class export
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "script_rtoken_list.h"
#include "script_space.h"

using namespace luabind;

void CScriptRTokenList::script_register(lua_State *L)
{
	module(L)
	[
		class_<CScriptRTokenList>("rtoken_list")
			.def(					constructor<>())
			.def("add",				&CScriptRTokenList::add)
			.def("remove",			&CScriptRTokenList::remove)
			.def("clear",			&CScriptRTokenList::clear)
			.def("count",			&CScriptRTokenList::size)
			.def("get",				&CScriptRTokenList::get)
	];
}
