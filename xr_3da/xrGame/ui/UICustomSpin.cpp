// file:		UIustomSpin.cpp
// description:	base class for CSpinNum & CSpinText
// created:		15.06.2005
// author:		Serge Vynnychenko
// mail:		narrator@gsc-game.kiev.ua
//
// copyright 2005 GSC Game World

#include "StdAfx.h"
#include "UI3tButton.h"
#include "UIFrameLineWnd.h"
#include "UILines.h"
#include "UICustomSpin.h"

#define SPIN_HEIGHT 21
#define BTN_SIZE 11

CUICustomSpin::CUICustomSpin(){
	m_pFrameLine	= xr_new<CUIFrameLineWnd>();
	m_pBtnUp		= xr_new<CUI3tButton>();
	m_pBtnDown		= xr_new<CUI3tButton>();
	m_pLines		= xr_new<CUILines>();

    m_pFrameLine->SetAutoDelete(true);
	m_pBtnUp->SetAutoDelete(true);
	m_pBtnDown->SetAutoDelete(true);

	AttachChild(m_pFrameLine);
	AttachChild(m_pBtnUp);
	AttachChild(m_pBtnDown);
	m_pLines->SetTextAlignment(CGameFont::alLeft);
	m_pLines->SetVTextAlignment(valCenter);
	m_pLines->SetFont(UI()->Font()->pFontLetterica16Russian);
	m_pLines->SetTextColor(color_argb(255,235,219,185));

	m_time_begin = 0;
	m_delay = 500;	
}

CUICustomSpin::~CUICustomSpin(){
	xr_delete(m_pLines);
//	xr_delete(m_pFrameLine);
//	xr_delete(m_pBtnUp);
//	xr_delete(m_pBtnDown);
}

void CUICustomSpin::Init(float x, float y, float width, float height){
	CUIWindow::Init(x,y,width,SPIN_HEIGHT);
	m_pFrameLine->Init(0,0,width, SPIN_HEIGHT);
	m_pFrameLine->InitTexture("ui_spiner");
	m_pBtnUp->Init(width - BTN_SIZE - 1, 0, BTN_SIZE, BTN_SIZE);
	m_pBtnUp->InitTexture("ui_spiner_button_t");
	m_pBtnDown->Init(width - BTN_SIZE - 1, BTN_SIZE, BTN_SIZE, BTN_SIZE);
	m_pBtnDown->InitTexture("ui_spiner_button_b");

	m_pLines->Init(0,0,width - BTN_SIZE - 10, SPIN_HEIGHT);
}

void CUICustomSpin::SendMessage(CUIWindow* pWnd, s16 msg, void* pData /* = NULL */){
	if (BUTTON_CLICKED == msg)
	{
		if (m_pBtnUp == pWnd){
			OnBtnUpClick();			
		}
		else if (m_pBtnDown == pWnd){
			OnBtnDownClick();			
		}
	}
}

void CUICustomSpin::Enable(bool status){
	m_pBtnDown->Enable(status);
	m_pBtnUp->Enable(status);

	if (!status)
		m_pLines->SetTextColor(color_argb(255,100,100,100));
	else
		m_pLines->SetTextColor(color_argb(255,235,219,185));
}

void CUICustomSpin::OnBtnUpClick(){
	GetMessageTarget()->SendMessage(this, BUTTON_CLICKED);
}

void CUICustomSpin::OnBtnDownClick(){
	GetMessageTarget()->SendMessage(this, BUTTON_CLICKED);
}

void CUICustomSpin::Draw(){
	CUIWindow::Draw();
	Fvector2 pos = GetAbsolutePos();
	m_pLines->Draw(pos.x + 3, pos.y);
}

void CUICustomSpin::Update(){
	CUIWindow::Update();
	if (CUIButton::BUTTON_PUSHED == m_pBtnUp->GetButtonsState())
	{		
		if (0 == m_time_begin){
			m_time_begin = Device.dwTimeContinual;
		}

		if (m_time_begin < Device.dwTimeContinual - m_delay)
		{
			OnBtnUpClick();
			m_time_begin = Device.dwTimeContinual;

			if (m_delay > 100)
				m_delay -= 100;
		}
	}
	else if (CUIButton::BUTTON_PUSHED == m_pBtnDown->GetButtonsState())
	{
		if (0 == m_time_begin){
			m_time_begin = Device.dwTimeContinual;
		}

		if (m_time_begin < Device.dwTimeContinual - m_delay)
		{
			OnBtnDownClick();
			m_time_begin = Device.dwTimeContinual;

			if (m_delay > 100)
				m_delay -= 100;
		}
	}
	else
	{
		m_delay = 500;
		m_time_begin = 0;
	}
}

LPCSTR CUICustomSpin::GetText(){
	return m_pLines->GetText();
}

