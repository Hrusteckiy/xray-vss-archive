#include "stdafx.h"
#pragma hdrstop

#include "HangingLamp.h"
#include "..\LightAnimLibrary.h"
#include "PhysicsShell.h"
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHangingLamp::CHangingLamp	()
{
	body					= 0;
	light_bone_idx			= -1;
	lanim					= 0;
	light_render			= ::Render->light_create();
	light_render->set_shadow(true);
}

CHangingLamp::~CHangingLamp	()
{
	xr_delete				(body);
	::Render->light_destroy	(light_render);
}

void CHangingLamp::Load		(LPCSTR section)
{
	inherited::Load			(section);
}

BOOL CHangingLamp::net_Spawn(LPVOID DC)
{
	xrSE_HangingLamp* lamp	= (xrSE_HangingLamp*)(DC);
	R_ASSERT				(lamp);
	cNameVisual_set			(lamp->visual_name);
	inherited::net_Spawn	(DC);

	// set bone id
	light_bone_idx			= lamp->spot_bone[0]?PKinematics(pVisual)->LL_BoneID(lamp->spot_bone):-1;
	light_render->set_range	(lamp->spot_range);
	light_render->set_color	(color_get_R(lamp->color),color_get_G(lamp->color),color_get_B(lamp->color));
	light_render->set_active(true);
	light_render->set_cone	(lamp->spot_cone_angle);
	light_render->set_texture(lamp->spot_texture[0]?lamp->spot_texture:0);

	R_ASSERT				(pVisual&&PKinematics(pVisual));
	PKinematics(pVisual)->PlayCycle("idle");
	PKinematics(pVisual)->Calculate();
	lanim					= LALib.FindItem(lamp->animator);

	CreateBody				();

	setVisible(true);
	setEnabled(true);

	return TRUE;
}

void CHangingLamp::Update	(u32 dt)
{
	inherited::Update		(dt);
}

void CHangingLamp::UpdateCL	()
{
	inherited::UpdateCL		();
	if(body){
		mRotate.i.set(body->mXFORM.i);
		mRotate.j.set(body->mXFORM.j);
		mRotate.k.set(body->mXFORM.k);
		vPosition.set(body->mXFORM.c);
		UpdateTransform();
	}
}

void CHangingLamp::OnVisible()
{
	inherited::OnVisible	();

	Fmatrix& M = (light_bone_idx>=0)?PKinematics(pVisual)->LL_GetTransform(light_bone_idx):clXFORM();
	light_render->set_direction	(M.k);
	light_render->set_position	(M.c);
	if (lanim){
		int frame;
		u32 clr		= lanim->Calculate(Device.fTimeGlobal,frame); // ���������� � ������� BGR
		light_render->set_color(color_get_B(clr),color_get_G(clr),color_get_R(clr));
	}
}

void CHangingLamp::Hit(float P, Fvector &dir,	CObject* who, s16 element,Fvector p_in_object_space, float impulse)
{
	inherited::Hit(P,dir,who,element,p_in_object_space,impulse);
	light_render->set_active(false);
}

void CHangingLamp::AddElement(CPhysicsElement* root_e, int id)
{
	CKinematics* K		= PKinematics(pVisual);

	CPhysicsElement* E	= P_create_Element();
	CBoneInstance& B	= K->LL_GetInstance(id);
	E->mXFORM.set		(K->LL_GetTransform(id));
	Fobb& bb			= K->LL_GetBox(id);
	bb.m_halfsize.add	(0.05f);
	E->add_Box			(bb);
	E->setDensity		(1000.f);
	E->set_ParentElement(root_e);
	B.set_callback		(body->GetBonesCallback(),E);
	body->add_Element	(E);

	if (root_e){
		CPhysicsJoint* J= P_create_Joint(CPhysicsJoint::full_control,root_e,E);
		J->SetAnchorVsSecondElement	(0,0,0);
		J->SetAxisVsSecondElement	(1,0,0,0);
		J->SetAxisVsSecondElement	(0,1,0,2);
		body->add_Joint	(J);
	}

	CBoneData& BD		= K->LL_GetData(id);
	for (vecBonesIt it=BD.children.begin(); it!=BD.children.end(); it++){
		AddElement		(E,(*it)->SelfID);
	}
}

void CHangingLamp::CreateBody()
{
	body				= P_create_Shell();
	body->set_Kinematics(PKinematics(pVisual));
	AddElement			(0,PKinematics(pVisual)->LL_BoneRoot());
	body->mXFORM.set(svTransform);
}
