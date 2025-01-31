#ifndef __SPECTATOR_H__
#define __SPECTATOR_H__

#pragma once

#include "../feel_touch.h"
#include "../iinputreceiver.h"

#include "entity.h"
#include "actor_flags.h"

// refs
class CActor;

class CSpectator: 
	public CGameObject,
	public IInputReceiver
{
private:
	typedef CGameObject		inherited;
protected:
	enum EActorCameras {
		eacFreeFly		= 0,
		eacFirstEye,
		eacLookAt,
		eacFreeLook,
		eacMaxCam
	};
private:
	// Cameras
	CCameraBase*			cameras[eacMaxCam];
	EActorCameras			cam_active;

	int						look_idx;

	//------------------------------
	void					cam_Set					(EActorCameras style);
	void					cam_Update				(CActor* A=0);
public:
							CSpectator				( );
	virtual					~CSpectator				( );

	virtual void			IR_OnMouseMove			(int x, int y);
	virtual void			IR_OnKeyboardPress		(int dik);
	virtual void			IR_OnKeyboardRelease	(int dik);
	virtual void			IR_OnKeyboardHold		(int dik);
	//virtual	void			Hit						(float P, Fvector &dir,	CObject* who, s16 element,Fvector p_in_object_space, float impulse){};
	virtual void			shedule_Update			( u32 T ); 
	virtual void			UpdateCL				( );
	virtual BOOL			net_Spawn				( CSE_Abstract*	DC );

	virtual void			Center					(Fvector& C)	const	{ C.set(Position());	}
	virtual float			Radius					()				const	{ return EPS;}
//	virtual const Fbox&		BoundingBox				()				const	{ VERIFY2(renderable.visual,*cName()); return renderable.visual->vis.box;									}
	virtual CGameObject*	cast_game_object		()						{return this;}
	virtual IInputReceiver*	cast_input_receiver		()						{return this;}
};

#endif // __SPECTATOR_H__
