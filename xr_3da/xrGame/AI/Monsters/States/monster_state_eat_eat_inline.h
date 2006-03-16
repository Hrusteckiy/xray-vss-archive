#pragma once
#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterEatingAbstract CStateMonsterEating<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterEatingAbstract::CStateMonsterEating(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
CStateMonsterEatingAbstract::~CStateMonsterEating()
{
}

TEMPLATE_SPECIALIZATION
void CStateMonsterEatingAbstract::initialize()
{
	inherited::initialize();
	time_last_eat = 0;
}

TEMPLATE_SPECIALIZATION
void CStateMonsterEatingAbstract::execute()
{
	object->set_action				(ACT_EAT);
	object->set_state_sound			(MonsterSound::eMonsterSoundEat);

	// ������ �����
	if (time_last_eat + u32(1000/object->db().m_fEatFreq) < Device.dwTimeGlobal) {
		object->conditions().ChangeSatiety(object->db().m_fEatSlice);
		corpse->m_fFood -= object->db().m_fEatSliceWeight;
		time_last_eat = Device.dwTimeGlobal;
	}
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterEatingAbstract::check_start_conditions()
{
	corpse	= const_cast<CEntityAlive*>(object->CorpseMan.get_corpse());
	VERIFY	(corpse);

	Fvector nearest_bone_pos;
	if ((corpse->m_pPhysicsShell == NULL) || (!corpse->m_pPhysicsShell->isActive())) {
		nearest_bone_pos	= corpse->Position(); 
	} else nearest_bone_pos = object->m_PhysicMovementControl->PHCaptureGetNearestElemPos(corpse);

	float dist				= nearest_bone_pos.distance_to(object->Position());
	float dist_to_corpse	= object->db().m_fDistToCorpse; 
	
	if (dist + 1.f < dist_to_corpse) return true;
	return false;
}


TEMPLATE_SPECIALIZATION
bool CStateMonsterEatingAbstract::check_completion()
{
	if (object->conditions().GetSatiety() > 0.95f) return true;
	
	Fvector nearest_bone_pos;
	if ((corpse->m_pPhysicsShell == NULL) || (!corpse->m_pPhysicsShell->isActive())) {
		nearest_bone_pos	= corpse->Position(); 
	} else nearest_bone_pos = object->m_PhysicMovementControl->PHCaptureGetNearestElemPos(corpse);

	float dist				= nearest_bone_pos.distance_to(object->Position());
	float dist_to_corpse	= object->db().m_fDistToCorpse; 
	if (dist > dist_to_corpse+0.5f) return true;

	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterEatingAbstract
