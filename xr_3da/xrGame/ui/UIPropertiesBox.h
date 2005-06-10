// UIPropertiesBox.h: 
//
// ����� � ��������� ��� ������ ��������, ���������� �� ������� ������ 
// ������� ����
//////////////////////////////////////////////////////////////////////

#pragma once


#include "uiframewindow.h"
#include "uilistwnd.h"

#include "../script_export_space.h"

class CUIPropertiesBox: public CUIFrameWindow
{
private:
	typedef CUIFrameWindow inherited; 
public:
	////////////////////////////////////
	//�����������/����������
	CUIPropertiesBox();
	virtual ~CUIPropertiesBox();


	////////////////////////////////////
	//�������������
	virtual void Init(LPCSTR base_name, float x, float y, float width, float height);



	virtual void SendMessage(CUIWindow *pWnd, s16 msg, void *pData);
	virtual void OnMouse(float x, float y, EUIMessages mouse_action);

	bool AddItem(const char*  str, void* pData = NULL, int value = 0);
	bool AddItem_script(const char*  str){return AddItem(str);};
	void RemoveItem(int index);
	void RemoveAll();

	virtual void Show(float x, float y);
	virtual void Hide();

	virtual void Update();
	virtual void Draw();


	virtual bool OnKeyboard(int dik, EUIMessages keyboard_action);

	//��������� ������ �������� �������� ��� -1 ����
	//������ �������� �� ����
	int GetClickedIndex();
	//���������� ������� ������� ��� NULL ���� ��� ������
	CUIListItem* GetClickedItem();

	//�������������� ��������� �������� � ����������� �� ����������
	//���������
	void AutoUpdateSize();

protected:
	CUIListWnd m_UIListWnd;

	int m_iClickedElement;
	DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CUIPropertiesBox)
#undef script_type_list
#define script_type_list save_type_list(CUIPropertiesBox)
