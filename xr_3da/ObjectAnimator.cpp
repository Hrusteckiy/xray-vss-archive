#include "stdafx.h"
#pragma hdrstop

#include "ObjectAnimator.h"
#include "motion.h"
 
bool st_AnimParam::Update(float dt){ 
	t+=dt; 
	bWrapped	= false;
	if (t>max_t){ 
		bWrapped= true;
		if (bLoop){ t=min_t; return true; }
		else	  { t=max_t; return false;}
	}
	return true;
}
void st_AnimParam::Set(COMotion* M, bool _loop){ 
	t			= 0; 
	bLoop		= _loop;
    min_t		= (float)M->FrameStart()/M->FPS(); 
    max_t		= (float)M->FrameEnd()/M->FPS();
	bWrapped	= false;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObjectAnimator::CObjectAnimator()
{
}

CObjectAnimator::~CObjectAnimator()
{	
	for(MotionPairIt m_it=m_Motions.begin(); m_it!=m_Motions.end(); m_it++)
	{
		xr_free		((char*)m_it->first);
		xr_delete	(m_it->second);
	}
	m_Motions.clear		();
}

void CObjectAnimator::SetActiveMotion(COMotion* mot, bool bLoop)
{
	m_ActiveMotion		=mot;
    if (m_ActiveMotion) m_MParam.Set(m_ActiveMotion, bLoop);
	vPosition.set		(0,0,0);
	mRotate.identity	();
}

void CObjectAnimator::LoadMotions(const char* fname)
{
	string256			full_path;
	if (!FS.exist( full_path, "$level$", fname ))
		if (!FS.exist( full_path, "$game_meshes$", fname ))
			Debug.fatal("Can't find motion file '%s'.",fname);

	IReader* F			= FS.r_open(full_path);
	u32 dwMCnt			= F->r_u32(); VERIFY(dwMCnt);
	for (u32 i=0; i<dwMCnt; i++){
		COMotion* M		= xr_new<COMotion> ();
		bool bRes		= M->Load(*F);
		if (!bRes)		Debug.fatal("ERROR: Can't load motion. Incorrect file version.");
		m_Motions[xr_strdup(M->Name())]=M;
	}
	FS.r_close		(F);
}

/*
void CObjectAnimator::Load(CInifile* ini, const char * section)
{
	LPCSTR temp			= ini->r_string(section,"motions");
	Load				(temp);
}
*/

void CObjectAnimator::Load(const char * name)
{
	LoadMotions			(name);
	SetActiveMotion		(0,false);
}

void CObjectAnimator::OnMove()
{
	if (IsMotionActive()){
		Fvector R;
		m_ActiveMotion->_Evaluate(m_MParam.Frame(),vPosition,R);
		if (!m_MParam.Update(Device.fTimeDelta)) StopMotion();
		mRotate.setHPB	(-R.x,-R.y,-R.z);
	}
}

COMotion* CObjectAnimator::PlayMotion(LPCSTR name, bool bLoop)
{
	MotionPairIt I = m_Motions.find(LPSTR(name));
	if (I!=m_Motions.end())	SetActiveMotion(I->second, bLoop);
	else {
		Debug.fatal	("OBJ ANIM::Cycle '%s' not found.",name);
		return NULL;
	}
	return I->second;
}

void CObjectAnimator::StopMotion(){
	SetActiveMotion		(0);
}

void CObjectAnimator::ManualUpdate(COMotion* M, float t){
	R_ASSERT2(0,"XYZ");
	Fvector R;
	m_ActiveMotion->_Evaluate(t,vPosition,R);
	mRotate.setHPB		(-R.x,-R.y,R.z);
}
