// exxZERO Time Stamp AddIn. Document modified at : Thursday, March 07, 2002 14:14:04 , by user : Oles , from computer : OLES
// Level.cpp: implementation of the CLevel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Level.h"
#include "entity.h"

// events
#include "..\fdemorecord.h"
#include "..\fdemoplay.h"
#include "PHdynamicdata.h"
#include "Physics.h"

CPHWorld*	ph_world = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLevel::CLevel()
{
	Server						= NULL;

	eChangeRP					= Engine.Event.Handler_Attach	("LEVEL:ChangeRP",this);
	eDemoPlay					= Engine.Event.Handler_Attach	("LEVEL:PlayDEMO",this);
	eChangeTrack				= Engine.Event.Handler_Attach	("LEVEL:PlayMusic",this);
	eDoorOpen					= Engine.Event.Handler_Attach	("LEVEL:DoorOpen",this);
	eDoorClose					= Engine.Event.Handler_Attach	("LEVEL:DoorClose",this);
	eDA_PlayMotion				= Engine.Event.Handler_Attach	("LEVEL:DA_PlayMotion",this);
	eDA_StopMotion				= Engine.Event.Handler_Attach	("LEVEL:DA_StopMotion",this);
	eEnvironment				= Engine.Event.Handler_Attach	("LEVEL:Environment",this);

	eEntitySpawn				= Engine.Event.Handler_Attach	("LEVEL:spawn",this);
}

CLevel::~CLevel()
{
	Msg			("- Destroying level");

	Engine.Event.Handler_Detach(eEntitySpawn,	this);

	Engine.Event.Handler_Detach(eEnvironment,	this);
	Engine.Event.Handler_Detach(eDA_StopMotion,	this);
	Engine.Event.Handler_Detach(eDA_PlayMotion,	this);
	Engine.Event.Handler_Detach(eDoorClose,		this);
	Engine.Event.Handler_Detach(eDoorOpen,		this);
	Engine.Event.Handler_Detach(eChangeTrack,	this);
	Engine.Event.Handler_Detach(eDemoPlay,		this);
	Engine.Event.Handler_Detach(eChangeRP,		this);

	/**/
	for (int i=0; i<tpaPatrolPaths.size(); i++) {
		tpaPatrolPaths[i].tpaWayPoints.clear();
		tpaPatrolPaths[i].tpaWayLinks.clear();
	}
	//tpaPatrolPaths.clear();
	/**/
	if (ph_world)	ph_world->Destroy		();
	_DELETE			(ph_world);
}

// Game interface ////////////////////////////////////////////////////
void CLevel::g_cl_Spawn		(LPCSTR name, int rp, int team, int squad, int group)
{
	Fvector		dummyPos,dummyAngle; dummyPos.set(0,0,0); dummyAngle.set(0,0,0);
	NET_Packet	P;
	P.w_begin	(M_SPAWN);
	P.w_string	(name);
	P.w_string	("");
	P.w_u8		((rp>=0)?u8(rp):0xff);
	P.w_vec3	(dummyPos);
	P.w_vec3	(dummyAngle);
	P.w_u16		(0);		// srv-id	| by server
	P.w_u16		(M_SPAWN_OBJECT_ACTIVE  | M_SPAWN_OBJECT_LOCAL);
	P.w_u16		(3);		// data size
	P.w_u8		(team);
	P.w_u8		(squad);
	P.w_u8		(group);
	Send		(P,net_flags(TRUE));
}

void CLevel::g_sv_Spawn		(NET_Packet* Packet)
{
	// Begin analysis
	NET_Packet&	P = *Packet;
	u16			type;
	P.r_begin	(type);
	R_ASSERT	(type==M_SPAWN);

	// Read definition
	char		s_name[128],s_replace[128];
	u8			s_rp;
	u16			s_server_id,s_data_size,s_flags;
	Fvector		o_pos,o_angle;
	P.r_string	(s_name);
	P.r_string	(s_replace);
	P.r_u8		(s_rp);
	P.r_vec3	(o_pos);
	P.r_vec3	(o_angle);
	P.r_u16		(s_server_id);
	P.r_u16		(s_flags);
	P.r_u16		(s_data_size);

	// Real spawn
	CObject*	O = Objects.LoadOne	(s_name);
	if (O)	
	{
		O->cName_set		(s_name);
		O->cNameSect_set	(s_name);
		if (s_replace[0])	O->cName_set		(s_replace);
	}
	if (0==O || (!O->net_Spawn(s_flags&M_SPAWN_OBJECT_LOCAL, s_server_id, o_pos, o_angle, P, s_flags))) 
	{
		Objects.DestroyObject(O);
		Msg("! Failed to spawn entity '%s'",s_name);
	} else {
		if ((s_flags&M_SPAWN_OBJECT_LOCAL) && (0==CurrentEntity()))	SetEntity		(O);
		if (s_flags&M_SPAWN_OBJECT_ACTIVE)							O->OnActivate	( );
	}
}

int	CLevel::get_RPID(LPCSTR name)
{
	// Gain access to string
	LPCSTR	params = pLevel->ReadSTRING("respawn_point",name);
	if (0==params)	return -1;

	// Read data
	Fvector4	pos;
	int			team;
	sscanf		(params,"%f,%f,%f,%d,%f",&pos.x,&pos.y,&pos.z,&team,&pos.w); pos.y += 0.1f;

	// Search respawn point
	svector<Fvector4,maxRP>	&rp = Level().get_team(team).RespawnPoints;
	for (int i=0; i<int(rp.size()); i++)
		if (pos.similar(rp[i],EPS_L))	return i;
	return -1;
}

void CLevel::OnFrame	()
{
	// Client-message parsing
	Device.Statistic.netClient.Begin();
	ClientReceive		();
	Device.Statistic.netClient.End	();

	// Inherited update
	inherited::OnFrame	();

	// Physics
	Device.Statistic.Physics.Begin		();
	if (ph_world) ph_world->Step		(Device.fTimeDelta);
	Device.Statistic.Physics.End		();

	// Merge visibility data from all units in the team
	for (DWORD T=0; T<Teams.size(); T++)
	{
		CTeam&	TD		= Teams[T];
		objVisible& VIS	= TD.KnownEnemys;

		VIS.clear		();
		for (DWORD S=0; S<TD.Squads.size(); S++)
		{
			CSquad&	SD	= TD.Squads[S];
			if (SD.Leader)	{
				CEntityAlive* E	= dynamic_cast<CEntityAlive*>(SD.Leader);
				if (E)		E->GetVisible(VIS);
			}

			for (DWORD G=0; G<SD.Groups.size(); G++)
			{
				CGroup& GD = SD.Groups[G];
				for (DWORD M=0; M<GD.Members.size(); M++)
				{
					CEntityAlive* E	= dynamic_cast<CEntityAlive*>(GD.Members[M]);
					if (E)	E->GetVisible(VIS);
				}
			}
		}
	}

	// If we have enought bandwidth - replicate client data on to server
	Device.Statistic.netClient.Begin();
	ClientSend ();
	Device.Statistic.netClient.End	();

	// If server - perform server-update
	if (Server && CurrentEntity())
	{
		Device.Statistic.netServer.Begin();
		Server->Update	();
		Device.Statistic.netServer.End	();
		if (psDeviceFlags&rsStatistic)
		{
			const IServerStatistic* S = Server->GetStatistic();
			pApp->pFont->Size	(0.015f);
			pApp->pFont->OutSet	(0.0f,0.5f);
			pApp->pFont->Color	(D3DCOLOR_XRGB(0,255,0));
			pApp->pFont->OutNext("IN:  %4d/%4d (%2.1f%%)",	S->bytes_in_real,	S->bytes_in,	100.f*float(S->bytes_in_real)/float(S->bytes_in));
			pApp->pFont->OutNext("OUT: %4d/%4d (%2.1f%%)",	S->bytes_out_real,	S->bytes_out,	100.f*float(S->bytes_out_real)/float(S->bytes_out));
			pApp->pFont->OutNext("client_2_sever ping: %d",	net_Statistic.getPing());
			
			pApp->pFont->Color	(D3DCOLOR_XRGB(255,255,255));
			for (DWORD I=0; I<Server->client_Count(); I++)
			{
				IClient*	C = Server->client_Get(I);
				pApp->pFont->OutNext("%10s: P(%d), BPS(%2.1fK), MRR(%2d), MSR(%2d)",
					C->Name,
					C->stats.getPing(),
					float(C->stats.getBPS())/1024,
					C->stats.getMPS_Receive(),
					C->stats.getMPS_Send()
					);
			}
		}
	} else {
		if (psDeviceFlags&rsStatistic)
		{
			pApp->pFont->Size	(0.015f);
			pApp->pFont->OutSet	(0.0f,0.5f);
			pApp->pFont->Color	(D3DCOLOR_XRGB(0,255,0));
			pApp->pFont->OutNext("client_2_sever ping: %d",	net_Statistic.getPing());
		}
	}
}

void CLevel::OnRender()
{
	inherited::OnRender	();
	Tracers.Render		();
	eff_Rain.Render		();
/*
	ph_world->Render	();
	if (bDebug)			{
		ObjectSpace.dbgRender	();
		AI.Render				();
	}
*/
}

void CLevel::OnEvent(EVENT E, DWORD P1, DWORD P2)
{
	if (E==eEntitySpawn)	{
		char	Name[128];	Name[0]=0;
		int		rp=0,team=0,squad=0,group=0;

		sscanf		(LPCSTR(P1),"%[^,],%d,%d,%d,%d", Name, &rp, &team, &squad, &group);
		g_cl_Spawn	(Name,rp,team,squad,group);

	} else if (E==eChangeRP && P1) {
	} else if (E==eDemoPlay && P1) {
		char* name = (char*)P1;
		char RealName [256];
		strcpy(RealName,name);
		strcat(RealName,".xrdemo");
		Cameras.AddEffector(new CDemoPlay(RealName,1.3f));
	} else if (E==eChangeTrack && P1) {
		int id = atoi((char*)P1);
		Environment.Music_Play(id);
	} else if (E==eEnvironment) {
		int id=0; float s=1;
		sscanf((char*)P1,"%d,%f",&id,&s);
		Environment.set_EnvMode(id,s);
	} else return;
}
