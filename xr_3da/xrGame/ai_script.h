////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_script.h
//	Created 	: 19.09.2003
//  Modified 	: 19.09.2003
//	Author		: Dmitriy Iassenev
//	Description : XRay Scripting system with Lua as a scripting language usage
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "script_space.h"

class CScript {
	CLuaVirtualMachine	*m_tpLuaThread;
	LPSTR				m_script_name;
public:
	bool				m_bActive;

						CScript						(LPCSTR caNamespaceName);
	virtual				~CScript					();
			bool		Update						();
};