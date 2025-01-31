#include "stdafx.h"
#include "UIGameCustom.h"
#include "ui.h"
#include "level.h"
#include "hudmanager.h"

CUIGameCustom::CUIGameCustom()
{
//	m_pMainInputReceiver	= NULL;
	uFlags					= 0;
	shedule.t_min			= 5;
	shedule.t_max			= 20;
	shedule_register		();
}

CUIGameCustom::~CUIGameCustom()
{
	shedule_unregister();
}


float CUIGameCustom::shedule_Scale		() 
{
	return 0.5f;
};

void CUIGameCustom::shedule_Update		(u32 dt)
{
	inherited::shedule_Update(dt);
/*
	xr_vector<CUIWindow*>::iterator it = m_dialogsToRender.begin();
	for(; it!=m_dialogsToRender.end();++it)
		if((*it)->IsEnabled())
			(*it)->Update();


	for(it = m_dialogsToErase.begin(); it!=m_dialogsToErase.end(); ++it)
	{
		xr_vector<CUIWindow*>::iterator it_find = std::find(m_dialogsToRender.begin(),
															m_dialogsToRender.end(), *it);
		if (it_find != m_dialogsToRender.end())
		{
			m_dialogsToRender.erase(it_find);
		}
    }
	m_dialogsToErase.clear();
*/
}

void CUIGameCustom::OnFrame() 
{
}

void CUIGameCustom::Render()
{
/*	xr_vector<CUIWindow*>::iterator it = m_dialogsToRender.begin();
	for(; it!=m_dialogsToRender.end();++it)
		if((*it)->IsShown())
			(*it)->Draw();
*/
	m_gameCaptions.Draw();
}

bool CUIGameCustom::IR_OnKeyboardPress(int dik) 
{
/*	if(m_pMainInputReceiver)
	{
		if(m_pMainInputReceiver->IR_OnKeyboardPress(dik)) 
			return true;
	}*/

	return false;
}

bool CUIGameCustom::IR_OnKeyboardRelease(int dik) 
{
/*	if(m_pMainInputReceiver)
	{
		if(m_pMainInputReceiver->IR_OnKeyboardRelease(dik)) 
			return true;
	}*/
	return false;
}

bool CUIGameCustom::IR_OnMouseMove(int dx,int dy)
{
/*	if(m_pMainInputReceiver)
	{
		if(m_pMainInputReceiver->IR_OnMouseMove(dx, dy)) 
			return true;
	}*/
	return false;
}
bool CUIGameCustom::IR_OnMouseWheel			(int direction)
{
	return false;
}

void CUIGameCustom::AddDialogToRender(CUIWindow* pDialog)
{
	HUD().GetUI()->AddDialogToRender(pDialog);
/*
	if(std::find(m_dialogsToRender.begin(), m_dialogsToRender.end(), pDialog) == m_dialogsToRender.end() )
	{
		m_dialogsToRender.push_back(pDialog);
		pDialog->Show(true);
	}*/
}

void CUIGameCustom::RemoveDialogToRender(CUIWindow* pDialog)
{
	HUD().GetUI()->RemoveDialogToRender(pDialog);
/*
	xr_vector<CUIWindow*>::iterator it = std::find(m_dialogsToRender.begin(),m_dialogsToRender.end(),pDialog);
	if(it != m_dialogsToRender.end())
	{
		(*it)->Show(false);
		(*it)->Enable(false);
		m_dialogsToErase.push_back(*it);
	}
*/
}

CUIDialogWnd* CUIGameCustom::MainInputReceiver	()
{ 
	return HUD().GetUI()->MainInputReceiver();
};

#include "script_space.h"
using namespace luabind;



void CUIGameCustom::script_register(lua_State *L)
{
	module(L)
		[
			luabind::class_< CUIGameCustom >("CUIGameCustom")
			.def("AddDialogToRender", &CUIGameCustom::AddDialogToRender)
			.def("RemoveDialogToRender", &CUIGameCustom::RemoveDialogToRender)

		];
}
