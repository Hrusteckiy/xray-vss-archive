//////////////////////////////////////////////////////////////////////
// ExplosiveItem.cpp:	����� ��� ���� ������� ���������� ��� 
//						��������� ��������� ����� (��������,
//						������ � ����� � �.�.)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ExplosiveItem.h"


CExplosiveItem::CExplosiveItem(void)
{
}
CExplosiveItem::~CExplosiveItem(void)
{
}

void CExplosiveItem::Load(LPCSTR section)
{
	inherited::Load							(section);
	CExplosive::Load						(section);
	m_bUsingCondition						= true;
}

void CExplosiveItem::net_Destroy()
{
	inherited::net_Destroy();
	CExplosive::net_Destroy();
}

void CExplosiveItem::Hit(float P, Fvector &dir,	CObject* who, s16 element,
						Fvector position_in_object_space, float impulse, 
						ALife::EHitType hit_type)
{
	inherited::Hit(P,dir,who,element,position_in_object_space,impulse,hit_type);

	
	if(GetCondition()<=0.f)
	{
		//��������� ����, ��� ������� ����
		SetInitiator( who->ID());

		Fvector normal;
		FindNormal(normal);
		CExplosive::GenExplodeEvent(Position(), normal);
	}
}

void  CExplosiveItem::OnEvent (NET_Packet& P, u16 type)
{
	CExplosive::OnEvent (P, type);
	inherited::OnEvent (P, type);

}
void CExplosiveItem::UpdateCL()
{
	CExplosive::UpdateCL();
	inherited::UpdateCL();
}
void CExplosiveItem::renderable_Render()
{
	inherited::renderable_Render();
}