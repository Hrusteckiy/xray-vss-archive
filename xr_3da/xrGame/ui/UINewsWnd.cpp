//=============================================================================
//  Filename:   UINewsWnd.cpp
//	Created by Roman E. Marchenko, vortex@gsc-game.kiev.ua
//	Copyright 2004. GSC Game World
//	---------------------------------------------------------------------------
//  News subwindow in PDA dialog
//=============================================================================

#include "stdafx.h"
#include "UINewsWnd.h"
#include "xrXMLParser.h"
#include "UIXmlInit.h"
#include "UIString.h"
#include "../UI.h"
#include "../HUDManager.h"
#include "../level.h"
#include "../game_news.h"
#include "../actor.h"
#include "../alife_registry_wrappers.h"
#include "UIInventoryUtilities.h"

const char * const	NEWS_XML			= "news.xml";

#define NEWS_TO_SHOW 15

//////////////////////////////////////////////////////////////////////////

CUINewsWnd::CUINewsWnd()
{
	// Remove this for enable interactive behaviour of list
	UIListWnd.ActivateList(false);
	SetWindowName("News");
}

//////////////////////////////////////////////////////////////////////////

CUINewsWnd::~CUINewsWnd()
{

}

//////////////////////////////////////////////////////////////////////////

void CUINewsWnd::Init()
{
	CUIXml uiXml;
	bool xml_result = uiXml.Init(CONFIG_PATH, UI_PATH, NEWS_XML);
	R_ASSERT3(xml_result, "xml file not found", NEWS_XML);
	CUIXmlInit xml_init;

	inherited::Init(0,0, UI_BASE_WIDTH, UI_BASE_HEIGHT);

	AttachChild(&UIListWnd);
	xml_init.InitListWnd(uiXml, "list", 0, &UIListWnd);
	UIListWnd.ActivateList(false);
	UIListWnd.EnableScrollBar(true);
	UIListWnd.SetRightIndention(static_cast<int>(20 * UI()->GetScaleX()));
}

//////////////////////////////////////////////////////////////////////////

void CUINewsWnd::AddNews()
{
	CActor *pActor = smart_cast<CActor*>(Level().CurrentEntity());
	UIListWnd.RemoveAll();

	static u32 lastNewsCount = 0;

	if (pActor)
	{
		GAME_NEWS_VECTOR& news_vector = pActor->game_news_registry->registry().objects();
		if (lastNewsCount < news_vector.size())
		{
			lastNewsCount = news_vector.size();
			GetMessageTarget()->SendMessage(this, DIARY_SET_NEWS_AS_UNREAD, NULL);
		}
		
		// �������� ������ NEWS_TO_SHOW ��������� ������
		int currentNews = 0;

		for (GAME_NEWS_VECTOR::reverse_iterator it = news_vector.rbegin(); it != news_vector.rend() && currentNews < NEWS_TO_SHOW ; ++it)
		{
			AddNewsItem(it->FullText());
			++currentNews;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CUINewsWnd::AddNewsItem(const shared_str &text)
{
	static CUIString	str;
	str.SetText(*text);
	UIListWnd.AddParsedItem<CUIListItem>(str, 0, UIListWnd.GetTextColor());
	UIListWnd.AddItem<CUIListItem>("");
}

//////////////////////////////////////////////////////////////////////////

void CUINewsWnd::Show(bool status)
{
	if (status)
		AddNews();
	else
		InventoryUtilities::SendInfoToActor("ui_pda_news_hide");
	inherited::Show(status);
}