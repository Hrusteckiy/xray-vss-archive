////////////////////////////////////////////////////////////////////////////
//	Module 		: state_manager_state.h
//	Created 	: 14.01.2004
//  Modified 	: 14.01.2004
//	Author		: Dmitriy Iassenev
//	Description : State manager state
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "state_manager_abstract.h"
#include "state_base.h"

class CStateManagerState : 
	public CStateManagerAbstract<
		CStateBase
	>,
	public CStateBase
{
protected:
	typedef CStateManagerAbstract<CStateBase> CSStateManagerAbstract;

	IC		xr_vector<u32> &sequence();
	IC		CStateBase	&current_state			();

public:
						CStateManagerState		();
	virtual				~CStateManagerState		();
			void		Init					();
	virtual	void		Load					(LPCSTR section);
	virtual	void		reinit					(CAI_Stalker *object, u32 state_id);
	virtual	void		reload					(LPCSTR section);
	virtual	void		initialize				();
	virtual	void		execute					();
	virtual	void		finalize				();
	virtual	void		update					(u32 time_delta);
			bool		completed				();
	IC		const xr_vector<u32> &sequence		() const;
	IC		const CStateBase &current_state		() const;
};

#include "state_manager_state_inline.h"