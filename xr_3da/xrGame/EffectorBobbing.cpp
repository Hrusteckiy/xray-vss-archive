#include "stdafx.h"
#include "EffectorBobbing.h"
#include "Actor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectorBobbing::CEffectorBobbing() : CEffector(cefBobbing,10000.f,FALSE)
{
	fTime			= 0;
	fReminderFactor	= 0;
}

CEffectorBobbing::~CEffectorBobbing	()
{
}

void CEffectorBobbing::SetState(u32 mstate){
	dwMState		= mstate;
}

#define AMPLITUDE_RUN	0.0075f
#define AMPLITUDE_WALK	0.005f
#define SPEED_RUN		10.f
#define SPEED_WALK		7.f
#define CROUCH_FACTOR	0.75f
#define SPEED_REMINDER	5.f 

BOOL CEffectorBobbing::Process		(Fvector &p, Fvector &d, Fvector &n, float& /**fFov/**/, float& /**fFar/**/, float& /**fAspect/**/)
{
	fTime			+= Device.fTimeDelta;
	if (dwMState&CActor::EMoveCommand::mcAnyMove){
		if (fReminderFactor<1.f)	fReminderFactor += SPEED_REMINDER*Device.fTimeDelta;
		else						fReminderFactor = 1.f;
	}else{
		if (fReminderFactor>0.f)	fReminderFactor -= SPEED_REMINDER*Device.fTimeDelta;
		else						fReminderFactor = 0.f;
	}
	if (!fsimilar(fReminderFactor,0)){
		Fmatrix		M;
		M.identity	();
		M.j.set		(n);
		M.k.set		(d);
		M.i.crossproduct(n,d);
		M.c.set		(p);
		
		// apply footstep bobbing effect
		Fvector dangle;
		float k		= ((dwMState&CActor::EMoveCommand::mcCrouch)?CROUCH_FACTOR:1.f);
		float A		= (CActor::isAccelerated(dwMState)?AMPLITUDE_RUN:AMPLITUDE_WALK)*k;
		float ST	= ((CActor::isAccelerated(dwMState)?SPEED_RUN:SPEED_WALK)*fTime)*k;
	
		float _sinA	= _abs(_sin(ST)*A)*fReminderFactor;
		float _cosA	= _cos(ST)*A*fReminderFactor;

		p.y			+=	_sinA;
		dangle.x	=	_cosA;
		dangle.z	=	_cosA;
		dangle.y	=	_sinA;

		Fmatrix		R;
		R.setHPB	(dangle.x,dangle.y,dangle.z);

		Fmatrix		mR;
		mR.mul		(M,R);
		
		d.set		(mR.k);
		n.set		(mR.j);
	}
//	else{
//		fTime		= 0;
//	}
	return TRUE;
}
