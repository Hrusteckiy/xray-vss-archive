// UIButton.h: ����� ���������� ������
//
//////////////////////////////////////////////////////////////////////

#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#pragma once


#include "UIWindow.h"


#include "../uistaticitem.h"
#include "UIStatic.h"



class CUIButton : public CUIStatic
{
private:
	typedef CUIStatic inherited;
public:
	CUIButton();
	virtual ~ CUIButton();


	virtual void Init(LPCSTR tex_name, int x, int y, int width, int height);
	
	virtual void OnMouse(int x, int y, E_MOUSEACTION mouse_action);

	//���������, ������������ ������������� ����
	typedef enum{BUTTON_CLICKED, BUTTON_FOCUS_RECEIVED,BUTTON_FOCUS_LOST} E_MESSAGE;

	//���������� ����
	virtual void Draw();
	//���������� ����� �����������
	virtual void Update();


	//������ � ������� ����� �������� ������
	typedef enum{NORMAL_PRESS, //������ ���������� ��� 
							   //������� � ���������� �� ��� ����
				 DOWN_PRESS,   //����� ��� �������
				 UP_PRESS      //����� ��� ����������
			} E_PRESS_MODE;

	void SetPressMode(E_PRESS_MODE ePressMode) {m_ePressMode = ePressMode;}
	E_PRESS_MODE GetPressMode() {return m_ePressMode;}

	//������ ����������� ���������
    virtual void Reset();

	void SetPushOffsetX(int offset_x) {m_iPushOffsetX = offset_x;}
	void SetPushOffsetY(int offset_y) {m_iPushOffsetY = offset_y;}
	int GetPushOffsetX() {return m_iPushOffsetX;}
	int GetPushOffsetY() {return m_iPushOffsetY;}

	virtual	void UpdateTextAlign();

protected:
	
	//��������� � ������� ���������� ������
	typedef enum{BUTTON_NORMAL, //������ ����� �� �������������
				 BUTTON_PUSHED,   //� ������� ��������
				 BUTTON_UP      //��� ������������ ������ ���� 
			} E_BUTTON_STATE;
	
	E_BUTTON_STATE m_eButtonState;

	//���� ������ ���� ������ ��� ������
	bool m_bButtonClicked;

	//���� ������ ��� ������
	bool m_bCursorOverButton;

	//����� � ������� ���������� ������
	E_PRESS_MODE m_ePressMode;

	//�������� ������ ��� �������
	int m_iPushOffsetX;
	int m_iPushOffsetY;
};

#endif // _UI_BUTTON_H_
