////////////////////////////////////////////////////////////////////////////
//	Module 		: item_manager.h
//	Created 	: 27.12.2003
//  Modified 	: 27.12.2003
//	Author		: Dmitriy Iassenev
//	Description : Item manager
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "object_manager.h"
#include "gameobject.h"

class CCustomMonster;

class CItemManager : public CObjectManager<const CGameObject > {
public:
	typedef CObjectManager<const CGameObject >	inherited;
	typedef OBJECTS								ITEMS;

private:
	CCustomMonster		*m_object;

public:
	IC					CItemManager	(CCustomMonster *object);
	virtual bool		useful			(const CGameObject *object) const;
	virtual bool		is_useful		(const CGameObject *object) const;
	virtual	float		evaluate		(const CGameObject *object) const;
	virtual	float		do_evaluate		(const CGameObject *object) const;
	virtual void		update			();
};

#include "item_manager_inline.h"