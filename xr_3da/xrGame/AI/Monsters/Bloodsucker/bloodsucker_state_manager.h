#pragma once
#include "../state_manager_second.h"

class CAI_Bloodsucker;

class CStateManagerBloodsucker : public CStateManagerSecond<CAI_Bloodsucker> {
	typedef CStateManagerSecond<CAI_Bloodsucker> inherited;

public:
					CStateManagerBloodsucker	(CAI_Bloodsucker *monster); 
	virtual void	execute						();
};
