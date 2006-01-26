// Actor.cpp: implementation of the CSpectator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "spectator.h"
#include "effectorfall.h"
#include "CameraLook.h"
#include "CameraFirstEye.h"
#include "actor.h"
#include "hudmanager.h"
#include "xrServer_Objects.h"
#include "game_cl_base.h"
#include "level.h"
#include "xr_level_controller.h"
#include "seniority_hierarchy_holder.h"
#include "team_hierarchy_holder.h"
#include "squad_hierarchy_holder.h"
#include "group_hierarchy_holder.h"
#include "../CameraManager.h"
#include "Inventory.h"
#include "huditem.h"
#include "clsid_game.h"
#include "game_cl_mp.h"

//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSpectator::CSpectator() : CGameObject()
{
	// Cameras
	cameras[eacFirstEye]	= xr_new<CCameraFirstEye>	(this, pSettings, "actor_firsteye_cam", false);
	cameras[eacLookAt]		= xr_new<CCameraLook>		(this, pSettings, "actor_look_cam",		false);
	cameras[eacFreeLook]	= xr_new<CCameraLook>		(this, pSettings, "actor_free_cam",	false);
	cameras[eacFreeFly]		= xr_new<CCameraFirstEye>	(this, pSettings, "actor_firsteye_cam", false);

//	cam_active				= eacFreeFly;
	cam_active				= eacFreeLook;
	look_idx				= 0;
	m_pActorToLookAt			= NULL;
}

CSpectator::~CSpectator()
{
	for (int i=0; i<eacMaxCam; ++i) xr_delete(cameras[i]);
}

void CSpectator::UpdateCL()
{
	inherited::UpdateCL();

	if (GameID() != GAME_SINGLE)
	{
		if (Game().local_player && Game().local_player->GameID == ID())
		{
			if (cam_active != eacFreeFly)
			{
				if (m_pActorToLookAt && !m_pActorToLookAt->g_Alive())
					cam_Set(eacFreeLook);
				if (!m_pActorToLookAt)
				{
					SelectNextPlayerToLook();
					if (!m_pActorToLookAt)
						cam_Set(eacFreeFly);
				};
			}
			if (Level().CurrentViewEntity() == this) cam_Update(m_pActorToLookAt);
			return;
		}		
		
	};
	
	if (g_pGameLevel->CurrentViewEntity()==this){
		if (eacFreeFly!=cam_active){
			//-------------------------------------
			
			//-------------------------------------
			int idx			= 0;
			game_PlayerState* P = Game().local_player;
			if (P&&(P->team>=0)&&(P->team<(int)Level().seniority_holder().teams().size())){
				const CTeamHierarchyHolder& T		= Level().seniority_holder().team(P->team);
				for (u32 i=0; i<T.squads().size(); ++i){
					const CSquadHierarchyHolder& S = T.squad(i);
					for (u32 j=0; j<S.groups().size(); ++j){
						const CGroupHierarchyHolder& G = S.group(j);
						for (u32 k=0; k<G.members().size(); ++k){
							CActor* A = smart_cast<CActor*>(G.members()[k]);
							if (A/*&&A->g_Alive()*/){
								if(idx==look_idx){
									cam_Update	(A);
									return;
								}
								++idx;
							}
						}
					}
				}
			}
			// �� ������ ������ � ����� �������� - ������� �� ������ ������
			look_idx = 0;
			// ������ ��� �� ��� �������� - ������������ �� 
			if (0==idx) cam_Set(eacFreeFly);
		}
		// �� ��������� eacFreeFly
		cam_Update		(0);
	}
}

void CSpectator::shedule_Update		(u32 DT)
{
	inherited::shedule_Update	(DT);
//	if (!getEnabled())	return;
	if (!Ready())		return;
}

#define START_ACCEL		16.0f
static float Accel_mul = START_ACCEL;

void CSpectator::IR_OnKeyboardPress(int cmd)
{
	if (Remote())												return;


	switch(cmd) 
	{
	case kACCEL:
		{
			Accel_mul = START_ACCEL*2;
		}break;
	case kCAM_1:	
		{
			SelectNextPlayerToLook();
			if (m_pActorToLookAt)
				cam_Set			(eacFirstEye);
			else
				cam_Set			(eacFreeFly);			
		}break;
	case kCAM_2:	if (cam_active == eacFreeFly)	SelectNextPlayerToLook();	cam_Set			(eacLookAt);		break;
	case kCAM_3:	if (cam_active == eacFreeFly)	SelectNextPlayerToLook();	cam_Set			(eacFreeLook);		break;
	case kCAM_4:	cam_Set			(eacFreeFly);	m_pActorToLookAt = NULL;	break;
	case kWPN_FIRE:	
		{
			if (cam_active != eacFreeFly)
			{
				++look_idx;
				SelectNextPlayerToLook();
				if (cam_active == eacFirstEye && m_pActorToLookAt)
					FirstEye_ToPlayer(m_pActorToLookAt);
			}			
		}break;
	case kWPN_ZOOM:
		{
			game_cl_mp* pMPGame = smart_cast<game_cl_mp*> (&Game());
			if (!pMPGame) break;
			game_PlayerState* PS = Game().local_player;
			if (!PS || PS->GameID != ID()) break;

			EActorCameras new_camera = EActorCameras((cam_active+1)%eacMaxCam);
			
			if (!PS->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
			{
				while (!pMPGame->Is_Spectator_Camera_Allowed(new_camera) && new_camera != eacFreeFly)
				{
					new_camera = EActorCameras((new_camera+1)%eacMaxCam);
				}
			};
			
			if (new_camera == eacFreeFly)
			{
				cam_Set			(eacFreeFly);	
				m_pActorToLookAt = NULL;
			}
			else
			{
				if (!m_pActorToLookAt) SelectNextPlayerToLook();
				if (!m_pActorToLookAt)
					cam_Set			(eacFreeFly);	
				else
					cam_Set			(new_camera);	
			}
		}break;
	}
}

void CSpectator::IR_OnKeyboardRelease(int cmd)
{
	switch (cmd)
	{
	case kACCEL:
		{
			Accel_mul = START_ACCEL;
		}break;
	}
}


void CSpectator::IR_OnKeyboardHold(int cmd)
{
	if (Remote())		return;

	game_cl_mp* pMPGame = smart_cast<game_cl_mp*> (&Game());
	game_PlayerState* PS = Game().local_player;

	if ((cam_active==eacFreeFly)||(cam_active==eacFreeLook)){
		CCameraBase* C	= cameras	[cam_active];
		Fvector vmove={0,0,0};
		switch(cmd){
		case kUP:
		case kDOWN: 
		case kCAM_ZOOM_IN: 
		case kCAM_ZOOM_OUT: 
			cameras[cam_active]->Move(cmd); break;
		case kLEFT:
		case kRIGHT:
			if (eacFreeLook!=cam_active) cameras[cam_active]->Move(cmd); break;
		case kFWD:			
			vmove.mad( C->vDirection, Device.fTimeDelta*Accel_mul );
			break;
		case kBACK:
			vmove.mad( C->vDirection, -Device.fTimeDelta*Accel_mul );
			break;
		case kR_STRAFE:{
			Fvector right;
			right.crossproduct(C->vNormal,C->vDirection);
			vmove.mad( right, Device.fTimeDelta*Accel_mul );
			}break;
		case kL_STRAFE:{
			Fvector right;
			right.crossproduct(C->vNormal,C->vDirection);
			vmove.mad( right, -Device.fTimeDelta*Accel_mul );
			}break;
		}
		if (cam_active != eacFreeFly || (pMPGame->Is_Spectator_Camera_Allowed(eacFreeFly) || (PS && PS->testFlag(GAME_PLAYER_FLAG_SPECTATOR))))
			XFORM().c.add( vmove );
	}
}

void CSpectator::IR_OnMouseMove(int dx, int dy)
{
	if (Remote())	return;

	CCameraBase* C	= cameras	[cam_active];
	float scale		= (C->f_fov/DEFAULT_FOV)*psMouseSens * psMouseSensScale/50.f;
	if (dx){
		float d = float(dx)*scale;
		cameras[cam_active]->Move((d<0)?kLEFT:kRIGHT, _abs(d));
	}
	if (dy){
		float d = ((psMouseInvert.test(1))?-1:1)*float(dy)*scale*3.f/4.f;
		cameras[cam_active]->Move((d>0)?kUP:kDOWN, _abs(d));
	}
}

void CSpectator::FirstEye_ToPlayer(CObject* pObject)
{
	CObject* pCurViewEntity = Level().CurrentEntity();
	if (pCurViewEntity)
	{
		CActor* pOldActor = smart_cast<CActor*>(pCurViewEntity);
		if (pOldActor)
		{
			pOldActor->inventory().Items_SetCurrentEntityHud(false);
		};
		if (pCurViewEntity->CLS_ID != CLSID_SPECTATOR)
		{
			Engine.Sheduler.Unregister	(pCurViewEntity);
			Engine.Sheduler.Register	(pCurViewEntity);
		};
	};
	if (pObject)
	{
		Level().SetEntity(pObject);

		Engine.Sheduler.Unregister	(pObject);
		Engine.Sheduler.Register	(pObject, TRUE);

		CActor* pActor = smart_cast<CActor*> (pObject);
		if (pActor)
		{
			pActor->inventory().Items_SetCurrentEntityHud(true);

			CHudItem* pHudItem = smart_cast<CHudItem*>(pActor->inventory().ActiveItem());
			if (pHudItem) 
			{
				pHudItem->OnStateSwitch(pHudItem->State());
			}
		}
	};
};

void CSpectator::cam_Set	(EActorCameras style)
{
	CCameraBase* old_cam = cameras[cam_active];
	//-----------------------------------------------
	if (style == eacFirstEye)
	{
		FirstEye_ToPlayer(m_pActorToLookAt);		
	};
	if (cam_active == eacFirstEye)
	{
		FirstEye_ToPlayer(this);
	};
	//-----------------------------------------------
	cam_active = style;
	old_cam->OnDeactivate();
	cameras[cam_active]->OnActivate(old_cam);
	
}

void CSpectator::cam_Update	(CActor* A)
{
	HUD().Font().pFontDI->SetColor			(0xF0808080);
	HUD().Font().pFontDI->SetSizeI			(0.03f);
	HUD().Font().pFontDI->SetAligment		(CGameFont::alCenter);
	if (A){
		const Fmatrix& M			= A->XFORM();
		CCameraBase* pACam			= A->cam_Active();
		CCameraBase* cam			= cameras[cam_active];
		switch(cam_active) {
		case eacFirstEye:{
//			Fvector P;
//			P.add					(M.c,1.6f);
//			cam->Set				(P,M.k,M.j);
			Fvector P, D, N;
			pACam->Get				(P, D, N);
			cam->Set				(P, D, N);
			}break;
		case eacLookAt:{
			float y,p,r;
			M.getHPB				(y,p,r);
			cam->Set				(pACam->yaw,pACam->pitch,-r);
			}
		case eacFreeLook:{
			cam->SetParent			(A);
			Fmatrix tmp; tmp.identity();
			
			Fvector point, point1, dangle;
			point.set	(0.f,1.6f,0.f);
			point1.set	(0.f,1.6f,0.f);			
			M.transform_tiny		(point);
			tmp.translate_over(point);
			tmp.transform_tiny		(point1);
			if (!A->g_Alive()) point.set(point1);
//			if(!A->g_Alive()) point.set	(A->Position());
			cam->Update				(point,dangle);
			}break;
		}
		//-----------------------------------
		Fvector P, D, N;
		cam->Get(P, D, N);
		cameras[eacFreeFly]->Set(P, D, N);
		cameras[eacFreeFly]->Set(cam->yaw, cam->pitch, 0);
		P.y -= 1.6f;
		XFORM().translate_over(P);
		//-----------------------------------
		g_pGameLevel->Cameras().Update(cam);
	}else{
		CCameraBase* cam			= cameras[eacFreeFly];
		Fvector point, dangle;
		point.set					(0.f,1.6f,0.f);
		XFORM().transform_tiny	(point);

		// apply shift
		dangle.set					(0,0,0);
		
		cam->Update					(point,dangle);
//		cam->vPosition.set(point0);
		g_pGameLevel->Cameras().Update	(cam);
		// hud output
	};
}

BOOL			CSpectator::net_Spawn				( CSE_Abstract*	DC )
{
	BOOL res = inherited::net_Spawn(DC);
	if (!res) return FALSE;

	CSE_Abstract			*E	= (CSE_Abstract*)(DC);
	if (!E) return FALSE;

	cam_active				= eacFreeFly;
	look_idx				= 0;

	cameras[cam_active]->Set		(-E->o_Angle.y,-E->o_Angle.x,0);		// set's camera orientation
	cameras[cam_active]->vPosition.set(E->o_Position);

	if (OnServer())
	{
		E->s_flags.set(M_SPAWN_OBJECT_LOCAL, TRUE);
	};
	return TRUE;
};

void			CSpectator::SelectNextPlayerToLook	()
{
	if (GameID() == GAME_SINGLE) return;
	
	game_PlayerState* PS = Game().local_player;
	if (!PS) return;
	m_pActorToLookAt = NULL;

	game_cl_mp* pMPGame = smart_cast<game_cl_mp*> (&Game());

	game_cl_GameState::PLAYERS_MAP_IT it = Game().players.begin();
	u16 PPCount = 0;
	CActor*	PossiblePlayers[32];
	for(;it!=Game().players.end();++it)
	{
		game_PlayerState* ps = it->second;
		if (!ps || ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD) || ps==PS) continue;
		if (pMPGame && pMPGame->Is_Spectator_TeamCamera_Allowed())
		{
			if (ps->team != PS->team && !PS->testFlag(GAME_PLAYER_FLAG_SPECTATOR)) continue;
		};
		u16 id = ps->GameID;
		CObject* pObject = Level().Objects.net_Find(id);
		if (!pObject) continue;
		CActor* A = smart_cast<CActor*>(pObject);
		if (!A) continue;
		PossiblePlayers[PPCount++] = A;
	};
	if (PPCount > 0)
	{
		look_idx %= PPCount;
		m_pActorToLookAt = PossiblePlayers[look_idx];
	};

};

void			CSpectator::net_Relcase				(CObject *O)
{
	if (O != m_pActorToLookAt) return;
	m_pActorToLookAt = NULL;
	if (cam_active != eacFreeFly) SelectNextPlayerToLook();
	if (!m_pActorToLookAt) cam_Set(eacFreeFly);
};

void			CSpectator::GetSpectatorString		(LPTSTR pStr)
{
	if (!pStr) return;
	if (GameID() == GAME_SINGLE) return;
	
	xr_string	SpectatorMsg;
	switch (cam_active)
	{
	case eacFreeFly:
		{
			SpectatorMsg = "SPECTATOR (Free-fly)";
		}break;
	case eacFirstEye:
		{
			SpectatorMsg = "SPECTATOR (First-Eye): ";
			SpectatorMsg += m_pActorToLookAt->Name();			

		}break;
	case eacFreeLook:
		{
			SpectatorMsg = "SPECTATOR (Free-Look):";
			SpectatorMsg += m_pActorToLookAt->Name();
		}break;
	case eacLookAt:
		{
			SpectatorMsg = "SPECTATOR (Look-At):";
			SpectatorMsg += m_pActorToLookAt->Name();
		}break;
	};
	strcpy(pStr, SpectatorMsg.c_str());
};