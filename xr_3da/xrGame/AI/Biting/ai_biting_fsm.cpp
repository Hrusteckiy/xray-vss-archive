////////////////////////////////////////////////////////////////////////////
//	Module 		: ai_biting_fsm.cpp
//	Created 	: 26.05.2003
//  Modified 	: 26.05.2003
//	Author		: Serge Zhem
//	Description : FSM for biting class
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ai_biting.h"

void CAI_Biting::Think()
{
	if (!g_Alive()) return;


	m_dwLastUpdateTime		= m_current_update;
	m_current_update		= Level().timeServer();

	vfUpdateParameters		();

	// pre-update path parameters
	enable_movement(true);
	CLevelLocationSelector::set_evaluator(0);

	// fix off-line displacement
	if ((flagsEnemy & FLAG_ENEMY_GO_OFFLINE) == FLAG_ENEMY_GO_OFFLINE) {
		CurrentState->Reset();
		SetState(stateRest);
	}

	StateSelector			();
	CurrentState->Execute	(m_current_update);

	// update path
	CDetailPathManager::set_path_type(eDetailPathTypeSmooth);
	CDetailPathManager::set_try_min_time(true);

//	CTimer T;
//	T.Start();
	
	update_path				();

//	if (T.GetElapsed_sec() > 0.1f)  {
//		// Time checker	
//		LOG_EX("------------- PATH BUILDER OVERTIME -------------");
//		LOG_EX2("Time: %f ", *"*/ T.GetElapsed_sec() /*"*);
//		LOG_EX2("Start Node ID = [%u] Start Position = [%f,%f] ", *"*/ CDetailPathManager::path()[0].vertex_id,  CDetailPathManager::path()[0].position.x, CDetailPathManager::path()[0].position.y /*"*);
//		
//		u32 last_index = CDetailPathManager::path().size() - 1;
//		LOG_EX2("Finish Node ID = [%u] Finish Position = [%f,%f] ", *"*/ CDetailPathManager::path()[last_index].vertex_id,  CDetailPathManager::path()[last_index].position.x, CDetailPathManager::path()[last_index].position.y /*"*);	
//		
//		LOG_EX("Velocity Mask: ");
//
//		xr_map<u32,STravelParams>::iterator it;
//		for (it = m_movement_params.begin(); it != m_movement_params.end(); it++) {
//			LOG_EX2("Mask: [linear = %f, angular = %f]", *"*/ it->second.linear_velocity, it->second.angular_velocity /*"*);
//		}
//		
//		LOG_EX("Desirable Mask: ");
//		u32 m = desirable_mask();
//		
////		if ((m & eMovementParameterStand) == eMovementParameterStand) {
////			xr_map<u32,STravelParams>::const_iterator it = m_movement_params.find(eMovementParameterStand);
////			LOG_EX2("Mask: [linear = %f, angular = %f]", *"*/ it->second.linear_velocity, it->second.angular_velocity /*"*);
////		} 
////		if ((m & eMovementParameterWalkFree) == eMovementParameterWalkFree) {
////			xr_map<u32,STravelParams>::const_iterator it = m_movement_params.find(eMovementParameterWalkFree);
////			LOG_EX2("Mask: [linear = %f, angular = %f]", *"*/ it->second.linear_velocity, it->second.angular_velocity /*"*);
////		}
////		
////		if ((m & eMovementParameterRunFree) == eMovementParameterRunFree) {
////			xr_map<u32,STravelParams>::const_iterator it = m_movement_params.find(eMovementParameterRunFree);
////			LOG_EX2("Mask: [linear = %f, angular = %f]", *"*/ it->second.linear_velocity, it->second.angular_velocity /*"*);
////		}
//		
//		LOG_EX2("Start dir: [yaw = %f]", *"*/m_body.current.yaw /*"*);
//
//		LOG_EX("-------------------------------------------------");
//	}

	PreprocessAction();
	MotionMan.ProcessAction();
	
//	if (IsMovingOnPath()) {
//		UpdateVelocities();
//	}

	SetVelocity();

#pragma todo("Dima to Jim : This method will be automatically removed after 22.12.2003 00:00")
	set_desirable_speed		(m_fCurSpeed);
	
	// process sound
	ControlSound(m_current_update);
	
	m_head = m_body;
}

// ���������� �������� � ������� �������� � ������������ � ����������� �����
void CAI_Biting::SetVelocity()
{
	if (IsMovingOnPath()) {
		u32 velocity_index = CDetailPathManager::path()[curr_travel_point_index()].velocity;

		u32 next_point_velocity = u32(-1);
		if (CDetailPathManager::path().size() > curr_travel_point_index() + 1) 
			next_point_velocity = CDetailPathManager::path()[curr_travel_point_index() + 1].velocity;

		if ((velocity_index == eVelocityParameterStand) && (next_point_velocity != u32(-1))) {
			if (angle_difference(m_body.current.yaw, m_body.target.yaw) < PI_DIV_6/6) {
				velocity_index = next_point_velocity;
			}
		}

		xr_map<u32,STravelParams>::const_iterator it = m_movement_params.find(velocity_index);
		R_ASSERT(it != m_movement_params.end());

		m_fCurSpeed		= (*it).second.linear_velocity;
		m_body.speed	= (*it).second.angular_velocity;

//		m_fCurSpeed		= 6.0f;
//		m_body.speed	= 3.14f;

//		m_fCurSpeed		= velocities[curr_travel_point_index()].linear_velocity;
//		m_body.speed	= PI_DIV_2;

	} else m_fCurSpeed	= 0;	



}

// � ����������� �� �������� - �������� Action
void CAI_Biting::PreprocessAction()
{
	if (IsMovingOnPath()) {
		u32 velocity_index = CDetailPathManager::path()[curr_travel_point_index()].velocity;

		if (velocity_index == eVelocityParameterStand) MotionMan.m_tAction = ACT_STAND_IDLE;
		else if (velocity_index == eVelocityParameterWalkNormal) MotionMan.m_tAction = ACT_WALK_FWD;
		else if (velocity_index == eVelocityParameterRunNormal) MotionMan.m_tAction = ACT_RUN;

		else if (velocity_index == eVelocityParameterWalkDamaged) {
			MotionMan.m_tAction = ACT_WALK_FWD;
		} else if (velocity_index == eVelocityParameterRunDamaged) {
			MotionMan.m_tAction = ACT_RUN;
		} else if (velocity_index == eVelocityParameterSteal) {
			MotionMan.m_tAction = ACT_STEAL;
		} else if (velocity_index == eVelocityParameterDrag) {
			MotionMan.m_tAction = ACT_DRAG;
		}

		u32 next_point_velocity = u32(-1);
		if (CDetailPathManager::path().size() > curr_travel_point_index() + 1) 
			next_point_velocity = CDetailPathManager::path()[curr_travel_point_index() + 1].velocity;

		if ((velocity_index == eVelocityParameterStand) && (next_point_velocity != u32(-1))) {
			if (angle_difference(m_body.current.yaw, m_body.target.yaw) < PI_DIV_6/6) {
				MotionMan.m_tAction = ACT_RUN;
			}
		}
	}
}

void CAI_Biting::UpdateVelocities()
{
	// ��������� ������ velocities
	velocities.clear();
	velocities.reserve(CDetailPathManager::path().size());

	for (u32 i=0; i<CDetailPathManager::path().size(); i++) {
		xr_map<u32,STravelParams>::const_iterator it = m_movement_params.find(CDetailPathManager::path()[i].velocity);
		R_ASSERT(it != m_movement_params.end());
		
		velocities.push_back(it->second);
	}

	u32	start_point_index	= 0;
	u32	end_point_index		= 0;		// ������ ���������� ��������� ��������
	u32 cur_point_index		= 0;

	
	while (true) {							// ���� ��� ����� ����

		xr_vector<u32>	section;			// ������� � ������� ���������
		bool			b_path_end			= false;
		bool			b_velocity_changed	= false;
		section.clear						();
		
		start_point_index					= cur_point_index;
		STravelParams	start_velocity		= velocities[start_point_index];
		STravelParams	end_velocity;

		while (true) {							// ���� ��� �������� �������
			cur_point_index++;

			if (cur_point_index >= velocities.size()) {
				b_path_end = true;
				break;
			}

			STravelParams	new_velocity = velocities[cur_point_index];

			// ���� ����� �������� �� ����� ����������
			if ((new_velocity.linear_velocity != start_velocity.linear_velocity ) || 
				(new_velocity.angular_velocity != start_velocity.angular_velocity )) {

				b_velocity_changed = true;
				break;
			}
			
			section.push_back(cur_point_index);			
		}

		end_point_index = cur_point_index-1;

		if (b_path_end)			end_velocity = STravelParams(0.f,0.f);
		if (b_velocity_changed) end_velocity = velocities[cur_point_index];
		
		// ������������ ������� ������
		if (section.size() < 2) break;

		// ����� ��������� ������� ������
		float D = 0.f;
		for (u32 i=start_point_index; i<end_point_index; i++) 
			D += CDetailPathManager::path()[i].position.distance_to(CDetailPathManager::path()[i+1].position);
		

		STravelParams from	= start_velocity;
		STravelParams to	= end_velocity;
		STravelParams new_vel;

		float dist;

		for (u32 j = start_point_index; j < end_point_index; j++) {
			dist	= CDetailPathManager::path()[j].position.distance_to(CDetailPathManager::path()[j+1].position) / D;

			new_vel.linear_velocity = from.linear_velocity + (to.linear_velocity - from.linear_velocity) * dist;
			new_vel.angular_velocity = from.angular_velocity + (to.angular_velocity - from.angular_velocity) * dist;
	
			velocities[j] = new_vel;

			from = new_vel;
		}

		if (b_path_end) break;		
	}

	LOG_EX("----------------- VELOCITIES --------------------");
	for (int i=0;i<velocities.size();i++) {
		LOG_EX2("V%u = [%f]", *"*/ i+1, velocities[i].linear_velocity /*"*);
	}
	LOG_EX("-------------------------------------------------");
	

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAI_Biting state-specific functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void CAI_Biting::SetState(IState *pS, bool bSkipInertiaCheck)
{
	if (CurrentState != pS) {
		// �������� �������
		if (!bSkipInertiaCheck)
			if (CurrentState->IsInertia()) {
				if (CurrentState->GetPriority() >= pS->GetPriority()) return;
			}

			CurrentState->Done();
			CurrentState->Reset();
			CurrentState = pS;
			CurrentState->Activate();
	}
}
