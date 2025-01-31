#include "stdafx.h"
#include "torridZone.h"
#include "../objectanimator.h"
#include "xrServer_Objects_ALife_Monsters.h"

CTorridZone::CTorridZone()
{
	m_animator			= xr_new<CObjectAnimator>();
}

CTorridZone::~CTorridZone()
{
	xr_delete			(m_animator);
}

BOOL CTorridZone::net_Spawn(CSE_Abstract* DC)
{
	if (!inherited::net_Spawn(DC))
		return			(FALSE);

	CSE_Abstract		*abstract=(CSE_Abstract*)(DC);
	CSE_ALifeTorridZone	*zone	= smart_cast<CSE_ALifeTorridZone*>(abstract);
	VERIFY				(zone);

	m_animator->Load	(zone->get_motion());
	m_animator->Play	(true);

	return				(TRUE);
}

void CTorridZone::UpdateWorkload(u32 dt)
{
	inherited::UpdateWorkload	(dt);
	m_animator->OnFrame			();
	XFORM().set					(m_animator->XFORM());
	OnMove						();
}

void CTorridZone::shedule_Update(u32 dt)
{
	inherited::shedule_Update(dt);

	if(m_idle_sound.feedback)		m_idle_sound.set_position		(XFORM().c);
	if(m_blowout_sound.feedback)	m_blowout_sound.set_position	(XFORM().c);
	if(m_hit_sound.feedback)		m_hit_sound.set_position		(XFORM().c);
	if(m_entrance_sound.feedback)	m_entrance_sound.set_position	(XFORM().c);
}
