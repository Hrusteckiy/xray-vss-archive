#include "stdafx.h"
#include "zombie.h"
#include "zombie_state_manager.h"
#include "../ai_monster_utils.h"
#include "../states/monster_state_rest.h"
#include "../states/monster_state_attack.h"
#include "../states/monster_state_eat.h"
#include "../states/monster_state_hear_int_sound.h"
#include "zombie_state_attack_run.h"
#include "../../../entitycondition.h"
#include "../../../detail_path_manager.h"
#include "../states/monster_state_controlled.h"

CStateManagerZombie::CStateManagerZombie(CZombie *obj) : inherited(obj)
{
	add_state(eStateRest,				xr_new<CStateMonsterRest<CZombie> >	(obj));
	add_state(
		eStateAttack,				
		xr_new<CStateMonsterAttack<CZombie> > (obj, 
			xr_new<CStateZombieAttackRun<CZombie> > (obj), 
			xr_new<CStateMonsterAttackMelee<CZombie> > (obj)
		)
	);
	add_state(eStateEat,					xr_new<CStateMonsterEat<CZombie> >(obj));
	add_state(eStateHearInterestingSound,	xr_new<CStateMonsterHearInterestingSound<CZombie> >(obj));
	add_state(eStateControlled,				xr_new<CStateMonsterControlled<CZombie> >	(obj));
}

CStateManagerZombie::~CStateManagerZombie()
{
}

void CStateManagerZombie::execute()
{
	if (object->MotionMan.TA_IsActive()) return;
	
	u32 state_id = u32(-1);
	
	if (!object->is_under_control()) {
	
		const CEntityAlive* enemy	= object->EnemyMan.get_enemy();

		if (enemy) {
			state_id = eStateAttack;
		} else if (object->hear_interesting_sound || object->hear_dangerous_sound) {
			state_id = eStateHearInterestingSound;
		} else {
			if (can_eat())	state_id = eStateEat;
			else			state_id = eStateRest;
		}
	} else state_id = eStateControlled;

	// установить текущее состояние
	select_state(state_id); 

	// выполнить текущее состояние
	get_state_current()->execute();

	prev_substate = current_substate;
}

