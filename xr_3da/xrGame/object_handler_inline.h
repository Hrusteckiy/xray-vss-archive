////////////////////////////////////////////////////////////////////////////
//	Module 		: object_handler_inline.h
//	Created 	: 11.03.2004
//  Modified 	: 11.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Object handler inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

IC	bool CObjectHandler::goal_reached			() const
{
	return				(solution().size() < 2);
}

IC	bool CObjectHandler::hammer_is_clutched		() const
{
	return				(m_hammer_is_clutched);
}
