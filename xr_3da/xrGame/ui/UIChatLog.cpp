//=============================================================================
//  Filename:   UIChatLog.cpp
//	Created by Roman E. Marchenko, vortex@gsc-game.kiev.ua
//	Copyright 2004. GSC Game World
//	---------------------------------------------------------------------------
//  Multiplayer chat log window
//=============================================================================

#include "stdafx.h"
#include "UIChatLog.h"
#include "UIXmlInit.h"
#include "UIMainIngameWnd.h"

//////////////////////////////////////////////////////////////////////////

extern const char * const	CHAT_MP_WND_XML;
extern const int			fadeDelay;
	   const char * const	CHAT_LOG_ITEMS_ANIMATION	= "ui_mp_chat";

//////////////////////////////////////////////////////////////////////////

void CUIChatLog::AddLogMessage(const shared_str &msg, const shared_str &author)
{
	string256 fullLine;
	::ZeroMemory(fullLine, 256);
	strconcat(fullLine, *author, ": ", *msg);
	CUIColorAnimatorWrapper	*animation	= xr_new<CUIColorAnimatorWrapper>(CHAT_LOG_ITEMS_ANIMATION);
	/*UILogList.*/AddItem<CUIListItem>(fullLine, 0, animation);
	CUIListItem	*item = /*UILogList.*/GetItem(/*UILogList.*/GetSize() - 1);
	VERIFY(item);
	item->Show(true);
	animation->SetColorToModify(&item->GetTextColorRef());
	animation->Cyclic(false);

}

//////////////////////////////////////////////////////////////////////////

void CUIChatLog::Init(int x, int y, int width, int height)
{
	CUIListWnd::Init(x, y, width, height);

	// Chat log
//	AttachChild(&UILogList);
//	UILogList.Init(0,0,width,height);
//	xml_init.InitListWnd(uiXml, "chat_log_list", 0, &UILogList);
	CUIStatic* ps = xr_new<CUIStatic>();
	AttachChild(ps);
	ps->Init(x, y, width, height);
	ps->InitSharedTexture("ui_texture.xml","debug");
	ps->SetStretchTexture(true);
}

void CUIChatLog::Update()
{
	CUIListItem				*item		= NULL;
	CUIColorAnimatorWrapper *anm		= NULL;
	toDelIndexes.clear();
	for (int i = 0; i < /*UILogList.*/GetSize(); ++i)
	{
		item	= /*UILogList.*/GetItem(i);
		VERIFY(item);
		anm		= reinterpret_cast<CUIColorAnimatorWrapper*>(item->GetData());
		anm->Update();

		// Remove at animation end
		if (anm->Done())
		{
			xr_delete(anm);
			toDelIndexes.insert(i);
		}
	}

	// Delete elements
	for (ToDelIndexes_it it = toDelIndexes.begin(); it != toDelIndexes.end(); ++it)
	{
		/*UILogList.*/RemoveItem(*it);
	}
}