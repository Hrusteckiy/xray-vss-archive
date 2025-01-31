// File:		UIPdaKillMessage.cpp
// Description:	HUD message about player death. Implementation of visual behavior
// Created:		10.03.2005
// Author:		Serge Vynnychenko
// Mail:		narrator@gsc-game.kiev.ua
// 
// Copyright 2005 GSC GameWorld

#include "StdAfx.h"
#include "UIPdaKillMessage.h"
#include "UIInventoryUtilities.h"

using namespace InventoryUtilities;

const int INDENT = 3;

CUIPdaKillMessage::CUIPdaKillMessage(){
	AttachChild(&m_victim_name);
	AttachChild(&m_killer_name);	
	AttachChild(&m_initiator);
	AttachChild(&m_ext_info);
}

CUIPdaKillMessage::CUIPdaKillMessage(int iDelay){
	AttachChild(&m_victim_name);
	AttachChild(&m_killer_name);	
	AttachChild(&m_initiator);
	AttachChild(&m_ext_info);

   	m_iDelay = iDelay;
	m_timeBegin = 0;
}

CUIPdaKillMessage::~CUIPdaKillMessage(){

}

void CUIPdaKillMessage::Init(KillMessageStruct& msg){
#ifdef DEBUG
//	R_ASSERT2(xr_strlen(msg.m_victim.m_name), "CUIPdaKillMessage::Init(msg) - victim has no name");
	R_ASSERT2(GetWidth(),  "CUIPdaKillMessage::Init(msg) - need to call ::Init(x, y, width, height) before");
	R_ASSERT2(GetHeight(), "CUIPdaKillMessage::Init(msg) - need to call ::Init(x, y, width, height) before");
#endif	

	int		x = 0;
	int width = 0;

	width = InitText(m_killer_name, x, msg.m_killer);		x += width + INDENT;
	width = InitIcon(m_initiator,   x, msg.m_initiator);	x += width + INDENT;
	width = InitText(m_victim_name, x, msg.m_victim);		x += width + INDENT;
			InitIcon(m_ext_info,	x, msg.m_ext_info);
}

int CUIPdaKillMessage::InitText(CUIStatic& refStatic, int x, PlayerInfo& info){

	if ( 0 == xr_strlen(info.m_name))
		return 0;

	int		y = 0;
	int		selfHeight = GetHeight();
	CGameFont* pFont = GetFont();

	int width = (int)pFont->SizeOf(*info.m_name);
	int height = (int)pFont->CurrentHeight();
	y = (selfHeight - height)/2;

	if (width > 110)
		width = 110;

	refStatic.Init(x, y, width + 1, height);
	refStatic.SetElipsis(CUIStatic::eepEnd, 0);
	refStatic.SetText(*info.m_name);
	refStatic.SetTextColor(info.m_color);

	return width;
}

void CUIPdaKillMessage::SetTextColor(u32 color){	
	m_victim_name.SetTextColor(subst_alpha(m_victim_name.GetTextColor(), color_get_A(color)));
	m_killer_name.SetTextColor(subst_alpha(m_killer_name.GetTextColor(), color_get_A(color)));
	SetColor(color);
}

void CUIPdaKillMessage::SetColor(u32 color){	
	m_initiator.SetColor(color);	
	m_ext_info.SetColor(color);
}

int CUIPdaKillMessage::InitIcon(CUIStatic& refStatic, int x, IconInfo& info){
	if ( 0 == info.m_rect.width())
		return 0;

	if (info.m_shader == NULL)
		return 0;

	int		y = 0;
	int		selfHeight = GetHeight();
	float	scale = 0;
	Irect	rect = info.m_rect;

	int width = rect.width();
	int height = rect.height();
	
	scale = 1;
	width  = (int)((float)width*scale);
	height = (int)((float)height*scale);
	y = (selfHeight - height) /2;
	refStatic.Init(x, y, width, height);
	refStatic.SetOriginalRect(info.m_rect);
	refStatic.SetShader(info.m_shader);
	refStatic.SetStretchTexture(true);

	return width;
}

void CUIPdaKillMessage::Init(int x, int y, int width, int height){
	inherited::Init(x, y, width, height);
}

void CUIPdaKillMessage::SetFont(CGameFont* pFont){
	m_victim_name.SetFont(pFont);
	m_killer_name.SetFont(pFont);
	CUIStatic::SetFont(pFont);
}