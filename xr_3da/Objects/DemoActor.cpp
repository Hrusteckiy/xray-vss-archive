#include "stdafx.h"
#include "..\CameraFirstEye.h"
#include "..\environment.h"
#include "..\xr_streamsnd.h"

#include "DemoActor.h"
#include "..\ObjectAnimator.h"
#include "dummyobject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDemoActor::CDemoActor()
{
	camera			= new CCameraFirstEye(this, pSettings, "actor_firsteye_cam", false);
	animator		= 0;
	attached		= 0;
	music			= 0;
}

CDemoActor::~CDemoActor()
{	
	_DELETE			(animator);
	_DELETE			(camera);
	_FREE			(attached);
}

void CDemoActor::Load	(LPCSTR section)
{
	inherited::Load		(section);
}

void CDemoActor::Update(u32 DT)
{
	inherited::Update(DT);
	if (time2start>0)
	{
		time2start-=float(DT)/1000.f;
		if (time2start<0) PlayDemo("demo");
	} else {
		animator->OnMove();
		if (animator->IsMotionActive()) {
			mRotate.set		(animator->GetRotate());
			vPosition.set	(animator->GetPosition());
			vPosition.add	(start_position);
			UpdateTransform	();
			
			// test nearest object
			pCreator->ObjectSpace.TestNearestObject(cfModel, vPosition, 1.f);
		} else {
			// animation stops
			time2start = time2start_ltx;
		}
	}
	if (IsMyCamera()) UpdateCamera();
}

void CDemoActor::PlayDemo(const char* name)
{
/*

	animator->PlayMotion		(name,false);
	animator->OnMove			();
	if (music)	music->Play		(false);
	if (attached){
		CObject* O		= pCreator->Objects.FindObjectByName(attached);
		R_ASSERT		(O);
		R_ASSERT		(O->SUB_CLS_ID==CLSID_OBJECT_DUMMY);
		dynamic_cast<CDummyObject*>(O)->PlayDemo(name);
	}
*/
}

void CDemoActor::StopDemo(){
	animator->StopMotion	();
}

void CDemoActor::g_fireParams(Fvector& P, Fvector& D){
	P.set			(vPosition);
	D.set			(mRotate.k);
}
