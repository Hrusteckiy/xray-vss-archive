#include "stdafx.h"
#include "CustomZone.h"
#include "../SkeletonAnimated.h"
#include "ZoneVisual.h"
#include "HairsZone.h"
#include "hudmanager.h"
#include "level.h"


bool CHairsZone::BlowoutState()
{
	bool result = inherited::BlowoutState();
	if(!result) UpdateBlowout();

	return result;
}

void CHairsZone::Affect(SZoneObjectInfo* O) 
{
	CPhysicsShellHolder *pGameObject = smart_cast<CPhysicsShellHolder*>(O->object);
	if(!pGameObject) return;

	if(O->zone_ignore) return;

	Fvector P; 
	XFORM().transform_tiny(P,CFORM()->getSphere().P);

#ifdef DEBUG
	char l_pow[255]; 
	sprintf(l_pow, "zone hit. %.1f", Power(pGameObject->Position().distance_to(P)));
	if(bDebug) HUD().outMessage(0xffffffff,pGameObject->cName(), l_pow);
#endif

	Fvector hit_dir; 
	hit_dir.set(::Random.randF(-.5f,.5f), 
		::Random.randF(.0f,1.f), 
		::Random.randF(-.5f,.5f)); 
	hit_dir.normalize();


	Fvector position_in_bone_space;

	P.y=0.f;
	float power = Power(pGameObject->Position().distance_to(P));
	float impulse = m_fHitImpulseScale*power*pGameObject->GetMass();

	//���������� �� �������
	O->total_damage += power;
	O->hit_num++;

	if(power > 0.01f) 
	{
		m_dwDeltaTime = 0;
		position_in_bone_space.set(0.f,0.f,0.f);

		CreateHit(pGameObject->ID(),ID(),hit_dir,power,0,position_in_bone_space,impulse,m_eHitTypeBlowout);
/*
		if (OnServer())
		{
			NET_Packet	l_P;
			u_EventGen	(l_P,GE_HIT, pGameObject->ID());
			l_P.w_u16	(ID());
			l_P.w_u16	(ID());
			l_P.w_dir	(hit_dir);
			l_P.w_float	(power);
			l_P.w_s16	(0);
			l_P.w_vec3	(position_in_bone_space);
			l_P.w_float	(impulse);
			l_P.w_u16	((u16)m_eHitTypeBlowout);
			u_EventSend	(l_P);
		};
*/

		PlayHitParticles(pGameObject);
	}
}