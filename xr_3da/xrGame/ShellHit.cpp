#include "StdAfx.h"
#include "PHDynamicData.h"
#include "Physics.h"
#include "tri-colliderknoopc/dTriList.h"
#include "PHShellSplitter.h"
#include "PHFracture.h"
#include "PHJointDestroyInfo.h"
#include "ExtendedGeom.h"

#include "PHElement.h"
#include "PHShell.h"


void CPHShell::applyHit(const Fvector& pos, const Fvector& dir, float val,const u16 id,ALife::EHitType hit_type)
{
	if(!m_pKinematics)
	{
		applyImpulseTrace(pos,dir,val);
		return;
	}
	switch(hit_type) {
		case ALife::eHitTypeExplosion:
			ExplosionHit(pos,dir,val,id);
		break;
	default: applyImpulseTrace(pos,dir,val,id);
	}
}

void CPHShell::ExplosionHit(const Fvector& pos, const Fvector& dir, float val,const u16 id)
{
	EnableObject();
	//Fvector local_pos;local_pos.set(0.f,0.f,0.f);
	ELEMENT_I i=elements.begin(),e=elements.end();
	float impulse=val/_sqrt(_sqrt((float)elements.size()));
	for(;i!=e;i++)
	{	
		//Fvector max_area_dir;	
		CPHElement* element=(*i);
		//element->get_MaxAreaDir(max_area_dir);
		//float	sign=max_area_dir.dotproduct(dir)>0.f ? 1.f : -1.f;
		//max_area_dir.mul(sign);
		Fvector r_dir,r_pos,r_box;
		float rad=element->getRadius();
		r_box.set(rad,rad,rad);
		r_pos.random_point(r_box);
		r_dir.random_dir();
		r_dir.mul(0.5f);
		r_dir.add(dir);
		r_dir.normalize();//safe???
		
		element->applyImpulseTrace(r_pos,r_dir,impulse,0);
	}
}
