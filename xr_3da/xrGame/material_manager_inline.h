////////////////////////////////////////////////////////////////////////////
//	Module 		: material_manager_inline.h
//	Created 	: 27.12.2003
//  Modified 	: 27.12.2003
//	Author		: Dmitriy Iassenev
//	Description : Material manager inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

IC	u16	CMaterialManager::last_material_idx		() const
{
	return				(m_last_material_idx);
}

IC	u16	CMaterialManager::self_material_idx		() const
{
	return				(m_my_material_idx);
}
