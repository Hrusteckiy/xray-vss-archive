#pragma once

#include "../../../level.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBurerAttackTeleAbstract CStateBurerAttackTele<_Object>


#define GOOD_DISTANCE_FOR_TELE	15.f
#define TELE_DELAY				4000

#define MAX_TIME_CHECK_FAILURE	6000

TEMPLATE_SPECIALIZATION
CStateBurerAttackTeleAbstract::CStateBurerAttackTele(_Object *obj) : inherited(obj)
{
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::initialize()
{
	inherited::initialize		();

	m_action					= ACTION_TELE_STARTED;
	selected_object				= 0;

	SelectObjects				();

	time_started				= 0;

	// ��������� ������ ��������
	object->set_script_capture	(false);

}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::execute()
{
	switch (m_action) {
		/************************/
		case ACTION_TELE_STARTED:
			/************************/

			ExecuteTeleStart();
			m_action = ACTION_TELE_CONTINUE;

			break;
			/************************/
		case ACTION_TELE_CONTINUE:
			/************************/

			ExecuteTeleContinue();

			break;

			/************************/
		case ACTION_TELE_FIRE:
			/************************/

			ExecuteTeleFire();
			m_action = ACTION_WAIT_TRIPLE_END;

			break;
			/***************************/
		case ACTION_WAIT_TRIPLE_END:
			/***************************/

			if (!object->MotionMan.TA_IsActive()) {
				if (IsActiveObjects())
					m_action = ACTION_TELE_STARTED;
				else 
					m_action = ACTION_COMPLETED; 
			}

			/*********************/
		case ACTION_COMPLETED:
			/*********************/

			break;
	}

	object->MotionMan.m_tAction		= ACT_STAND_IDLE;
	object->DirMan.face_target		(object->EnemyMan.get_enemy(), 700);

}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::finalize()
{
	inherited::finalize				();

	tele_objects.clear				();
	object->DeactivateShield		();

	// clear particles on active objects
	if (object->CTelekinesis::is_active()) {
		for (u32 i=0; i<object->CTelekinesis::get_objects_count(); i++) {
			object->StopTeleObjectParticle(object->CTelekinesis::get_object_by_index(i).get_object());
		}
	}

	// �������� ������ �� ������ ��������
	object->set_script_capture			(true);
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::critical_finalize()
{
	inherited::critical_finalize		();

	object->MotionMan.TA_Deactivate		();
	object->CTelekinesis::Deactivate	();
	object->DeactivateShield			();

	tele_objects.clear					();

	// clear particles on active objects
	if (object->CTelekinesis::is_active()) {
		for (u32 i=0; i<object->CTelekinesis::get_objects_count(); i++) {
			object->StopTeleObjectParticle(object->CTelekinesis::get_object_by_index(i).get_object());
		}
	}
	
	// �������� ������ �� ������ ��������
	object->set_script_capture			(true);
}

TEMPLATE_SPECIALIZATION
bool CStateBurerAttackTeleAbstract::check_start_conditions()
{
	return CheckTeleStart();
}

TEMPLATE_SPECIALIZATION
bool CStateBurerAttackTeleAbstract::check_completion()
{
	return (m_action == ACTION_COMPLETED);
}

//////////////////////////////////////////////////////////////////////////

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::FindObjects()
{
	tele_objects.clear();

	// �������� ������ �������� ������ �����
	Level().ObjectSpace.GetNearest	(object->EnemyMan.get_enemy()->Position(), object->m_tele_find_radius);
	xr_vector<CObject*> &tpObjects	= Level().ObjectSpace.q_nearest;

	for (u32 i=0;i<tpObjects.size();i++) {
		CPhysicsShellHolder *obj = smart_cast<CPhysicsShellHolder *>(tpObjects[i]);
		if (!obj || !obj->PPhysicsShell() || !obj->PPhysicsShell()->bActive || (obj->m_pPhysicsShell->getMass() < object->m_tele_object_min_mass) || (obj->m_pPhysicsShell->getMass() > object->m_tele_object_max_mass) || (obj == object) || object->CTelekinesis::is_active_object(obj)) continue;

		tele_objects.push_back(obj);
	}

	
	// �������� ������ �������� ������ �������
	Level().ObjectSpace.GetNearest	(object->Position(), object->m_tele_find_radius);
	tpObjects = Level().ObjectSpace.q_nearest;

	for (u32 i=0;i<tpObjects.size();i++) {
		CPhysicsShellHolder *obj = smart_cast<CPhysicsShellHolder *>(tpObjects[i]);
		if (!obj || !obj->PPhysicsShell() || !obj->PPhysicsShell()->bActive || (obj->m_pPhysicsShell->getMass() < object->m_tele_object_min_mass) || (obj->m_pPhysicsShell->getMass() > object->m_tele_object_max_mass) || (obj == object) || object->CTelekinesis::is_active_object(obj)) continue;

		tele_objects.push_back(obj);
	}


	// �������� ������ �������� ����� �������� � ������
	float dist = object->EnemyMan.get_enemy()->Position().distance_to(object->Position());
	Fvector dir;
	dir.sub(object->EnemyMan.get_enemy()->Position(), object->Position());
	dir.normalize();

	Fvector pos;
	pos.mad(object->Position(), dir, dist / 2.f);
	Level().ObjectSpace.GetNearest(pos, object->m_tele_find_radius); 
	tpObjects = Level().ObjectSpace.q_nearest;

	for (u32 i=0;i<tpObjects.size();i++) {
		CPhysicsShellHolder *obj = smart_cast<CPhysicsShellHolder *>(tpObjects[i]);
		if (!obj || !obj->m_pPhysicsShell || !obj->m_pPhysicsShell->bActive || (obj->m_pPhysicsShell->getMass() < object->m_tele_object_min_mass) || (obj->m_pPhysicsShell->getMass() > object->m_tele_object_max_mass) || (obj == object) || object->CTelekinesis::is_active_object(obj)) continue;

		tele_objects.push_back(obj);
	}

	// �������� ���������� �������
	xr_vector<CPhysicsShellHolder*>::iterator I = unique(tele_objects.begin(),tele_objects.end());
	tele_objects.erase(I,tele_objects.end());
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::ExecuteTeleStart()
{
	object->MotionMan.TA_Activate(&object->anim_triple_tele);
	time_started = Device.dwTimeGlobal;
	object->ActivateShield();

}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::ExecuteTeleContinue()
{
	if (time_started + object->m_tele_time_to_hold > Device.dwTimeGlobal) return;

	// ����� ������ ��� �����
	bool object_found = false;
	CTelekineticObject tele_object;

	u32 i=0;
	while (i < object->CTelekinesis::get_objects_count()) {
		tele_object = object->CTelekinesis::get_object_by_index(i);

		if ((tele_object.get_state() == TS_Keep) && (tele_object.time_keep_started + 1500 < Device.dwTimeGlobal)) {

			object_found = true;
			break;

		} else i++;

	}

	if (object_found) {
		m_action		= ACTION_TELE_FIRE;
		selected_object = tele_object.get_object();
	} else {
		if (!IsActiveObjects() || (time_started + MAX_TIME_CHECK_FAILURE < Device.dwTimeGlobal)) {
			object->MotionMan.TA_Deactivate	();
			m_action						= ACTION_COMPLETED;
		} 
	}
}

TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::ExecuteTeleFire()
{
	object->MotionMan.TA_PointBreak();

	Fvector enemy_pos = object->EnemyMan.get_enemy()->Position();
	enemy_pos.y += 5 * object->EnemyMan.get_enemy()->Radius();

	float dist = selected_object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
	object->CTelekinesis::fire(selected_object, enemy_pos, dist/12);

	object->StopTeleObjectParticle(selected_object);

	object->sound().play(MonsterSpace::eMonsterSoundTeleAttack);
	object->DeactivateShield();
}

TEMPLATE_SPECIALIZATION
bool CStateBurerAttackTeleAbstract::IsActiveObjects()
{
	return (object->CTelekinesis::get_objects_count() > 0);
}

TEMPLATE_SPECIALIZATION
bool CStateBurerAttackTeleAbstract::CheckTeleStart()
{
	// �������� �� ������� ���������� 
	if (IsActiveObjects()) return false;

	// ��������� ��������� �� �����
	float dist = object->Position().distance_to(object->EnemyMan.get_enemy()->Position());
	if (dist < GOOD_DISTANCE_FOR_TELE) return false;

	// ����� ���������������� �������
	FindObjects();

	// ���� ��� ��������
	if (tele_objects.empty()) return false;

	// �� �� ����� �������� ���������
	return true;

}

//////////////////////////////////////////////////////////////////////////
// ����� ���������� �������� ��� ����������
//////////////////////////////////////////////////////////////////////////
class best_object_predicate {
	Fvector enemy_pos;
	Fvector monster_pos;
public:
	best_object_predicate(const Fvector &m_pos, const Fvector &pos) {
		monster_pos = m_pos;
		enemy_pos	= pos;
	}

	bool operator()	 (const CGameObject *tpObject1, const CGameObject *tpObject2) const
	{

		float dist1 = monster_pos.distance_to(tpObject1->Position());
		float dist2 = enemy_pos.distance_to(tpObject2->Position());
		float dist3 = enemy_pos.distance_to(monster_pos);

		return ((dist1 < dist3) && (dist2 > dist3));
	};
};

class best_object_predicate2 {
	Fvector enemy_pos;
	Fvector monster_pos;
public:
	best_object_predicate2(const Fvector &m_pos, const Fvector &pos) {
		monster_pos = m_pos;
		enemy_pos	= pos;
	}

	bool operator()	 (const CGameObject *tpObject1, const CGameObject *tpObject2) const
	{
		float dist1 = enemy_pos.distance_to(tpObject1->Position());
		float dist2 = enemy_pos.distance_to(tpObject2->Position());

		return (dist1 < dist2);		
	};
};


TEMPLATE_SPECIALIZATION
void CStateBurerAttackTeleAbstract::SelectObjects()
{
	sort(tele_objects.begin(),tele_objects.end(),best_object_predicate2(object->Position(), object->EnemyMan.get_enemy()->Position()));

	// ������� ������
	for (u32 i=0; i<tele_objects.size(); i++) {
		CPhysicsShellHolder *obj = tele_objects[i];

		// ��������� ��������� �� ������
		object->CTelekinesis::activate(obj, 3.f, 2.f, 10000);
		object->StartTeleObjectParticle(obj);

		// ������� �� ������
		tele_objects[i] = tele_objects[tele_objects.size()-1];
		tele_objects.pop_back();

		if (object->CTelekinesis::get_objects_count() >= object->m_tele_max_handled_objects) break;
	}
}


#undef TEMPLATE_SPECIALIZATION
#undef CStateBurerAttackTeleAbstract
