#include "stdafx.h"
#pragma hdrstop

#include "IGame_Persistent.h"
#include "environment.h"
#ifndef _EDITOR
#	include "IGame_Level.h"
#	include "XR_IOConsole.h"
#	include "Render.h"
#	include "ps_instance.h"
#endif

ENGINE_API	IGame_Persistent*		g_pGamePersistent	= NULL;

IGame_Persistent::IGame_Persistent	()
{
	Device.seqAppStart.Add			(this);
	Device.seqAppEnd.Add			(this);
	Device.seqFrame.Add				(this,REG_PRIORITY_HIGH+1);
	Device.seqAppActivate.Add		(this);
	Device.seqAppDeactivate.Add		(this);

	m_pMainUI						= NULL;
}

IGame_Persistent::~IGame_Persistent	()
{
	Device.seqFrame.Remove			(this);
	Device.seqAppStart.Remove		(this);
	Device.seqAppEnd.Remove			(this);
	Device.seqAppActivate.Remove	(this);
	Device.seqAppDeactivate.Remove	(this);
}

void IGame_Persistent::OnAppActivate		()
{
	Device.Pause(FALSE);
}

void IGame_Persistent::OnAppDeactivate		()
{
	if(!bDedicatedServer)
		Device.Pause(TRUE);
}

void IGame_Persistent::OnAppStart	()
{
	if (strstr(Core.Params,"-dedicated"))	bDedicatedServer	= TRUE;
	else									bDedicatedServer	= FALSE;
	Environment.load				();
}

void IGame_Persistent::OnAppEnd		()
{
	Environment.unload				();
	OnGameEnd						();
}

void IGame_Persistent::Start		(LPCSTR op)
{
	string256						prev_type;
	strcpy							(prev_type,m_game_params.m_game_type);
	m_game_params.parse_cmd_line	(op);
	// change game type
	if (0!=xr_strcmp(prev_type,m_game_params.m_game_type)){
		OnGameEnd					();
		OnGameStart					();
	}
}

void IGame_Persistent::Disconnect	()
{
	// Cleanup particles, some of them can be still active
	ps_needtoplay.clear				();
	ps_destroy.clear				();
#ifndef _EDITOR
	while (!ps_active.empty())
		(*ps_active.begin())->PSI_internal_delete();
#endif
}

void IGame_Persistent::OnGameStart	()
{
#ifndef _EDITOR
	if (0==strstr(Core.Params,"-noprefetch")){
		// prefetch game objects
		ObjectPool.prefetch				();
		Render->models_Prefetch			();
	}
#endif
}

void IGame_Persistent::OnGameEnd	()
{
#ifndef _EDITOR
	ObjectPool.clear				();
	Render->models_Clear			();
#endif
}

void IGame_Persistent::OnFrame		()
{
	Environment.OnFrame				();
#ifndef _EDITOR
	if(Device.dwFrame ==50){
		if(!g_pGameLevel)
			Console->Execute("main_menu on");
	}

	Device.Statistic->Particles_starting	= ps_needtoplay.size	();
	Device.Statistic->Particles_active	= ps_active.size		();
	Device.Statistic->Particles_destroy	= ps_destroy.size		();

	// Play req particle systems
	while (ps_needtoplay.size())
	{
		CPS_Instance*	psi		= ps_needtoplay.back	();
		ps_needtoplay.pop_back	();
		psi->Play				();
	}
	// Destroy inactive particle systems
	while (ps_destroy.size())
	{
		CPS_Instance*	psi		= ps_destroy.back	();
		if (psi->Locked())		break;
		ps_destroy.pop_back		();
		psi->PSI_internal_delete();
	}
#endif
}
