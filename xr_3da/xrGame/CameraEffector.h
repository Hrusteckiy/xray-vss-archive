// CameraEffector.h: interface for the CCameraEffector class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../CameraDefs.h"
#include "../cameramanager.h"

enum ECameraEffectorType
{
	eCEFall,
	eCENoise,
	eCEShot,
	eCEZoom,
	eCERecoil,
	eCEBobbing,
	eCEHit
};

class CEffectorShot;
class CEffectorZoomInertion;

class CCameraEffector
{
protected:
	ECameraEffectorType		eType;
	BOOL					bAffected;
	float					fLifeTime;
public:
	CCameraEffector							(ECameraEffectorType type, float tm, BOOL affected) {eType = type; fLifeTime=tm; bAffected=affected;};
	virtual				~CCameraEffector	() {};
	IC ECameraEffectorType	GetType			() {return eType;}
	IC BOOL					Affected		() {return bAffected;}
	IC float				LifeTime		() {return fLifeTime;}

	virtual CEffectorShot			*cast_effector_shot				()	{return 0;}
	virtual CEffectorZoomInertion	*cast_effector_zoom_inertion	()	{return 0;}

	virtual	BOOL		Process			(Fvector &p, Fvector &d, Fvector &n, float& fFov, float& fFar, float& fAspect)= 0;
};