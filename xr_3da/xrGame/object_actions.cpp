////////////////////////////////////////////////////////////////////////////
//	Module 		: object_actions.h
//	Created 	: 12.03.2004
//  Modified 	: 26.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Object actions
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "object_actions.h"
#include "inventory.h"
#include "ai/stalker/ai_stalker.h"
#include "xr_level_controller.h"
#include "xrmessages.h"
#include "fooditem.h"
#include "weapon.h"
#include "weaponmagazined.h"
#include "object_handler_space.h"
#include "stalker_animation_manager.h"

//////////////////////////////////////////////////////////////////////////
// CObjectActionCommand
//////////////////////////////////////////////////////////////////////////

CObjectActionCommand::CObjectActionCommand(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, u32 command, LPCSTR action_name) :
	inherited			(item,owner,storage,action_name),
	m_command			(command)
{
}

void CObjectActionCommand::initialize	()
{
	inherited::initialize();
	object().inventory().Action(m_command,CMD_START);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionShow
//////////////////////////////////////////////////////////////////////////

CObjectActionShow::CObjectActionShow	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name)
{
}

void CObjectActionShow::initialize		()
{
	inherited::initialize			();
	VERIFY							(m_item);
	if (object().inventory().m_slots[m_item->GetSlot()].m_pIItem)
		object().inventory().Ruck	(object().inventory().m_slots[m_item->GetSlot()].m_pIItem);
	object().inventory().SetActiveSlot(NO_ACTIVE_SLOT);
	object().inventory().Slot		(m_item);
	bool							result = object().inventory().Activate	(m_item->GetSlot());
	VERIFY							(result);
}

void CObjectActionShow::execute		()
{
	inherited::execute				();
	VERIFY							(m_item);
	if (!object().inventory().ActiveItem() || (object().inventory().ActiveItem()->object().ID() != m_item->object().ID())) {
		CHudItem					*hud_item = smart_cast<CHudItem*>(object().inventory().ActiveItem());
		if (!hud_item)
			return;
		if (!hud_item->IsPending()) {
			if (object().inventory().m_slots[m_item->GetSlot()].m_pIItem)
				object().inventory().Ruck	(object().inventory().m_slots[m_item->GetSlot()].m_pIItem);
			object().inventory().SetActiveSlot(NO_ACTIVE_SLOT);
			object().inventory().Slot		(m_item);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionHide
//////////////////////////////////////////////////////////////////////////

CObjectActionHide::CObjectActionHide	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name)
{
}

void CObjectActionHide::execute		()
{
	inherited::execute				();
	VERIFY							(m_item);
	object().inventory().Activate	(NO_ACTIVE_SLOT);
	set_property					(ObjectHandlerSpace::eWorldPropertyUseEnough,false);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionReload
//////////////////////////////////////////////////////////////////////////

CObjectActionReload::CObjectActionReload	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, _condition_type type, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name),
	m_type			(type)
{
}

void CObjectActionReload::initialize		()
{
	inherited::initialize		();
	object().inventory().Action(kWPN_RELOAD,	CMD_START);
}

void CObjectActionReload::execute			()
{
	inherited::execute();
	CWeapon			*weapon = smart_cast<CWeapon*>(object().inventory().ActiveItem());
	VERIFY			(weapon);
	if (!weapon->IsPending() && !weapon->GetAmmoElapsed())
		object().inventory().Action(kWPN_RELOAD,	CMD_START);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionFire
//////////////////////////////////////////////////////////////////////////

CObjectActionFire::CObjectActionFire	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, _condition_type type, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name),
	m_type			(type)
{
}

void CObjectActionFire::initialize		()
{
	inherited::inherited::initialize	();
	if (!m_object->can_kill_member())
		object().inventory().Action	(kWPN_FIRE,	CMD_START);
	else
		object().inventory().Action	(kWPN_FIRE,	CMD_STOP);
}

void CObjectActionFire::execute			()
{
	inherited::execute					();
	if (!m_object->can_kill_member())
		object().inventory().Action	(kWPN_FIRE,	CMD_START);
	else
		object().inventory().Action	(kWPN_FIRE,	CMD_STOP);
}

void CObjectActionFire::finalize		()
{
	inherited::finalize					();
	object().inventory().Action		(kWPN_FIRE,	CMD_STOP);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionStrapping
//////////////////////////////////////////////////////////////////////////

CObjectActionStrapping::CObjectActionStrapping	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name)
{
}

void CObjectActionStrapping::initialize		()
{
	inherited::initialize				();
	m_storage->set_property(ObjectHandlerSpace::eWorldPropertyStrapped2Idle,true);
	object().animation().setup_storage(m_storage);
	object().animation().property_id(ObjectHandlerSpace::eWorldPropertyStrapped);
	object().animation().property_value(true);
}

void CObjectActionStrapping::execute			()
{
	inherited::execute();
}

void CObjectActionStrapping::finalize		()
{
	inherited::finalize();
	object().animation().setup_storage(0);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionStrappingToIdle
//////////////////////////////////////////////////////////////////////////

CObjectActionStrappingToIdle::CObjectActionStrappingToIdle	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name)
{
}

void CObjectActionStrappingToIdle::initialize		()
{
	inherited::initialize				();
	object().animation().setup_storage(m_storage);
	object().animation().property_id(ObjectHandlerSpace::eWorldPropertyStrapped2Idle);
	object().animation().property_value(false);
}

void CObjectActionStrappingToIdle::execute			()
{
	inherited::execute();
}

void CObjectActionStrappingToIdle::finalize		()
{
	inherited::finalize();
	object().animation().setup_storage(0);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionUnstrapping
//////////////////////////////////////////////////////////////////////////

CObjectActionUnstrapping::CObjectActionUnstrapping	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name)
{
}

void CObjectActionUnstrapping::initialize		()
{
	inherited::initialize();
	m_storage->set_property(ObjectHandlerSpace::eWorldPropertyStrapped2Idle,true);
	object().animation().setup_storage(m_storage);
	object().animation().property_id(ObjectHandlerSpace::eWorldPropertyStrapped);
	object().animation().property_value(false);
}

void CObjectActionUnstrapping::execute			()
{
	inherited::execute();
}

void CObjectActionUnstrapping::finalize		()
{
	inherited::finalize();
	object().animation().setup_storage(0);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionUnstrappingToIdle
//////////////////////////////////////////////////////////////////////////

CObjectActionUnstrappingToIdle::CObjectActionUnstrappingToIdle	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name)
{
}

void CObjectActionUnstrappingToIdle::initialize		()
{
	inherited::initialize				();
	object().animation().setup_storage(m_storage);
	object().animation().property_id(ObjectHandlerSpace::eWorldPropertyStrapped2Idle);
	object().animation().property_value(false);
}

void CObjectActionUnstrappingToIdle::execute			()
{
	inherited::execute();
}

void CObjectActionUnstrappingToIdle::finalize		()
{
	inherited::finalize();
	object().animation().setup_storage(0);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionQueueWait
//////////////////////////////////////////////////////////////////////////

CObjectActionQueueWait::CObjectActionQueueWait	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, _condition_type type, LPCSTR action_name) :
	inherited				(item,owner,storage,action_name),
	m_type					(type)
{
	m_magazined		= smart_cast<CWeaponMagazined*>(item);
}

void CObjectActionQueueWait::initialize		()
{
	inherited::inherited::initialize	();
}

void CObjectActionQueueWait::execute			()
{
	inherited::execute		();
	if (completed())
		m_magazined->StopedAfterQueueFired(false);
}

void CObjectActionQueueWait::finalize		()
{
	inherited::finalize		();
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionSwitch
//////////////////////////////////////////////////////////////////////////

CObjectActionSwitch::CObjectActionSwitch	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, _condition_type type, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name),
	m_type			(type)
{
}

void CObjectActionSwitch::initialize		()
{
	inherited::initialize	();
}

void CObjectActionSwitch::execute			()
{
	inherited::execute		();
}

void CObjectActionSwitch::finalize		()
{
	inherited::finalize		();
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionDrop
//////////////////////////////////////////////////////////////////////////

CObjectActionDrop::CObjectActionDrop	(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited		(item,owner,storage,action_name)
{
}

void CObjectActionDrop::initialize		()
{
	inherited::initialize	();
	if (!m_item || !m_item->object().H_Parent() || (m_object->ID() != m_item->object().H_Parent()->ID()))
		return;

	NET_Packet				P;
	m_object->u_EventGen	(P,GE_OWNERSHIP_REJECT,m_object->ID());
	P.w_u16					(u16(m_item->object().ID()));
	m_object->u_EventSend	(P);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionThreaten
//////////////////////////////////////////////////////////////////////////

CObjectActionThreaten::CObjectActionThreaten	(CAI_Stalker *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited				(item,owner,storage,action_name)
{
}

void CObjectActionThreaten::execute			()
{
	inherited::execute		();
	if (completed())
		object().inventory().Action(kWPN_FIRE,	CMD_STOP);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionAim
//////////////////////////////////////////////////////////////////////////

CObjectActionAim::CObjectActionAim			(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, _condition_type condition_id, _value_type value, LPCSTR action_name) :
	inherited							(item,owner,storage,condition_id,value,action_name)
{
}

void CObjectActionAim::initialize			()
{
	inherited::inherited::initialize	();
}

void CObjectActionAim::execute				()
{
	inherited::execute					();
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionPrepare
//////////////////////////////////////////////////////////////////////////

CObjectActionPrepare::CObjectActionPrepare(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited			(item,owner,storage,action_name)
{
}

void CObjectActionPrepare::execute	()
{
	inherited::execute();
	object().inventory().Action(kWPN_FIRE,CMD_START);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionUse
//////////////////////////////////////////////////////////////////////////

CObjectActionUse::CObjectActionUse(CFoodItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited			(item,owner,storage,action_name)
{
}

void CObjectActionUse::execute	()
{
	inherited::execute();
	object().inventory().Action(kWPN_FIRE,CMD_START);
	if (m_item->State() != FOOD_EATING)
		m_storage->set_property(ObjectHandlerSpace::eWorldPropertyUseEnough,true);
}

//////////////////////////////////////////////////////////////////////////
// CObjectActionIdle
//////////////////////////////////////////////////////////////////////////

CObjectActionIdle::CObjectActionIdle(CInventoryItem *item, CAI_Stalker *owner, CPropertyStorage *storage, LPCSTR action_name) :
	inherited			(item,owner,storage,action_name)
{
}

void CObjectActionIdle::initialize	()
{
	inherited::initialize();
	if (m_storage->property(ObjectHandlerSpace::eWorldPropertyUseEnough))
		object().CObjectHandler::set_goal(MonsterSpace::eObjectActionActivate,object().inventory().ActiveItem());
	m_storage->set_property	(ObjectHandlerSpace::eWorldPropertyUseEnough,false);
}
