///////////////////////////////////////////////////////////////
// GraviArtifact.cpp
// GraviArtefact - �������������� ��������, ������� �� �����
// � ����������� ����� ��� ������
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraviArtifact.h"
#include "PhysicsShell.h"
#include "level.h"
#include "xrmessages.h"
#include "game_cl_base.h"
#include "../skeletoncustom.h"

#define CHOOSE_MAX(x,inst_x,y,inst_y,z,inst_z)\
	if(x>y)\
		if(x>z){inst_x;}\
		else{inst_z;}\
	else\
		if(y>z){inst_y;}\
		else{inst_z;}


CGraviArtefact::CGraviArtefact(void) 
{
	shedule.t_min = 20;
	shedule.t_max = 50;
	
	m_fJumpHeight = 0;
	m_fEnergy = 1.f;
}

CGraviArtefact::~CGraviArtefact(void) 
{
}

void CGraviArtefact::Load(LPCSTR section) 
{
	// verify class
	LPCSTR Class = pSettings->r_string(section,"class");
	CLASS_ID load_cls = TEXT2CLSID(Class);
	R_ASSERT(load_cls==CLS_ID);

	inherited::Load(section);

	if(pSettings->line_exist(section, "jump_height")) m_fJumpHeight = pSettings->r_float(section,"jump_height");
//	m_fEnergy = pSettings->r_float(section,"energy");
}



void CGraviArtefact::UpdateCLChild() 
{

	if (getVisible() && m_pPhysicsShell) {
		if (m_fJumpHeight) {
			Fvector dir; 
			dir.set(0, -1.f, 0);
			collide::rq_result RQ;
			BOOL		enabled = getEnabled();
			setEnabled	(FALSE);
			
			//��������� ������ ���������
			if(Level().ObjectSpace.RayPick(Position(), dir, m_fJumpHeight, collide::rqtBoth, RQ)) 
			{
				dir.y = 1.f; 
				m_pPhysicsShell->applyImpulse(dir, 
											  30.f * Device.fTimeDelta * 
											  m_pPhysicsShell->getMass());
			}
			setEnabled	(enabled);
		}
	} else 
		if(H_Parent()) 
		{
			XFORM().set(H_Parent()->XFORM());
			
			if (GameID() == GAME_ARTEFACTHUNT && m_CarringBoneID != u16(-1))
			{
				CKinematics* K	= smart_cast<CKinematics*>(H_Parent()->Visual());
				if (K)
				{
					K->CalculateBones	();
					Fmatrix Ruck_MTX	= K->LL_GetTransform(m_CarringBoneID);
					Fvector	x;
					x.set(-0.1f, 0.f, -0.3f);
					Ruck_MTX.translate_add(x);
					Ruck_MTX.mulA		(XFORM());
					XFORM().set(Ruck_MTX);
				};
			};
		};
}