// Window.h: interface for the CUIWindow class.
//
// ����������� ����� ����� ��������� CUIWindowDraw
//////////////////////////////////////////////////////////////////////

#pragma once


class CUIWindow  
{
public:
	////////////////////////////////////
	//�����������/����������
	CUIWindow();
	virtual ~CUIWindow();

	////////////////////////////////////
	//�������������
	virtual void Init(int x, int y, int width, int height);
	virtual void Init(RECT* pRect);

	////////////////////////////////////
	//������ � ��������� � ������������� ������
	void AttachChild(CUIWindow* pChild);
	void DetachChild(CUIWindow* pChild);
	void DetachAll();
	int GetChildNum() {return m_ChildWndList.size();} 

	void SetParent(CUIWindow* pNewParent) {m_pParentWnd = pNewParent;}
	CUIWindow* GetParent() {return m_pParentWnd;}
	
	//�������� ���� ������ �������� ������
	CUIWindow* GetTop() {if(m_pParentWnd == NULL)	
								return  this;
							else
								return  m_pParentWnd->GetTop();}


	//������� �� ������� ������ ��������� �������� ����
	bool BringToTop(CUIWindow* pChild);

	//������� �� ������� ������ ���� ��������� ���� � ��� ������
	void BringAllToTop();
	

	////////////////////////////////////
	//������ � �������� ����������� (����, ����������, ������ ����)

	//������� �� ����
	typedef enum{LBUTTON_DOWN, RBUTTON_DOWN, LBUTTON_UP, RBUTTON_UP, MOUSE_MOVE,
				 LBUTTON_DB_CLICK} E_MOUSEACTION;

	virtual void OnMouse(int x, int y, E_MOUSEACTION mouse_action);

	//���������/���������� ���� �����
	//��������� ���������� �������� ����� �������������
	virtual void SetCapture(CUIWindow* pChildWindow, bool capture_status);


	//������� �� ����������
	typedef enum{KEY_PRESSED, KEY_RELEASED} E_KEYBOARDACTION;
	virtual bool OnKeyboard(int dik, E_KEYBOARDACTION keyboard_action);
	virtual void SetKeyboardCapture(CUIWindow* pChildWindow, bool capture_status);

	
	//������ ������������, �� ������� ������ ���� ������������� �������
	typedef enum{} E_MESSAGE;
	
	//��������� ��������� �� �������������� ������������ �������������
	//�-��� ������ ����������������
	//pWnd - ��������� �� ����, ������� ������� ���������
	//pData - ��������� �� �������������� ������, ������� ����� ������������
	virtual void SendMessage(CUIWindow* pWnd, s16 msg, void* pData = NULL);
	
	

	//����������/���������� �� ���� � ����������
	void Enable(bool status) {m_bIsEnabled =  status;}
	bool IsEnabled() {return m_bIsEnabled;}

	//������/�������� ���� � ��� �������� ����
	void Show(bool status) {m_bIsShown =  status;}
	bool IsShown() {return m_bIsShown;}
	
	////////////////////////////////////
	//��������� � ������� ����

	//������������� ����������
	RECT GetWndRect() {return m_WndRect;}
	void SetWndRect(int x, int y, int width, int height) 
						{SetRect(&m_WndRect,x,y,x+width,y+height);}

	void MoveWindow(int x, int y)
					{int w = GetWidth();
					 int h = GetHeight();
					 SetRect(&m_WndRect,x,y,x+w,y+h);}



	//���������� ����������
	RECT GetAbsoluteRect();

	int GetWidth() {return m_WndRect.right-m_WndRect.left;}
	void SetWidth(int width) {SetRect(&m_WndRect,
										m_WndRect.left,
										m_WndRect.top,
										m_WndRect.left+width,
										m_WndRect.bottom);}

	int GetHeight() {return m_WndRect.bottom-m_WndRect.top;}
	void SetHeight(int height) {SetRect(&m_WndRect,
										m_WndRect.left,
										m_WndRect.top,
										m_WndRect.right,
										m_WndRect.top+height);}



	////////////////////////////////////
	//���������� ����
	virtual void Draw();
	//���������� ���� ����������������
	virtual void Update();


	//��������!!!!
	void SetFont(CGameFont* pFont) {m_pFont = pFont;}
	CGameFont* GetFont() {if(m_pFont) return m_pFont;
							if(m_pParentWnd== NULL)	
								return  m_pFont;
							else
								return  m_pParentWnd->GetFont();}

protected:

	DEF_LIST (WINDOW_LIST, CUIWindow*);


	//������ �������� ����
	WINDOW_LIST m_ChildWndList;
	
	//��������� �� ������������ ����
	CUIWindow* m_pParentWnd;

	//�������� ���� �������, ��������� ���� ����
	CUIWindow* m_pMouseCapturer;
	//�������� ���� �������, ��������� ���� ����������
	CUIWindow* m_pKeyboardCapturer;

	//��������� � ������ ����, �������� 
	//������������ ������������� ����
	RECT m_WndRect;

	//�������� �� ���� ������������
	bool m_bIsEnabled;
	//���������� �� ����
	bool m_bIsShown;


	/////////////
	//��������� �� ������������ �����
	//��������!!!!
	CGameFont* m_pFont;


	//����� �������� ����� �����
	//��� ����������� DoubleClick
	u32 m_dwLastClickTime;

};