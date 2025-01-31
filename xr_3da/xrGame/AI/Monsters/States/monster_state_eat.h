#pragma once

#include "../state.h"

template<typename _Object>
class	CStateMonsterEat : public CState<_Object> {
protected:
	typedef CState<_Object>		inherited;
	typedef CState<_Object>*	state_ptr;

public:
						CStateMonsterEat		(_Object *obj);
						CStateMonsterEat		(_Object *obj, state_ptr state_eat);
	virtual				~CStateMonsterEat		();

	virtual void		initialize				();
	virtual	void		reselect_state			();
	virtual	void		setup_substates			();
	virtual bool		check_completion		();
	virtual bool		check_start_conditions	();

};

#include "monster_state_eat_inline.h"
