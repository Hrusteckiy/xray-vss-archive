////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_biting_misc.cpp
//	Created 	: 26.05.2003
//  Modified 	: 26.05.2003
//	Author		: Serge Zhem
//	Description : Miscellanious functions for all the biting monsters
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ai_biting.h"
#include "../../actor.h"
#include "../../hudmanager.h"

#include "../../ai_script_actions.h"
#include "../ai_monster_jump.h"

// A - � ����� ������� ����
// B - � ����� ��������� ����
// � - � ���� ����� �������� �����
// D - � ���� �������� �����
// E - � ���� ������� �����
// F - � ���� ������� �����
// H - ���� ��������
// I - ���� ����� ����
// J - A | B
// K - C | D | E | F 

////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������� ����������� 
// ������, ����, ����������� ������, ���������� ����������
void CAI_Biting::vfUpdateParameters()
{
	UpdateMemory();

	//------------------------------------
	// ����
	A = B = false;
	
	SoundElem se;
	
	if (IsRememberSound()) {
		GetSound(se,A);
		B = !A;
	}

	J = A | B;

	//------------------------------------
	// ������
	objVisible			&VisibleEnemies = Level().Teams[g_Team()].Squads[g_Squad()].KnownEnemys;

	// ����������, ����� �� ���� ����
	I = false;

	SEnemy ve;

	if (GetEnemy(ve)) {
		VisibleEnemies.insert(ve.obj);

		// ����������, ����� �� ���� ����
		float			yaw1 = 0.f, pitch1 =0.f, yaw2, pitch2, fYawFov = 0.f, fPitchFov = 0.f, fRange = 0.f;
		

		CCustomMonster	*tpCustomMonster = dynamic_cast<CCustomMonster *>(ve.obj);
		if (tpCustomMonster) {
			yaw1		= -tpCustomMonster->m_head.current.yaw;
			pitch1		= -tpCustomMonster->m_head.current.pitch;
			fYawFov		= angle_normalize_signed(tpCustomMonster->ffGetFov()*PI/180.f);
			fRange		= tpCustomMonster->ffGetRange();
		}
		else {
			CActor		*tpActor = dynamic_cast<CActor *>(ve.obj);
			if (tpActor) {
				yaw1	= tpActor->Orientation().yaw;
				pitch1	= tpActor->Orientation().pitch;
				fYawFov	= angle_normalize_signed(tpActor->ffGetFov()*PI/180.f);
				fRange	= tpActor->ffGetRange();
			}
		}

		if (ve.position.distance_to(Position()) <= fRange) {
			
			fYawFov			= angle_normalize_signed((_abs(fYawFov) + _abs(atanf(1.f/ve.position.distance_to(Position()))))/2.f);
			fPitchFov		= angle_normalize_signed(fYawFov*1.f);
			ve.position.sub	(Position());
			ve.position.mul	(-1);
			ve.position.getHP	(yaw2,pitch2);
			yaw1			= angle_normalize_signed(yaw1);
			pitch1			= angle_normalize_signed(pitch1);
			yaw2			= angle_normalize_signed(yaw2);
			pitch2			= angle_normalize_signed(pitch2);
			
			I = (angle_difference(yaw1,yaw2) <= fYawFov) && (angle_difference(pitch1,pitch2) <= fPitchFov);
		}
	}

	//------------------------------------
	// ����������� ������
	C = D = E = F = G	= false;
	
	if (bfIsAnyAlive(VisibleEnemies) && ve.obj) {
		switch (dwfChooseAction(0,m_fAttackSuccessProbability[0],m_fAttackSuccessProbability[1],m_fAttackSuccessProbability[2],m_fAttackSuccessProbability[3],g_Team(),g_Squad(),g_Group(),0,1,2,3,4,this,30.f)) {
			case 4 : 
				C = true;
				break;
			case 3 : 
				D = true;
				break;
			case 2 : 
				E = true;
				break;
			case 1 : 
			case 0 : 
				F = true;
				break;
		}
	}
	K					= C | D | E | F;

	// K ������ ���� true, ������ ���� ���������� ve.obj
	R_ASSERT(ve.obj || !K);

	//------------------------------------
	// ���� ������� ?
	H = true;

	// Fill flags, properties and vars for attack mode
	flagsEnemy	= 0;
	
#pragma todo("enemy hiding & enemy runaway flags")

	// Set current enemy
	m_tEnemy					= ve;
	
	if (m_tEnemy.obj && (m_tEnemyPrevFrame.obj == m_tEnemy.obj) && (m_tEnemy.time != m_current_update)) {
		flagsEnemy |= FLAG_ENEMY_LOST_SIGHT;		// ���� ������� �� ����
	}
	
	if (m_tEnemyPrevFrame.obj && !m_tEnemyPrevFrame.obj->g_Alive()) {
		flagsEnemy |= FLAG_ENEMY_DIE;
	}
	
	float dist_now, dist_prev;
	if (m_tEnemy.obj && (m_tEnemyPrevFrame.obj == m_tEnemy.obj)) {
		dist_now	= m_tEnemy.position.distance_to(Position());
		dist_prev	= m_tEnemyPrevFrame.position.distance_to(Position());
		
		if (_abs(dist_now - dist_prev) < 0.2f) flagsEnemy |= FLAG_ENEMY_STANDING;
		else {
			if (dist_now < dist_prev) flagsEnemy |= FLAG_ENEMY_GO_CLOSER;
			else flagsEnemy |= FLAG_ENEMY_GO_FARTHER;

			if (_abs(dist_now - dist_prev) < 1.2f) {
				if (dist_now < dist_prev)  flagsEnemy |= FLAG_ENEMY_GO_CLOSER_FAST;
				else flagsEnemy |= FLAG_ENEMY_GO_FARTHER_FAST;
			}
		}

		if (((flagsEnemy & FLAG_ENEMY_STANDING) == FLAG_ENEMY_STANDING)  && !I) flagsEnemy |= FLAG_ENEMY_DOESNT_KNOW_ABOUT_ME;
	}
	
	if (m_tEnemy.obj && !I) flagsEnemy |= FLAG_ENEMY_DOESNT_SEE_ME;

	if (!m_tEnemy.obj && m_tEnemyPrevFrame.obj && m_tEnemyPrevFrame.obj->getDestroy()) flagsEnemy |= FLAG_ENEMY_GO_OFFLINE;

	// Save current enemy (only if valid)
	if (m_tEnemy.obj)
		m_tEnemyPrevFrame = m_tEnemy;

	// update standing
	cur_pos			= Position();

	bStanding		= !!prev_pos.similar(cur_pos);
	if (bStanding && (0 == time_start_stand)) time_start_stand = m_dwCurrentTime;		// ������ �������� ������ �� �����
	if (!bStanding) time_start_stand = 0; 

	prev_pos	= cur_pos;

	// Setup is own additional flags
	m_bDamaged = ((GetHealth() < _sd->m_fDamagedThreshold) ? true : false);
}

bool CAI_Biting::IsStanding (TTime time)
{
	return (bStanding && (time_start_stand + time < m_dwCurrentTime));
}

////////////////////////////////////////////////////////////////////////////////////////////
// ��������� ��������
////////////////////////////////////////////////////////////////////////////////////////////

bool CAI_Biting::bfAssignMovement (CEntityAction *tpEntityAction)
{
	if (!inherited::bfAssignMovement(tpEntityAction))
		return		(false);

	LOG_EX("Scripting now!!!");
	
	CMovementAction	&l_tMovementAction	= tpEntityAction->m_tMovementAction;
	
	MotionMan.m_tAction = EAction(l_tMovementAction.m_tActState);

	bool bEnablePath = true;
	u32 vel_mask = 0;
	u32 des_mask = 0;

	switch (MotionMan.m_tAction) {
	case ACT_STAND_IDLE: 
	case ACT_SIT_IDLE:	 
	case ACT_LIE_IDLE:
	case ACT_EAT:
	case ACT_SLEEP:
	case ACT_REST:
	case ACT_LOOK_AROUND:
	case ACT_ATTACK:
		bEnablePath = false;
		break;
	
	case ACT_WALK_FWD:
		vel_mask = eVelocityParamsWalk;
		des_mask = eVelocityParameterWalkNormal;
		break;
	case ACT_WALK_BKWD:
		break;
	case ACT_RUN:
		vel_mask = eVelocityParamsRun;
		des_mask = eVelocityParameterRunNormal;
		break;
	case ACT_DRAG:
		vel_mask = eVelocityParameterDrag;
		des_mask = eVelocityParameterDrag;
		break;
	case ACT_STEAL:
		vel_mask = eVelocityParameterSteal;
		des_mask = eVelocityParameterSteal;
		break;
	case ACT_JUMP:
		break;
	}

	if (bEnablePath) {
		CDetailPathManager::set_velocity_mask(vel_mask);
		CDetailPathManager::set_desirable_mask(des_mask);

		CDetailPathManager::set_path_type(eDetailPathTypeSmooth);
		CDetailPathManager::set_try_min_time(true);
	} else {
		enable_movement(false);
	}

	update_path				();

	PreprocessAction();
	MotionMan.ProcessAction();

	SetVelocity();

#pragma todo("Dima to Jim : This method will be automatically removed after 22.12.2003 00:00")
	set_desirable_speed		(m_fCurSpeed);

	m_head = m_body;

	return			(true);		
}

bool CAI_Biting::bfAssignObject(CEntityAction *tpEntityAction)
{
	if (!inherited::bfAssignObject(tpEntityAction))
		return	(false);

	CObjectAction	&l_tObjectAction = tpEntityAction->m_tObjectAction;
	if (!l_tObjectAction.m_tpObject)
		return	(false == (l_tObjectAction.m_bCompleted = true));

	CEntityAlive	*l_tpEntity		= dynamic_cast<CEntityAlive*>(l_tObjectAction.m_tpObject);
	if (!l_tpEntity) return	(false == (l_tObjectAction.m_bCompleted = true));
	
	switch (l_tObjectAction.m_tGoalType) {
		case eObjectActionTake: 
			m_PhysicMovementControl.PHCaptureObject(l_tpEntity);
			break;
		case eObjectActionDrop: 
			m_PhysicMovementControl.PHReleaseObject();
			break;
	}
	l_tObjectAction.m_bCompleted = true;
	return	(true);
}



