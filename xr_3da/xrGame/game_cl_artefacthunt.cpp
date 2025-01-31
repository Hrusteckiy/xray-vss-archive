#include "stdafx.h"
#include "game_cl_artefacthunt.h"
#include "xrMessages.h"
#include "hudmanager.h"
#include "level.h"
#include "UIGameAHunt.h"
#include "clsid_game.h"
#include "map_manager.h"
#include "LevelGameDef.h"
#include "actor.h"
#include "ui/UIMainIngameWnd.h"
#include "xr_level_controller.h"


#define TEAM0_MENU		"artefacthunt_team0"
#define	TEAM1_MENU		"artefacthunt_team1"
#define	TEAM2_MENU		"artefacthunt_team2"

game_cl_ArtefactHunt::game_cl_ArtefactHunt()
{
	m_game_ui = NULL;
	pMessageSounds[0].create(TRUE, "messages\\multiplayer\\mp_artifact_lost");
	pMessageSounds[1].create(TRUE, "messages\\multiplayer\\mp_artifact_on_base");
	pMessageSounds[2].create(TRUE, "messages\\multiplayer\\mp_artifact_on_base_radio");
	pMessageSounds[3].create(TRUE, "messages\\multiplayer\\mp_got_artifact");
	pMessageSounds[4].create(TRUE, "messages\\multiplayer\\mp_got_artifact_radio");
	pMessageSounds[5].create(TRUE, "messages\\multiplayer\\mp_new_artifact");

	pMessageSounds[6].create(TRUE, "messages\\multiplayer\\mp_artifact_delivered_by_enemy");
	pMessageSounds[7].create(TRUE, "messages\\multiplayer\\mp_artifact_stolen");
	
	m_bBuyEnabled	= FALSE;
}

void game_cl_ArtefactHunt::Init ()
{
//	string64	Team1, Team2;
//	std::strcpy(Team1, TEAM1_MENU);
//	std::strcpy(Team2, TEAM2_MENU);
//	LoadTeamData(TEAM0_MENU);
	LoadTeamData(TEAM1_MENU);
	LoadTeamData(TEAM2_MENU);

	old_artefactBearerID = 0;
	old_artefactID = 0;
	old_teamInPossession = 0;
	//---------------------------------------------------
	//---------------------------------------------------------
	string256	fn_game;
	if (FS.exist(fn_game, "$level$", "level.game")) 
	{
		IReader *F = FS.r_open	(fn_game);
		IReader *O = 0;

		// Load RPoints
		if (0!=(O = F->open_chunk	(RPOINT_CHUNK)))
		{ 
			for (int id=0; O->find_chunk(id); ++id)
			{
				RPoint					R;
				u8						RP_team;
				u8						RP_type;
				u8						RP_GameType;

				O->r_fvector3			(R.P);
				O->r_fvector3			(R.A);
				RP_team					= O->r_u8	();	VERIFY(RP_team>=0 && RP_team<4);
				RP_type					= O->r_u8	();
				RP_GameType				= O->r_u8	();
				//u16 res					= 
				O->r_u8	();

				if (RP_GameType != rpgtGameAny && RP_GameType != rpgtGameArtefactHunt)
				{
					continue;					
				};
				switch (RP_type)
				{
				case rptTeamBaseParticle:
					{
						string256 ParticleStr;
						sprintf(ParticleStr, "teambase_particle_%d", RP_team);
						if (pSettings->line_exist("artefacthunt_gamedata", ParticleStr))
						{
							Fmatrix			transform;
							transform.identity();
							transform.setXYZ(R.A);
							transform.translate_over(R.P);
							CParticlesObject* pStaticParticles			= xr_new<CParticlesObject>	(pSettings->r_string("artefacthunt_gamedata", ParticleStr),FALSE);
							pStaticParticles->UpdateParent	(transform,zero_vel);
							pStaticParticles->Play			();
							Level().m_StaticParticles.push_back		(pStaticParticles);
						};
					}break;
				};
			};
			O->close();
		}

		FS.r_close	(F);
	}
};
game_cl_ArtefactHunt::~game_cl_ArtefactHunt()
{
	pMessageSounds[0].destroy();
	pMessageSounds[1].destroy();
	pMessageSounds[2].destroy();
	pMessageSounds[3].destroy();
	pMessageSounds[4].destroy();
	pMessageSounds[5].destroy();
	pMessageSounds[6].destroy();
	pMessageSounds[7].destroy();
}


BOOL	bBearerCantSprint = TRUE;
void game_cl_ArtefactHunt::net_import_state	(NET_Packet& P)
{
	inherited::net_import_state	(P);
	
	P.r_u8	(artefactsNum);
	P.r_u16	(artefactBearerID);
	P.r_u8	(teamInPossession);
	P.r_u16	(artefactID);
	bBearerCantSprint = !!P.r_u8();

	if (P.r_u8() != 0)
	{
		P.r_s32	(dReinforcementTime);
		dReinforcementTime += Level().timeServer();
	}
	else
		dReinforcementTime = 0;
}


void game_cl_ArtefactHunt::TranslateGameMessage	(u32 msg, NET_Packet& P)
{
	string512 Text;
	LPSTR	Color_Teams[3]		= {"%c255,255,255", "%c64,255,64", "%c64,64,255"};
	char	Color_Main[]		= "%c192,192,192";
	char	Color_Artefact[]	= "%c255,255,0";
	LPSTR	TeamsNames[3]		= {"Zero Team", "Team Green", "Team Blue"};

	switch(msg)	{
//-------------------UI MESSAGES
	case GAME_EVENT_ARTEFACT_TAKEN: //ahunt
		{
			u16 PlayerID, Team;
			P.r_u16 (PlayerID);
			P.r_u16 (Team);

			game_PlayerState* pPlayer = GetPlayerByGameID(PlayerID);
			if (!pPlayer) break;

			sprintf(Text, "%s%s %shas taken the %sArtefact", 
				Color_Teams[Team], 
				pPlayer->name, 
				Color_Main,
				Color_Artefact);
			CommonMessageOut(Text);
						
			CActor* pActor = smart_cast<CActor*> (Level().CurrentEntity());
			if (!pActor) break;
			if (pActor->ID() == PlayerID)
				pMessageSounds[3].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
			else
				if (pActor->g_Team() == Team)
					pMessageSounds[4].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
				else
					pMessageSounds[7].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
		}break;
	case GAME_EVENT_ARTEFACT_DROPPED: //ahunt
		{
			u16 PlayerID, Team;
			P.r_u16 (PlayerID);
			P.r_u16 (Team);

			game_PlayerState* pPlayer = GetPlayerByGameID(PlayerID);
			if (!pPlayer) break;

			sprintf(Text, "%s%s %shas dropped the %sArtefact", 
				Color_Teams[Team], 
				pPlayer->name, 
				Color_Main,
				Color_Artefact);
			CommonMessageOut(Text);

			pMessageSounds[0].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
		}break;
	case GAME_EVENT_ARTEFACT_ONBASE: //ahunt
		{
			u16 PlayerID, Team;
			P.r_u16 (PlayerID);
			P.r_u16 (Team);

			game_PlayerState* pPlayer = GetPlayerByGameID(PlayerID);
			if (!pPlayer) break;

			sprintf(Text, "%s%s %sscores", 
				Color_Teams[Team], 
				TeamsNames[Team], 
				Color_Main);
			CommonMessageOut(Text);

			CActor* pActor = smart_cast<CActor*> (Level().CurrentEntity());
			if (!pActor) break;
			if (pActor->ID() == PlayerID)
				pMessageSounds[1].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
			else
				if (pActor->g_Team() == Team)
					pMessageSounds[2].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
				else
					pMessageSounds[6].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
/*
			if (Level().CurrentEntity() && Level().CurrentEntity()->ID() == PlayerID)
				pMessageSounds[1].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
			else
				pMessageSounds[2].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
*/
		}break;
	case GAME_EVENT_ARTEFACT_SPAWNED: //ahunt
		{
			sprintf(Text, "%sArtefact has been spawned. Bring it to your base to score.", 
				Color_Main);
			CommonMessageOut(Text);

			pMessageSounds[5].play_at_pos(NULL, Device.vCameraPosition, sm_2D, 0);
		}break;
	case GAME_EVENT_ARTEFACT_DESTROYED:  //ahunt
		{
			sprintf(Text, "%sArtefact has been destroyed.", 
				Color_Main);
			CommonMessageOut(Text);

		}break;

//-----GAME MESSAGES
		/*
	case GAME_EVENT_PLAYER_ENTER_TEAM_BASE:
		{
			u16 player_id;
			u8  base_team_id;
			P.r_u16(player_id);
			P.r_u8(base_team_id);
			OnObjectEnterTeamBase(player_id, base_team_id);

			if(OnServer()){
				NET_Packet			P_;
				sv_GameEventGen		(P_);
				P_.w_u16			(GAME_EVENT_PLAYER_ENTER_TEAM_BASE);
				P_.w_u16			(player_id);
				P_.w_u8				(base_team_id);
				sv_EventSend		(P_);
			};

		}break;
	case GAME_EVENT_PLAYER_LEAVE_TEAM_BASE:
		{
			u16 player_id;
			u8  base_team_id;
			P.r_u16(player_id);
			P.r_u8(base_team_id);
			OnObjectLeaveTeamBase(player_id, base_team_id);

			if(OnServer()){
				NET_Packet			P_;
				sv_GameEventGen		(P_);
				P_.w_u16			(GAME_EVENT_PLAYER_LEAVE_TEAM_BASE);
				P_.w_u16			(player_id);
				P_.w_u8				(base_team_id);
				sv_EventSend		(P_);
			};
		}break;
		*/

	default:
		inherited::TranslateGameMessage(msg,P);
	}
}

CUIGameCustom* game_cl_ArtefactHunt::createGameUI()
{
	game_cl_mp::createGameUI();

	CLASS_ID clsid			= CLSID_GAME_UI_ARTEFACTHUNT;
	m_game_ui	= smart_cast<CUIGameAHunt*> ( NEW_INSTANCE ( clsid ) );
	R_ASSERT(m_game_ui);
	m_game_ui->SetClGame(this);
	m_game_ui->Init();

	//----------------------------------------------------------------
	pBuyMenuTeam1 = InitBuyMenu("artefacthunt_base_cost", 1);
	pBuyMenuTeam2 = InitBuyMenu("artefacthunt_base_cost", 2);
	//----------------------------------------------------------------
	pSkinMenuTeam1 = InitSkinMenu(1);
	pSkinMenuTeam2 = InitSkinMenu(2);

	pInventoryMenu = xr_new<CUIInventoryWnd>();

	return m_game_ui;
}

void game_cl_ArtefactHunt::GetMapEntities(xr_vector<SZoneMapEntityData>& dst)
{
	inherited::GetMapEntities(dst);

	SZoneMapEntityData D;
	u32 color_enemy_with_artefact		=		0xffff0000;
	u32 color_artefact					=		0xffffffff;
	u32 color_friend_with_artefact		=		0xffffff00;


	s16 local_team						=		local_player->team;


	CObject* pObject = Level().Objects.net_Find(artefactID);
	if(!pObject)
		return;

	CArtefact* pArtefact = smart_cast<CArtefact*>(pObject);
	VERIFY(pArtefact);

	CObject* pParent = pArtefact->H_Parent();
	if(!pParent){// Artefact alone
		D.color	= color_artefact;
		D.pos	= pArtefact->Position();
		dst.push_back(D);
		return;
	};

	if (pParent && pParent->ID() == artefactBearerID && GetPlayerByGameID(artefactBearerID)){
		CObject* pBearer = Level().Objects.net_Find(artefactBearerID);
		VERIFY(pBearer);
		D.pos	= pBearer->Position();

		game_PlayerState*	ps  =	GetPlayerByGameID		(artefactBearerID);
		(ps->team==local_team)? D.color=color_friend_with_artefact:D.color=color_enemy_with_artefact;
		
		//remove previous record about this actor !!!
		dst.push_back(D);
		return;
	}

}

void game_cl_ArtefactHunt::shedule_Update			(u32 dt)
{
	//out game information
	m_game_ui->SetReinforcementCaption("");
	m_game_ui->SetBuyMsgCaption		("");
	m_game_ui->SetScoreCaption		("");
	m_game_ui->SetTodoCaption		("");
	m_game_ui->SetPressBuyMsgCaption	("");

	inherited::shedule_Update		(dt);

	switch (phase)
	{
	case GAME_PHASE_INPROGRESS:
		{
//			HUD().GetUI()->ShowIndicators();
			if (local_player)
			{
				if (local_player->testFlag(GAME_PLAYER_FLAG_ONBASE))
				{
					m_bBuyEnabled = TRUE;
				}
				else
				{
					m_bBuyEnabled = FALSE;
				};
			};

			if (m_bBuyEnabled)
			{
				if (local_player && Level().CurrentControlEntity() && Level().CurrentControlEntity()->CLS_ID == CLSID_OBJECT_ACTOR/*pCurActor && pCurActor->g_Alive() && !m_gameUI->pCurBuyMenu->IsShown()*/ )
				{
					if (!(pCurBuyMenu && pCurBuyMenu->IsShown()) && 
						!(pCurSkinMenu && pCurSkinMenu->IsShown()))
					{					
						m_game_ui->SetBuyMsgCaption("Press B to access Buy Menu");
					};
				};
			};

			if (local_player/*pCurActor && !m_game_ui->pCurBuyMenu->IsShown()*/)
			{
				game_TeamState team0 = teams[0];
				game_TeamState team1 = teams[1];

				string256 S;
				
				if (dReinforcementTime != 0 && Level().CurrentEntity())
				{
					u32 CurTime = Level().timeServer();
					u32 dTime;
					if (s32(CurTime) > dReinforcementTime) dTime = 0;
					else dTime = iCeil(float(dReinforcementTime - CurTime) / 1000);

					sprintf(S,		"Next reinforcement will arrive at . . .%d", dTime);
					 
					CActor* pActor = NULL;
					if (Level().CurrentEntity()->CLS_ID == CLSID_OBJECT_ACTOR)
						pActor = smart_cast<CActor*>(Level().CurrentEntity());

					if (Level().CurrentEntity()->CLS_ID == CLSID_SPECTATOR || 
						(pActor && !pActor->g_Alive()))
							m_game_ui->SetReinforcementCaption(S);
					else
						m_game_ui->SetReinforcementCaption("");

				};

				s16 lt = local_player->team;
				if (lt>=0)
				{
					sprintf(S,		"Your Team : %3d - Enemy Team %3d - from %3d Artefacts",
									teams[lt-1].score, 
									teams[(lt==1)?1:0].score, 
									artefactsNum);
					m_game_ui->SetScoreCaption(S);
				};
	/*
			if ( (artefactBearerID==0))// && (artefactID!=0) )
				{
					m_game_ui->SetTodoCaption("Grab the Artefact");
				}
				else
				{
					if (teamInPossession != local_player->team )
					{
						m_game_ui->SetTodoCaption("Stop ArtefactBearer");
					}
					else
					{
						if (local_player->GameID == artefactBearerID)
						{
							m_game_ui->SetTodoCaption("You got the Artefact. Bring it to your base.");
						}
						else
						{
							m_game_ui->SetTodoCaption("Protect your ArtefactBearer");
						};
					};
				};
			*/
			};
			
		}break;
	case GAME_PHASE_TEAM1_ELIMINATED:
		{
//			HUD().GetUI()->HideIndicators();
//			GetUICursor()->Hide();
			m_game_ui->SetRoundResultCaption("Team Green ELIMINATED!");
		}break;
	case GAME_PHASE_TEAM2_ELIMINATED:
		{
//			HUD().GetUI()->HideIndicators();
//			GetUICursor()->Hide();
			m_game_ui->SetRoundResultCaption("Team Blue ELIMINATED!");
		}break;
	default:
		{
			
		}break;
	};

}

BOOL game_cl_ArtefactHunt::CanCallBuyMenu			()
{
	if (!m_bBuyEnabled) return FALSE;
	if (Phase()!=GAME_PHASE_INPROGRESS) return false;
	
	if (pUITeamSelectWnd && pUITeamSelectWnd->IsShown())
	{
		return FALSE;
	};
	if (pCurSkinMenu && pCurSkinMenu->IsShown())
	{
		return FALSE;
	};
	if (pInventoryMenu && pInventoryMenu->IsShown())
	{
		return FALSE;
	};

	CActor* pCurActor = smart_cast<CActor*> (Level().CurrentEntity());
	if (!pCurActor || !pCurActor->g_Alive()) return FALSE;

	return TRUE;
};

bool game_cl_ArtefactHunt::CanBeReady				()
{
	if (!local_player) return false;
	SetCurrentSkinMenu();
	SetCurrentBuyMenu();

	if (!m_bTeamSelected)
	{
		if (CanCallTeamSelectMenu())
			StartStopMenu(pUITeamSelectWnd,true);
		return false;
	};

	if (pCurBuyMenu && !pCurBuyMenu->IsShown())
		ClearBuyMenu();

	return true;
};

///-------------------------------------------------------------------
/*
void game_cl_ArtefactHunt::OnObjectEnterTeamBase	(u16 player_id, u8 zone_team_id)
{
	game_PlayerState*	pl = GetPlayerByGameID (player_id);
	if(!pl || !Level().CurrentEntity() )
		return;

	if (pl->GameID == Level().CurrentEntity()->ID() && pl->team == zone_team_id)
	{
//		m_bBuyEnabled = TRUE;
	};

};

void game_cl_ArtefactHunt::OnObjectLeaveTeamBase	(u16 player_id, u8 zone_team_id)
{
	game_PlayerState*	pl = GetPlayerByGameID (player_id);
	if(!pl || !Level().CurrentEntity() )
		return;

	if (pl->GameID == Level().CurrentEntity()->ID() && pl->team == zone_team_id)
	{
//		m_bBuyEnabled = FALSE;
	};
};
*/
char*	game_cl_ArtefactHunt::getTeamSection(int Team)
{
	switch (Team)
	{
	case 1:
		{
			return "artefacthunt_team1";
		}break;
	case 2:
		{
			return "artefacthunt_team2";
		}break;
	default:
		NODEFAULT;
	};
#ifdef DEBUG
	return NULL;
#endif
};

bool	game_cl_ArtefactHunt::PlayerCanSprint			(CActor* pActor)
{
	if (artefactBearerID == 0) return true;
	if (bBearerCantSprint && pActor->ID() == artefactBearerID) return false;
	return true;
};

#define ARTEFACT_NEUTRAL "mp_af_neutral_location"
#define ARTEFACT_FRIEND "mp_af_friend_location"
#define ARTEFACT_ENEMY "mp_af_enemy_location"

void	game_cl_ArtefactHunt::UpdateMapLocations		()
{
	inherited::UpdateMapLocations();

	if (local_player)
	{
		if (!artefactID)
		{
			if (old_artefactID)
				Level().MapManager().RemoveMapLocationByObjectID(old_artefactID);
		}
		else
		{
			if (!artefactBearerID)
			{
				if (!Level().MapManager().HasMapLocation(ARTEFACT_NEUTRAL, artefactID))
				{
					Level().MapManager().RemoveMapLocationByObjectID(artefactID);
					Level().MapManager().AddMapLocation(ARTEFACT_NEUTRAL, artefactID);
				};
			}
			else
			{
				if (teamInPossession == local_player->team)
				{
					if (!Level().MapManager().HasMapLocation(ARTEFACT_FRIEND, artefactID))
					{
						Level().MapManager().RemoveMapLocationByObjectID(artefactID);
						Level().MapManager().AddMapLocation(ARTEFACT_FRIEND, artefactID);
					}
				}
				else
				{
					if (!Level().MapManager().HasMapLocation(ARTEFACT_ENEMY, artefactID))
					{
						Level().MapManager().RemoveMapLocationByObjectID(artefactID);
						Level().MapManager().AddMapLocation(ARTEFACT_ENEMY, artefactID);
					}
				}
			};
/*
			if (old_artefactBearerID != artefactBearerID || old_teamInPossession != teamInPossession)
			{
				Level().MapManager().RemoveMapLocationByObjectID(old_artefactID);
				if (!artefactBearerID)
				{
					Level().MapManager().AddMapLocation(ARTEFACT_NEUTRAL, artefactID);
				}
				else
				{
					if (teamInPossession == local_player->team)
						Level().MapManager().AddMapLocation(ARTEFACT_FRIEND, artefactID);
					else
						Level().MapManager().AddMapLocation(ARTEFACT_ENEMY, artefactID);
				};
			};
			*/
		};
		old_artefactBearerID = artefactBearerID;
		old_artefactID = artefactID;
		old_teamInPossession = teamInPossession;
	};
};

bool	game_cl_ArtefactHunt::NeedToSendReady_Spectator			(int key, game_PlayerState* ps)
{
	return ( GAME_PHASE_PENDING	== Phase() && kWPN_FIRE == key) || 
		( (kWPN_FIRE == key || kJUMP == key) && GAME_PHASE_INPROGRESS	== Phase() && 
		CanBeReady());
}