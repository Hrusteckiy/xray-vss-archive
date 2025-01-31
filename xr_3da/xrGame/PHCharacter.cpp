#include "stdafx.h"

#include "phcharacter.h"
#include "PHDynamicData.h"
#include "Physics.h"
#include "ExtendedGeom.h"
#include "../cl_intersect.h"
#include "tri-colliderKNoOPC\__aabb_tri.h"
#include <../ode/src/util.h>

CPHCharacter::CPHCharacter(void):
  CPHDisablingTranslational()
{

m_params.acceleration=0.001f;
m_params.velocity=0.0001f;
m_body=NULL;


m_safe_velocity[0]=0.f;
m_safe_velocity[1]=0.f;
m_safe_velocity[2]=0.f;
m_restriction_type=rtNone;
b_actor_movable=true;
}

CPHCharacter::~CPHCharacter(void)
{

}

void	CPHCharacter::FreezeContent()
{
	
	dBodyDisable(m_body);
	CPHObject::FreezeContent();
}
void	CPHCharacter::UnFreezeContent()
{

	dBodyEnable(m_body);
	CPHObject::UnFreezeContent();
}
void	CPHCharacter::getForce(Fvector& force)
{
	if(!b_exist)return;
	force.set(*(Fvector*)dBodyGetForce(m_body));
}
void	CPHCharacter::setForce(const Fvector &force)
{
	if(!b_exist)return;
	dBodySetForce(m_body,force.x,force.y,force.z);
}


void CPHCharacter::get_State(SPHNetState& state)
{
	GetPosition(state.position);
	m_body_interpolation.GetPosition(state.previous_position,0);
	GetVelocity(state.linear_vel);
	getForce(state.force);

	state.angular_vel.set(0.f,0.f,0.f);
	state.quaternion.identity();
	state.previous_quaternion.identity();
	state.torque.set(0.f,0.f,0.f);
//	state.accel = GetAcceleration();
//	state.max_velocity = GetMaximumVelocity();

	if(!b_exist) 
	{
		state.enabled=false;
		return;
	}
	state.enabled=CPHObject::is_active();//!!dBodyIsEnabled(m_body);
}
void CPHCharacter::set_State(const SPHNetState& state)
{
	m_body_interpolation.SetPosition(state.previous_position,0);
	m_body_interpolation.SetPosition(state.position,1);
	SetPosition(state.position);
	SetVelocity(state.linear_vel);
	setForce(state.force);
	
//	SetAcceleration(state.accel);
//	SetMaximumVelocity(state.max_velocity);

	if(!b_exist) return;
	if(state.enabled) 
	{
		Enable();
	};
	if(!state.enabled ) 
	{
		Disable();
	};
	VERIFY2(dBodyStateValide(m_body),"WRONG BODYSTATE WAS SET");
}

void CPHCharacter::Disable()
{

	CPHObject::deactivate();
	dBodyDisable(m_body);

}

void CPHCharacter::Enable()
{
	if(!b_exist) return;
	CPHObject::activate();
	dBodyEnable(m_body);

}







void __stdcall CarHitCallback(bool& /**do_colide/**/,dContact& /**c/**/)
{
	/*
	dxGeomUserData* usr_data_1=NULL;
	dxGeomUserData* usr_data_2=NULL;
	l_pUD1 = retrieveGeomUserData(c.geom.g1);
	l_pUD2 = retrieveGeomUserData(c.geom.g2);

	if(!(usr_data_1&&usr_data_2)) return;

	CPHCharacter* Character;
	Character=smart_cast<CPHCharacter*>(usr_data_1->ph_object);
	if(Character)
	{
	CGameObject* Obj=smart_cast<CGameObject*>(usr_data_1->ph_ref_object);
	CCar*		 Car=smart_cast<CCar*>	   (usr_data_2->ph_ref_object);
	if(Obj&&Car)
	{
	Fvector vel,rvel;
	Car->GetVelocity(vel);
	rvel.sub(vel,Character->GetVelocity());
	if(rvel.dotproduct(*((Fvector*)c.geom.normal))>1.f)
	{
	vel.normalize();
	Fvector pos;

	pos.set(0,0,0);
	Obj->Hit(100.f,vel,Car,0,pos,1000.f);
	Obj->PHSetPushOut();
	}
	}
	}

	Character=smart_cast<CPHCharacter*>(usr_data_2->ph_object);
	if(Character)
	{
	CGameObject* Obj=smart_cast<CGameObject*>(usr_data_2->ph_ref_object);
	CCar*		 Car=smart_cast<CCar*>	   (usr_data_1->ph_ref_object);
	if(Obj&&Car)
	{
	Fvector vel,rvel;
	Car->GetVelocity(vel);
	rvel.sub(vel,Character->GetVelocity());
	if(-rvel.dotproduct(*((Fvector*)c.geom.normal))>5.f)
	{
	vel.normalize();
	Fvector pos;

	pos.set(0,0,0);
	Obj->Hit(100.f,vel,Car,0,pos,10.f);
	Obj->PHSetPushOut();
	}
	}
	}

	*/
}

void CPHCharacter::CutVelocity(float l_limit,float /*a_limit*/)
{
	dVector3 limitedl,diffl;
	dVectorLimit(dBodyGetLinearVel(m_body),l_limit,limitedl);
	dVectorSub(diffl,limitedl,dBodyGetLinearVel(m_body));
	dBodySetLinearVel(m_body,diffl[0],diffl[1],diffl[2]);
	dBodySetAngularVel(m_body,0.f,0.f,0.f);
	dxStepBody(m_body,fixed_step);
	dBodySetLinearVel(m_body,limitedl[0],limitedl[1],limitedl[2]);

}