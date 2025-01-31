// File:        UITabButton.cpp
// Description: 
// Created:     19.11.2004
// Last Change: 19.11.2004
// Author:      Serhiy Vynnychenko
// Mail:        narrator@gsc-game.kiev.ua
//
// Copyright: 2004 GSC Game World

#pragma once
#include "UI3tButton.h"
#include "UIFrameWindow.h"

class CUITabButton : public CUI3tButton {
	typedef CUI3tButton inherited;
public:
	CUITabButton();
	virtual ~CUITabButton();
	
	// behavior	
	virtual void AssociateWindow(CUIFrameWindow* pWindow);
	virtual CUIFrameWindow* GetAssociatedWindow();
	//virtual void Enable(bool bEnable);
	virtual void ShowAssociatedWindow(bool bShow = true);
	virtual void SendMessage(CUIWindow* pWnd, s16 msg, void* pData = 0);
	virtual void OnMouse(int x, int y, EUIMessages mouse_action);
	virtual void OnMouseDown(bool left_button = true);
	virtual void Update();

protected:
	// associated window
	CUIFrameWindow* m_pAssociatedWindow;
};