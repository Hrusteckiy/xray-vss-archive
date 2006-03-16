////////////////////////////////////////////////////////////////////////////
//	Module 		: stalker_alife_task_actions.cpp
//	Created 	: 25.10.2004
//  Modified 	: 25.10.2004
//	Author		: Dmitriy Iassenev
//	Description : Stalker alife task action classes
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stalker_alife_task_actions.h"
#include "ai/stalker/ai_stalker.h"
#include "ai/trader/ai_trader.h"
#include "inventory_item.h"
#include "weapon.h"
#include "script_game_object.h"
#include "inventory.h"
#include "alife_simulator.h"
#include "alife_object_registry.h"
#include "stalker_decision_space.h"
#include "cover_manager.h"
#include "cover_evaluators.h"
#include "cover_point.h"
#include "movement_manager_space.h"
#include "detail_path_manager_space.h"
#include "game_location_selector.h"
#include "sight_manager.h"
#include "ai_object_location.h"
#include "stalker_movement_manager.h"
#include "ai/stalker/ai_stalker_space.h"
#include "ai_space.h"
#include "xrServer_Objects_ALife_Monsters.h"
#include "alife_human_brain.h"
#include "alife_smart_terrain_task.h"

using namespace StalkerSpace;
using namespace StalkerDecisionSpace;

//////////////////////////////////////////////////////////////////////////
// CStalkerActionSolveZonePuzzle
//////////////////////////////////////////////////////////////////////////

CStalkerActionSolveZonePuzzle::CStalkerActionSolveZonePuzzle	(CAI_Stalker *object, LPCSTR action_name) :
	inherited				(object,action_name)
{
}

void CStalkerActionSolveZonePuzzle::initialize	()
{
	inherited::initialize						();

	m_stop_weapon_handling_time					= Device.dwTimeGlobal;
	if (object().inventory().ActiveItem() && object().best_weapon() && (object().inventory().ActiveItem()->object().ID() == object().best_weapon()->object().ID()))
		m_stop_weapon_handling_time				+= ::Random32.random(30000) + 30000;

	object().movement().set_desired_position	(0);
	object().movement().set_desired_direction	(0);
	object().movement().set_path_type			(MovementManager::ePathTypeGamePath);
	object().movement().set_detail_path_type	(DetailPathManager::eDetailPathTypeSmooth);
	object().movement().set_body_state			(eBodyStateStand);
	object().movement().set_movement_type		(eMovementTypeWalk);
	object().movement().set_mental_state		(eMentalStateFree);
	object().sight().setup						(CSightAction(SightManager::eSightTypeCover,false,true));
}

void CStalkerActionSolveZonePuzzle::finalize	()
{
	inherited::finalize				();

	if (!object().g_Alive())
		return;

	object().sound().remove_active_sounds		(u32(eStalkerSoundMaskNoHumming));
}

void CStalkerActionSolveZonePuzzle::execute		()
{
	inherited::execute				();

	if (Device.dwTimeGlobal >= m_stop_weapon_handling_time)
		if (!object().best_weapon())
			object().CObjectHandler::set_goal	(eObjectActionIdle);
		else
			object().CObjectHandler::set_goal	(eObjectActionStrapped,object().best_weapon());
	else
		object().CObjectHandler::set_goal		(eObjectActionIdle,object().best_weapon());
}

//////////////////////////////////////////////////////////////////////////
// CStalkerActionSmartTerrain
//////////////////////////////////////////////////////////////////////////

CStalkerActionSmartTerrain::CStalkerActionSmartTerrain	(CAI_Stalker *object, LPCSTR action_name) :
	inherited				(object,action_name)
{
	set_inertia_time		(30000);
}

void CStalkerActionSmartTerrain::initialize				()
{
	inherited::initialize							();
	object().movement().set_desired_position		(0);
	object().movement().set_desired_direction		(0);
	object().movement().game_selector().set_selection_type		(eSelectionTypeMask);
	object().movement().set_detail_path_type		(DetailPathManager::eDetailPathTypeSmooth);
	object().movement().set_body_state				(eBodyStateStand);
	object().movement().set_movement_type			(eMovementTypeWalk);
	object().movement().set_mental_state			(eMentalStateFree);
	object().sight().setup							(CSightAction(SightManager::eSightTypePathDirection));

	if (!object().best_weapon()) {
		object().CObjectHandler::set_goal		(eObjectActionIdle);
		return;
	}

	object().CObjectHandler::set_goal			(eObjectActionIdle);

	CWeapon										*best_weapon = smart_cast<CWeapon*>(object().best_weapon());
	if (object().CObjectHandler::weapon_strapped(best_weapon))
		return;

	object().CObjectHandler::set_goal			(eObjectActionIdle,object().best_weapon());
}

void CStalkerActionSmartTerrain::finalize				()
{
	inherited::finalize					();
	object().movement().game_selector().set_selection_type		(eSelectionTypeRandomBranching);
}

void CStalkerActionSmartTerrain::execute				()
{
	inherited::execute					();

	if (completed())
		object().CObjectHandler::set_goal		(eObjectActionStrapped,object().best_weapon());

	object().sound().play						(eStalkerSoundHumming,60000,10000);

	CSE_ALifeHumanAbstract						*stalker = smart_cast<CSE_ALifeHumanAbstract*>(ai().alife().objects().object(m_object->ID()));
	VERIFY										(stalker);
	VERIFY										(stalker->m_smart_terrain_id != 0xffff);

	CALifeSmartTerrainTask						*task = stalker->brain().smart_terrain().task(stalker);
	THROW2										(task,"Smart terrain is assigned but returns no task");
	if (object().ai_location().game_vertex_id() != task->game_vertex_id()) {
		object().movement().set_path_type		(MovementManager::ePathTypeGamePath);
		object().movement().set_game_dest_vertex(task->game_vertex_id());
		return;
	}

	object().movement().set_path_type			(MovementManager::ePathTypeLevelPath);
	if (object().movement().accessible(task->level_vertex_id())) {
		object().movement().set_level_dest_vertex	(task->level_vertex_id());
		Fvector										temp = task->position();
		object().movement().set_desired_position	(&temp);
		return;
	}

	object().movement().set_nearest_accessible_position	(task->position(),task->level_vertex_id());
}
