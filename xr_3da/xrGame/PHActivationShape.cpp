#include "stdafx.h"
#include "PHObject.h"
#include "Physics.h"
#include "MathUtils.h"
#include "phvalidevalues.h"
#include "PHActivationShape.h"
#include "Extendedgeom.h"
#include "SpaceUtils.h"
#include "MathUtils.h"
#include "GameMtlLib.h"
#include "Level.h"
#include "PHWorld.h"
#include <../ode/src/util.h>
#ifdef DEBUG
#include "PHDebug.h"
#endif
static float max_depth			=0.f;
static float friction_factor	=0.f;
static float cfm				=1.e-10f;
static float erp				=1.f;

void ActivateTestDepthCallback (bool& do_colide,dContact& c,SGameMtl* material_1,SGameMtl* material_2)
{
	
	if(do_colide&&!material_1->Flags.test(SGameMtl::flPassable) &&!material_2->Flags.test(SGameMtl::flPassable))
	{
		float& depth=c.geom.depth;
		float test_depth=depth;
		save_max(max_depth,test_depth);
		c.surface.mu*=friction_factor;
 
		c.surface.soft_cfm=cfm;
		c.surface.soft_erp=erp;
		
	}

}
void GetMaxDepthCallback (bool& do_colide,dContact& c,SGameMtl* material_1,SGameMtl* material_2)
{

	if(do_colide&&!material_1->Flags.test(SGameMtl::flPassable) &&!material_2->Flags.test(SGameMtl::flPassable))
	{
		float& depth=c.geom.depth;
		float test_depth=depth;
		save_max(max_depth,test_depth);
	}
	do_colide=false;
}
CPHActivationShape::CPHActivationShape()
{
	m_geom=NULL;
	m_body=NULL;
	m_flags.zero();
	m_flags.set(flFixedRotation,TRUE);
}
CPHActivationShape::~CPHActivationShape()
{
	VERIFY(!m_body&&!m_geom);
}
void	CPHActivationShape::	Create								(const Fvector start_pos,const Fvector start_size,CPhysicsShellHolder* ref_obj,EType type/*=etBox*/)
{
	m_body			=	dBodyCreate	(0)												;

	m_geom			=	dCreateBox	(0,start_size.x,start_size.y,start_size.z)		;

	dGeomCreateUserData				(m_geom)										;
	dGeomUserDataSetObjectContactCallback(m_geom,ActivateTestDepthCallback)			;
	dGeomUserDataSetPhysicsRefObject(m_geom,ref_obj)								;
	dGeomSetBody					(m_geom,m_body)									;
	dBodySetPosition				(m_body,start_pos.x,start_pos.y,start_pos.z)	;
	Island()		.AddBody		(m_body)										;
	dBodyEnable						(m_body)										;
	m_safe_state					.create(m_body)									;
	spatial_register				()												;
}
void CPHActivationShape::	Destroy	()
{
	VERIFY(m_geom&&m_body)				;
	spatial_unregister		()			;
	CPHObject::deactivate	()			;
	dGeomDestroyUserData	(m_geom)	;
	dGeomDestroy			(m_geom)	;
	m_geom					=NULL		;
	dBodyDestroy			(m_body)	;
	m_body					=NULL		;
}
bool	CPHActivationShape::	Activate							(const Fvector need_size,u16 steps,float max_displacement,float max_rotation)										
{
#ifdef	DEBUG 
	if(ph_dbg_draw_mask.test(phDbgDrawDeathActivationBox))
	{
		DBG_OpenCashedDraw();
		Fmatrix M;M.identity();M.c.set(cast_fv(dBodyGetPosition(m_body)));
		Fvector v;v.set(need_size);v.mul(0.5f);
		DBG_DrawOBB(M,v,D3DCOLOR_XRGB(0,255,0));
		DBG_ClosedCashedDraw(30000);
	}
#endif
	VERIFY(m_geom&&m_body);
	CPHObject::activate();
	ph_world->Freeze();
	UnFreeze();
	max_depth=0.f;

	dGeomUserDataSetObjectContactCallback(m_geom,GetMaxDepthCallback)			;
	ph_world->Step();
	
	u16		num_it =5;
	float	fnum_it=float(num_it);
	float	fnum_steps=float(steps);
	float	fnum_steps_r=1.f/fnum_steps;
	float	resolve_depth=0.01f;
	float	max_vel=2.f*max_depth/fnum_it*fnum_steps_r/fixed_step;
	

	float	max_a_vel=max_rotation/fnum_it*fnum_steps_r/fixed_step;
	ph_world->CutVelocity(max_vel,max_a_vel);
	
	dGeomUserDataSetObjectContactCallback(m_geom,ActivateTestDepthCallback)			;
	max_depth=0.f;
	
	Fvector from_size;
	Fvector step_size,size;
	dGeomBoxGetLengths(m_geom,cast_fp(from_size));
	step_size.sub(need_size,from_size);
	step_size.mul(fnum_steps_r);
	size.set(from_size);
	bool ret=false;
	for(int m=0;steps>m;++m)
	{
		//float param =fnum_steps_r*(1+m);
		//InterpolateBox(id,param);
		size.add(step_size);
		dGeomBoxSetLengths(m_geom,size.x,size.y,size.z);
		ret=false;
		for(int i=0;num_it>i;++i){
			max_depth=0.f;
			ph_world->Step();
			ph_world->CutVelocity(max_vel,max_a_vel);
			if(max_depth	<	resolve_depth) 
			{
				ret=true;
				break;
			}	
		}
		if(!ret) break;
	}
	ph_world->UnFreeze();
	return ret;
}
const Fvector&	CPHActivationShape::	Position							()																
{
	return cast_fv(dBodyGetPosition(m_body));
}
void	CPHActivationShape::	Size								(Fvector &size)																
{
	
}

void	CPHActivationShape::	PhDataUpdate						(dReal step)												
{
	m_safe_state.new_state(m_body);
}
void	CPHActivationShape::	PhTune								(dReal step)												
{

}
dGeomID	CPHActivationShape::	dSpacedGeom							()															
{
	return m_geom;
}
void	CPHActivationShape::	get_spatial_params					()															
{
	spatialParsFromDGeom(m_geom,spatial.center,AABB,spatial.radius);
}

void	CPHActivationShape::	InitContact							(dContact* c,bool& do_collide,SGameMtl * ,SGameMtl * )
{

}

void CPHActivationShape::CutVelocity(float l_limit,float /*a_limit*/)
{
	dVector3 limitedl,diffl;
	dVectorLimit(dBodyGetLinearVel(m_body),l_limit,limitedl);
	dVectorSub(diffl,limitedl,dBodyGetLinearVel(m_body));
	dBodySetLinearVel(m_body,diffl[0],diffl[1],diffl[2]);
	dBodySetAngularVel(m_body,0.f,0.f,0.f);
	dxStepBody(m_body,fixed_step);
	dBodySetLinearVel(m_body,limitedl[0],limitedl[1],limitedl[2]);

}