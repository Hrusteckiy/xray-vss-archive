// WeaponHUD.cpp:	HUD ��� ������ � ������ ���������, �������
//					����� ������� � ����� ���������, ����� ������������
//					��� ������������� �������� � ����� �� 3-�� ����
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "WeaponHUD.h"
#include "Weapon.h"
#include "../Motion.h"
#include "../skeletonanimated.h"

SHARED_HUD_INFO::SHARED_HUD_INFO(LPCSTR section, CHudItem* pHudItem)
{	
	// Geometry and transform
	Fvector						pos,ypr;
	pos							= pSettings->r_fvector3(section,"position");
	ypr							= pSettings->r_fvector3(section,"orientation");
	ypr.mul						(PI/180.f);

	m_Offset.setHPB				(ypr.x,ypr.y,ypr.z);
	m_Offset.translate_over		(pos);

	// Visual
	LPCSTR visual_name			= pSettings->r_string(section, "visual");
	pVisual						= ::Render->model_Create(visual_name);
	//	R_ASSERT					(pVisual->Type==MT_SKELETON_ANIM);

	// fire bone	
	if(smart_cast<CWeapon*>(pHudItem)){
		LPCSTR fire_bone		= pSettings->r_string					(section,"fire_bone");
		iFireBone				= smart_cast<CKinematics*>(pVisual)->LL_BoneID	(fire_bone);
		if (iFireBone>=smart_cast<CKinematics*>(pVisual)->LL_BoneCount())	
			Debug.fatal	("There is no '%s' bone for weapon '%s'.",fire_bone, section);

		vFirePoint				= pSettings->r_fvector3					(section,"fire_point");
	}else{
		iFireBone				= -1;
	}

	if(pSettings->line_exist(section,"fire_point2")) 
		vFirePoint2				= pSettings->r_fvector3(section,"fire_point2");
	else 
		vFirePoint2				= vFirePoint;

	if(pHudItem && pSettings->line_exist(pHudItem->object().cNameSect(), "shell_particles")) 
		vShellPoint				= pSettings->r_fvector3	(section,"shell_point");
}

SHARED_HUD_INFO::~SHARED_HUD_INFO()
{
	::Render->model_Delete		(pVisual);
}


//-----------------------------------------------------------------------

SHARED_HUD_INFO_MAP	CWeaponHUD::m_SharedHudInfoMap;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWeaponHUD::CWeaponHUD		()
{
	m_pParentWeapon				= NULL;
	m_pSharedHudInfo			= NULL;

	m_bHidden = true;

	m_bStopAtEndAnimIsRunning = false;
	m_pCallbackItem = NULL;
}
CWeaponHUD::CWeaponHUD(CHudItem* pHudItem)
{
	m_pParentWeapon				= pHudItem;
	m_pSharedHudInfo			= NULL;

	m_bHidden = true;

	m_bStopAtEndAnimIsRunning = false;
	m_pCallbackItem = NULL;
}

CWeaponHUD::~CWeaponHUD()
{
	ReleaseHudInfo				(m_sHudSectionName);
}

void CWeaponHUD::Load			(LPCSTR section)
{
	//��������� ��� ������ HUD
	m_sHudSectionName = section;

	//�������� ��������� �� ���������� � HUD
	m_pSharedHudInfo = GetHudInfo(m_sHudSectionName, m_pParentWeapon);
}

void  CWeaponHUD::Init	()
{
	m_bStopAtEndAnimIsRunning = false;
	m_pCallbackItem = NULL;
}


void  CWeaponHUD::net_DestroyHud	()
{
	m_bStopAtEndAnimIsRunning = false;
	m_pCallbackItem = NULL;
	m_bCurrentEntityIsParent = false;
}

void CWeaponHUD::UpdatePosition(const Fmatrix& trans)
{
	Transform().mul	(trans,m_pSharedHudInfo->m_Offset);
	VERIFY							(!fis_zero(DET(Transform())));
}

MotionID CWeaponHUD::animGet		(LPCSTR name)
{
	return smart_cast<CSkeletonAnimated*>(Visual())->ID_Cycle_Safe(name);
}

void CWeaponHUD::animDisplay		(MotionID M,	BOOL bMixIn)
{
	if(m_bCurrentEntityIsParent)
	{
		CSkeletonAnimated* pSkeletonAnimated			= smart_cast<CSkeletonAnimated*>(Visual());
		VERIFY(pSkeletonAnimated);
		//pSkeletonAnimated->Update						();
		pSkeletonAnimated->PlayCycle					(M,bMixIn);
		pSkeletonAnimated->CalculateBones_Invalidate	();
	}
}
void CWeaponHUD::animPlay			(MotionID M,	BOOL bMixIn, CInventoryItem* W)
{
	Show				();

	animDisplay			(M, bMixIn);

	CSkeletonAnimated	*skeleton_animated = smart_cast<CSkeletonAnimated*>(Visual());
	VERIFY				(skeleton_animated);
	CMotionDef			*motion_def = skeleton_animated->LL_GetMotionDef(M);
	VERIFY				(motion_def);

	if (motion_def->flags & esmStopAtEnd) {
		//���� ���������� �������� ��� �� ����������, �� ���������� ��
		m_bStopAtEndAnimIsRunning = true;

		CBoneData			&bone_data = smart_cast<CKinematics*>(Visual())->LL_GetData(smart_cast<CKinematics*>(Visual())->LL_GetBoneRoot());
		CBoneDataAnimated	*bone_anim = smart_cast<CBoneDataAnimated *>(&bone_data);
		CMotion				&motion = bone_anim->Motions[M.slot]->at(M.idx);
		u32					anim_time = iFloor(0.5f + 1000.f*motion.GetLength()/ motion_def->Dequantize(motion_def->speed));
		m_pCallbackItem		= W;
		m_dwAnimEndTime		= Device.dwTimeGlobal + anim_time;
	}
	else
		m_pCallbackItem		= NULL;
}

void CWeaponHUD::UpdateHud		()
{
	if(m_bStopAtEndAnimIsRunning && Device.dwTimeGlobal > m_dwAnimEndTime)
	{
		StopCurrentAnim	();
	}

	if(m_bCurrentEntityIsParent)
		smart_cast<CSkeletonAnimated*>(Visual())->Update	();
}

void CWeaponHUD::StopCurrentAnim	()
{
	m_dwAnimEndTime = 0;
	m_bStopAtEndAnimIsRunning = false;
	
	//Msg("stop anim ,callback item [%s],id [%d]",
	//	m_pCallbackItem ? (*m_pCallbackItem->object().cName()): "no callback item",
	//	m_pCallbackItem ? (m_pCallbackItem->object().ID()): 0
	//	);
	
	if(m_pCallbackItem)
	{
		m_pCallbackItem->OnAnimationEnd();
	}
}

void CWeaponHUD::StopCurrentAnimWithoutCallback		()
{
	m_dwAnimEndTime = 0;
	m_bStopAtEndAnimIsRunning = false;

	m_pCallbackItem = NULL;
}

///////////////////////////////////////////////////////////
// ��������� ��� �������� � ������������ ����� ���������� 
// � HUD
SHARED_HUD_INFO*  CWeaponHUD::GetHudInfo (shared_str section, CHudItem* pHudItem)
{
	SHARED_HUD_INFO* hud_info = NULL;

	SHARED_HUD_INFO_MAP_IT it = m_SharedHudInfoMap.find(section);

	//������ ��� ��������� � ������
	if(it != m_SharedHudInfoMap.end())
	{	
		hud_info = it->second;
		hud_info->AddRef();
	}
	//������ ��� ���, ������������������� �� � ���������
	else
	{
		hud_info = xr_new<SHARED_HUD_INFO>(*section, pHudItem);
		m_SharedHudInfoMap[section] = hud_info;
		hud_info->AddRef();

	}

	return hud_info;
}

void CWeaponHUD::ReleaseHudInfo		(shared_str section)
{
	SHARED_HUD_INFO_MAP_IT it = m_SharedHudInfoMap.find(section);

	if(it == m_SharedHudInfoMap.end())
		return;

	SHARED_HUD_INFO* hud_info = it->second;
	hud_info->ReleaseRef();

	if(hud_info->GetRefCount()==0)
	{
		xr_delete(hud_info);
		m_SharedHudInfoMap.erase(it);
	}
}
