// UIPdaWnd.cpp:  ������ PDA
// 
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "UIPdaWnd.h"
#include "../Pda.h"

#include "xrXMLParser.h"
#include "UIXmlInit.h"

#include "../HUDManager.h"
#include "../level.h"

#define PDA_XML "pda.xml"

const char * const ALL_PDA_HEADER_PREFIX = "#root 15/FD-665#68";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIPdaWnd::CUIPdaWnd()
{
	Init();

	Hide();
	
//	m_pInvOwner = NULL;
//	m_pPda = NULL;

//	m_pActiveDialog = NULL;
	SetFont(HUD().pFontMedium);
}

CUIPdaWnd::~CUIPdaWnd()
{
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::Init()
{
	CUIXml uiXml;
	bool xml_result = uiXml.Init("$game_data$",PDA_XML);
	R_ASSERT2(xml_result, "xml file not found");

	CUIXmlInit xml_init;

	inherited::Init(0,0, Device.dwWidth, Device.dwHeight);

//	AttachChild(&UIStaticTop);
//	UIStaticTop.Init("ui\\ui_top_background", 0,0,UI_BASE_WIDTH,128);
//	AttachChild(&UIStaticBottom);
//	UIStaticBottom.Init("ui\\ui_bottom_background", 0,UI_BASE_HEIGHT-32,UI_BASE_HEIGHT,32);
	
	AttachChild(&UIMainPdaFrame);
	xml_init.InitStatic(uiXml, "background_static", 0, &UIMainPdaFrame);

	//�������� ��������������� ����������
	xml_init.InitAutoStatic(uiXml, "auto_static", this);

	// Main buttons background
	AttachChild(&UIMainButtonsBackground);
	xml_init.InitFrameLine(uiXml, "mbbackground_frame_line", 0, &UIMainButtonsBackground);

	// Timer background
	AttachChild(&UITimerBackground);
	xml_init.InitFrameLine(uiXml, "timer_frame_line", 0, &UITimerBackground);


 	// Tab control
	AttachChild(&UITabControl);
	xml_init.InitTabControl(uiXml, "tab", 0, &UITabControl);

	// O��� ��������a���
	UIPdaCommunication.Init();
	UIMainPdaFrame.AttachChild(&UIPdaCommunication);

	// O��� �����
	UIMainPdaFrame.AttachChild(&UIMapWnd);
	UIMapWnd.Init();

	// ���� �����
	UIMainPdaFrame.AttachChild(&UITaskWnd);
	UITaskWnd.Init();
	
	// O��� ��������
//	UIMainPdaFrame.AttachChild(&UIDiaryWnd);
//	UIDiaryWnd.Init();
//	UIDiaryWnd.SetMessageTarget(this);

	// ���� ������������
	UIMainPdaFrame.AttachChild(&UIEncyclopediaWnd);
	UIEncyclopediaWnd.Init();
	UIEncyclopediaWnd.Hide();

	m_pActiveDialog = &UIPdaCommunication;
	//!!!
	//UITabControl.SetNewActiveTab(1);
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::SendMessage(CUIWindow* pWnd, s16 msg, void* pData)
{
	if(pWnd == &UITabControl)
	{
		if (CUITabControl::TAB_CHANGED == msg)
		{
			if (m_pActiveDialog)
				m_pActiveDialog->Hide();

			// Add custom dialogs here
			switch (UITabControl.GetActiveIndex()) 
			{
			case 0:
				m_pActiveDialog = dynamic_cast<CUIDialogWnd*>(&UITaskWnd);
				break;
			case 1:
				m_pActiveDialog = dynamic_cast<CUIDialogWnd*>(&UIPdaCommunication);
				break;
			case 2:
				m_pActiveDialog = dynamic_cast<CUIDialogWnd*>(&UIMapWnd);
				break;
			case 3:
				m_pActiveDialog = dynamic_cast<CUIDialogWnd*>(&UIEncyclopediaWnd);
				break;
			}
			m_pActiveDialog->Reset();
			m_pActiveDialog->Show();
		}
	}
	// ������� ����� ������������� �����. ����� ���������� ��������� � pData
	else if (PDA_MAP_SET_ACTIVE_POINT == msg)
	{
		UIMapWnd.SetActivePoint(*reinterpret_cast<Fvector*>(pData));
	}
	else
	{
		R_ASSERT(m_pActiveDialog);
		m_pActiveDialog->SendMessage(pWnd, msg, pData);
	}
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::Show()
{
	inherited::Show();
	m_pActiveDialog->Show();
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::ChangeActiveTab(E_PDA_TABS tabNewTab)
{
	UITabControl.SetNewActiveTab(tabNewTab);
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::FocusOnMap(const int x, const int y, const int z)
{
	Fvector a;
	a.set(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	SendMessage(this, CUIPdaWnd::PDA_MAP_SET_ACTIVE_POINT, &a);
	ChangeActiveTab(CUIPdaWnd::TAB_MAP);
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::AddNewsItem(const char *sData)
{
	UIDiaryWnd.AddNewsItem(sData);
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::UpdateDateTime()
{
	string32 buf;
	ALife::_TIME_ID currMsec = Level().GetGameTime();

	static u8 prevCurrSecs = 0xff;
	u8 currSecs		= static_cast<u8>(currMsec / 1000 % 60 & 0xFF);
	u8 currMins		= static_cast<u8>(currMsec / (1000 * 60) % 60 & 0xFF);
	u8 currHours	= static_cast<u8>(currMsec / (1000 * 3600) % 24 & 0xFF);

	if (prevCurrSecs != currSecs)
	{
		sprintf(buf, "%02i:%02i:%02i", currHours, currMins, currSecs);
		UITimerBackground.UITitleText.SetText(buf);
		prevCurrSecs = currSecs;
	}
}

//////////////////////////////////////////////////////////////////////////

void CUIPdaWnd::Update()
{
	inherited::Update();
	UpdateDateTime();
}