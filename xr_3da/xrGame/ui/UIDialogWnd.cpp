// UIDialogWnd.cpp: ����� �������� �������, ����� ��� ������������ �������
// ������ ������� ����� ������ ����������� Show() � Hide()
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "uidialogwnd.h"
#include "../hudmanager.h"
#include "../xr_level_controller.h"
#include "../../xr_ioconsole.h"
#include "../level.h"
#include "../GameObject.h"

CUIDialogWnd:: CUIDialogWnd()
{
	m_pHolder = NULL;
	Hide();
}

CUIDialogWnd::~ CUIDialogWnd()
{
}

void CUIDialogWnd::Show()
{
	inherited::Enable(true);
	inherited::Show(true);


	ResetAll();
}


void CUIDialogWnd::Hide()
{

	inherited::Enable(false);
	inherited::Show(false);
	
}


bool CUIDialogWnd::IR_OnKeyboardPress(int dik)
{
	if(!IsEnabled()) return false;
			
	//mouse click
	if(dik==MOUSE_1)
	{
		OnMouse(static_cast<int>(GetUICursor()->GetPos().x /* / UI()->GetScaleX() */),
				static_cast<int>(GetUICursor()->GetPos().y /* / UI()->GetScaleY() */),
                WINDOW_LBUTTON_DOWN);
		return true;
	}
	else if(dik==MOUSE_2)
	{
		OnMouse(static_cast<int>(GetUICursor()->GetPos().x /* / UI()->GetScaleX() */),
				static_cast<int>(GetUICursor()->GetPos().y /* / UI()->GetScaleY() */),
                WINDOW_RBUTTON_DOWN);
		return true;
	}

	
	return 	OnKeyboard(dik,	WINDOW_KEY_PRESSED);
}

bool CUIDialogWnd::IR_OnKeyboardRelease(int dik)
{
	if(!IsEnabled()) return false;
	

	//mouse click
	if(dik==MOUSE_1 || dik==MOUSE_2)
	{
		int x = static_cast<int>(GetUICursor()->GetPos().x /* / UI()->GetScaleX() */);
		int y = static_cast<int>(GetUICursor()->GetPos().y /* / UI()->GetScaleY() */);
		
		if(dik==MOUSE_1)
			OnMouse(x,   y,	WINDOW_LBUTTON_UP);
		else
			OnMouse(x,   y,	WINDOW_RBUTTON_UP);

		return true;
	}

	return 	OnKeyboard(dik,	WINDOW_KEY_RELEASED);
}

bool CUIDialogWnd::IR_OnMouseWheel (int direction)
{
	Ivector2 pos = GetUICursor()->GetPos();

	if(direction>0)
		OnMouse(pos.x,pos.y,WINDOW_MOUSE_WHEEL_UP);
	else
		OnMouse(pos.x,pos.y,WINDOW_MOUSE_WHEEL_DOWN);

	return true;
}

bool CUIDialogWnd::IR_OnMouseMove(int dx, int dy)
{
	if(!IsEnabled()) return false;
	
	if (GetUICursor()->IsVisible())
	{ 
		GetUICursor()->MoveBy(dx, dy);
		Ivector2 cPos = GetUICursor()->GetPos();
		OnMouse(cPos.x, cPos.y , WINDOW_MOUSE_MOVE);
	}

	return true;
}

bool CUIDialogWnd::OnKeyboard(int dik, EUIMessages keyboard_action)
{
	if (inherited::OnKeyboard(dik, keyboard_action) )
		return true;

	if( !StopAnyMove() && g_pGameLevel ){
		CObject* O = Level().CurrentEntity();
		if( O ){
			IInputReceiver*		IR	= smart_cast<IInputReceiver*>( smart_cast<CGameObject*>(O) );
			if (!IR)
				return			(false);

			if(keyboard_action==WINDOW_KEY_PRESSED)
					IR->IR_OnKeyboardPress(key_binding[dik]);
			else	
					IR->IR_OnKeyboardRelease(key_binding[dik]);
		
		//	return true;
		}
	};
	return false;
}
