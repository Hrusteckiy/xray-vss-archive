#include "stdafx.h"
#include "UIWindow.h"
#include "UIFrameWindow.h"
#include "UIFrameLineWnd.h"
#include "UIDialogWnd.h"
#include "../HUDManager.h"
#include "../script_space.h"
#include <luabind\adopt_policy.hpp>
#include "../GamePersistent.h"
#include "../MainUI.h"

CFontManager& mngr(){
	return *(UI()->Font());
}

// hud font
CGameFont* GetFontSmall()
{return mngr().pFontSmall;}

CGameFont* GetFontMedium()
{return mngr().pFontMedium;}
CGameFont* GetFontDI()
{return mngr().pFontDI;}
CGameFont* GetFontBigDigit()
{return mngr().pFontBigDigit;}

	//������������ �����
CGameFont* GetFontHeaderRussian()
{return mngr().pFontHeaderRussian;}
CGameFont* GetFontHeaderEurope()
{return mngr().pFontHeaderEurope;}

//������ ��� ����������
CGameFont* GetFontArialN21Russian()
{return mngr().pArialN21Russian;}
CGameFont* GetFontGraffiti19Russian()
{return mngr().pFontGraffiti19Russian;}
CGameFont* GetFontGraffiti22Russian()
{return mngr().pFontGraffiti22Russian;}
CGameFont* GetFontLetterica16Russian()
{return mngr().pFontLetterica16Russian;}
CGameFont* GetFontLetterica18Russian()
{return mngr().pFontLetterica18Russian;}
CGameFont* GetFontGraffiti32Russian()
{return mngr().pFontGraffiti32Russian;}
CGameFont* GetFontGraffiti50Russian()
{return mngr().pFontGraffiti50Russian;}
CGameFont* GetFontLetterica25()
{return mngr().pFontLetterica25;}


int GetARGB(u16 a, u16 r, u16 g, u16 b)
{return color_argb(a,r,g,b);}

using namespace luabind;

void CUIWindow::script_register(lua_State *L)
{
	module(L)
	[
		def("GetARGB",					&GetARGB),
		def("GetFontSmall",				&GetFontSmall),
		def("GetFontMedium",			&GetFontMedium),
		def("GetFontDI",				&GetFontDI),
		def("GetFontBigDigit",			&GetFontBigDigit),
		def("GetFontHeaderRussian",		&GetFontHeaderRussian),
		def("GetFontHeaderEurope",		&GetFontHeaderEurope),
		def("GetFontArialN21Russian",	&GetFontArialN21Russian),
		def("GetFontGraffiti19Russian",	&GetFontGraffiti19Russian),
		def("GetFontGraffiti22Russian",	&GetFontGraffiti22Russian),
		def("GetFontLetterica16Russian",&GetFontLetterica16Russian),
		def("GetFontLetterica18Russian",&GetFontLetterica18Russian),
		def("GetFontGraffiti32Russian",	&GetFontGraffiti32Russian),
		def("GetFontGraffiti50Russian",	&GetFontGraffiti50Russian),
		def("GetFontLetterica25",		&GetFontLetterica25),

		class_<Irect>("Irect")
		.def(					constructor<>())
		.def_readwrite("right",					&Irect::right)
		.def_readwrite("left",					&Irect::left)
		.def_readwrite("top",					&Irect::top)
		.def_readwrite("bottom",				&Irect::bottom)
		.def("width",							&Irect::width)
		.def("height",							&Irect::height),

		class_<CUIWindow>("CUIWindow")
		.def(							constructor<>())
		.def("AttachChild",				&CUIWindow::AttachChild, adopt(_2))
		.def("DetachChild",				&CUIWindow::DetachChild)
		.def("SetAutoDelete",			&CUIWindow::SetAutoDelete)
		.def("IsAutoDelete",			&CUIWindow::IsAutoDelete)

		.def("SetWndRect",				(void (CUIWindow::*)(Irect))				 &CUIWindow::SetWndRect)
		.def("SetWndRect",				(void (CUIWindow::*)(int,int,int,int))   &CUIWindow::SetWndRect)
		.def("Init",					(void (CUIWindow::*)(int,int,int,int))   &CUIWindow::Init)
		.def("Init",					(void (CUIWindow::*)(Irect*))			 &CUIWindow::Init)
		.def("GetWidth",				&CUIWindow::GetWidth)
		.def("SetWidth",				&CUIWindow::SetWidth)
		.def("GetHeight",				&CUIWindow::GetHeight)
		.def("SetHeight",				&CUIWindow::SetHeight)

		.def("Enable",					&CUIWindow::Enable)
		.def("IsEnabled",				&CUIWindow::IsEnabled)
		.def("Show",					&CUIWindow::Show)
		.def("IsShown",					&CUIWindow::IsShown)
		.def("SetFont",					&CUIWindow::SetFont)
		.def("GetFont",					&CUIWindow::GetFont)

		.def("WindowName",				&CUIWindow::WindowName_script)
		.def("SetWindowName",			&CUIWindow::SetWindowName),
//		.def("",						&CUIWindow::)
		
		class_<CDialogHolder>("CDialogHolder")
		.def("MainInputReceiver",		&CDialogHolder::MainInputReceiver)
		.def("start_stop_menu",			&CDialogHolder::StartStopMenu)
		.def("AddDialogToRender",		&CDialogHolder::AddDialogToRender)
		.def("RemoveDialogToRender",	&CDialogHolder::RemoveDialogToRender),

		class_<CUIDialogWnd, CUIWindow>("CUIDialogWnd")
		.def("GetHolder",				&CUIDialogWnd::GetHolder)
		.def("SetHolder",				&CUIDialogWnd::SetHolder),

		class_<CUIFrameWindow, CUIWindow>("CUIFrameWindow")
		.def(					constructor<>())
		.def("SetWidth",				&CUIFrameWindow::SetWidth)
		.def("SetHeight",				&CUIFrameWindow::SetHeight)
		.def("SetColor",				&CUIFrameWindow::SetColor)
		.def("GetTitleStatic",			&CUIFrameWindow::GetTitleStatic)
		.def("Init",					(void(CUIFrameWindow::*)(LPCSTR,int,int,int,int))&CUIFrameWindow::Init)
		.def("InitLeftTop",				&CUIFrameWindow::InitLeftTop)
		.def("InitLeftBottom",			&CUIFrameWindow::InitLeftBottom),

		class_<CUIFrameLineWnd, CUIWindow>("CUIFrameLineWnd")
		.def(					constructor<>())
		.def("SetWidth",						&CUIFrameLineWnd::SetWidth)
		.def("SetHeight",						&CUIFrameLineWnd::SetHeight)
		.def("SetOrientation",					&CUIFrameLineWnd::SetOrientation)
		.def("SetColor",						&CUIFrameLineWnd::SetColor)
		.def("GetTitleStatic",					&CUIFrameLineWnd::GetTitleStatic)
		.def("Init",							(void(CUIFrameLineWnd::*)(LPCSTR,int,int,int,int,bool))&CUIFrameLineWnd::Init),
//		.def("",						&CUIFrameLineWnd::)
//		.def("",						&CUIFrameLineWnd::)
//		.def("",						&CUIFrameLineWnd::)

		class_<enum_exporter<EUIMessages> >("ui_events")
			.enum_("events")
			[
	// CUIWindow
				value("WINDOW_LBUTTON_DOWN",			int(WINDOW_LBUTTON_DOWN)),
				value("WINDOW_RBUTTON_DOWN",			int(WINDOW_RBUTTON_DOWN)),
				value("WINDOW_LBUTTON_UP",				int(WINDOW_LBUTTON_UP)),
				value("WINDOW_RBUTTON_UP",				int(WINDOW_RBUTTON_UP)),
				value("WINDOW_MOUSE_MOVE",				int(WINDOW_MOUSE_MOVE)),
				value("WINDOW_LBUTTON_DB_CLICK",		int(WINDOW_LBUTTON_DB_CLICK)),
				value("WINDOW_KEY_PRESSED",				int(WINDOW_KEY_PRESSED)),
				value("WINDOW_KEY_RELEASED",			int(WINDOW_KEY_RELEASED)),
				value("WINDOW_MOUSE_CAPTURE_LOST ",		int(WINDOW_MOUSE_CAPTURE_LOST )),
				value("WINDOW_KEYBOARD_CAPTURE_LOST",	int(WINDOW_KEYBOARD_CAPTURE_LOST)),


	// CUIStatic
				value("STATIC_FOCUS_RECEIVED",			int(STATIC_FOCUS_RECEIVED)),
				value("STATIC_FOCUS_LOST",				int(STATIC_FOCUS_LOST)),

	// CUIButton
				value("BUTTON_CLICKED",					int(BUTTON_CLICKED)),
				value("BUTTON_DOWN",					int(BUTTON_DOWN)),
				
	// CUITabControl
				value("TAB_CHANGED",					int(TAB_CHANGED)),

	// CUICheckButton
				value("CHECK_BUTTON_SET",				int(CHECK_BUTTON_SET)),
				value("CHECK_BUTTON_RESET",				int(CHECK_BUTTON_RESET)),
				
	// CUIRadioButton
				value("RADIOBUTTON_SET",				int(RADIOBUTTON_SET)),

	// CUIdragDropItem
				value("DRAG_DROP_ITEM_DRAG",			int(DRAG_DROP_ITEM_DRAG)),
				value("DRAG_DROP_ITEM_DROP ",			int(DRAG_DROP_ITEM_DROP )),
				value("DRAG_DROP_ITEM_MOVE",			int(DRAG_DROP_ITEM_MOVE)),
				value("DRAG_DROP_ITEM_DB_CLICK",		int(DRAG_DROP_ITEM_DB_CLICK)),
				value("DRAG_DROP_ITEM_RBUTTON_CLICK",	int(DRAG_DROP_ITEM_RBUTTON_CLICK)),
				value("DRAG_DROP_REFRESH_ACTIVE_ITEM",	int(DRAG_DROP_REFRESH_ACTIVE_ITEM)),


	// CUIScrollBox
				value("SCROLLBOX_MOVE",					int(SCROLLBOX_MOVE)),
				value("SCROLLBOX_STOP",					int(SCROLLBOX_STOP)),
				
	// CUIScrollBar
				value("SCROLLBAR_VSCROLL",				int(SCROLLBAR_VSCROLL)),
				value("SCROLLBAR_HSCROLL",				int(SCROLLBAR_HSCROLL)),

	// CUIListWnd
				value("LIST_ITEM_CLICKED",				int(LIST_ITEM_CLICKED)),
	
	// CUIInteractiveItem
				value("INTERACTIVE_ITEM_CLICK",			int(INTERACTIVE_ITEM_CLICK)),

	// UIPropertiesBox
				value("PROPERTY_CLICKED",				int(PROPERTY_CLICKED)),

	// CUIMessageBox
				value("MESSAGE_BOX_OK_CLICKED",			int(MESSAGE_BOX_OK_CLICKED)),
				value("MESSAGE_BOX_YES_CLICKED",		int(MESSAGE_BOX_YES_CLICKED)),
				value("MESSAGE_BOX_NO_CLICKED",			int(MESSAGE_BOX_NO_CLICKED)),
				value("MESSAGE_BOX_CANCEL_CLICKED",		int(MESSAGE_BOX_CANCEL_CLICKED)),

	// CUITalkDialogWnd
				value("TALK_DIALOG_TRADE_BUTTON_CLICKED",	int(TALK_DIALOG_TRADE_BUTTON_CLICKED)),
				value("TALK_DIALOG_QUESTION_CLICKED",		int(TALK_DIALOG_QUESTION_CLICKED)),

	// CUIMapBaground
				value("MAPSPOT_FOCUS_RECEIVED",				int(MAPSPOT_FOCUS_RECEIVED)),
				value("MAPSPOT_FOCUS_LOST",					int(MAPSPOT_FOCUS_LOST)),
				value("MAP_MOVED",							int(MAP_MOVED)),

	// CUIPdaWnd
				value("PDA_MAP_SET_ACTIVE_POINT",			int(PDA_MAP_SET_ACTIVE_POINT)),

	// CUIPdaDialogWnd
				value("PDA_DIALOG_WND_BACK_BUTTON_CLICKED",			int(PDA_DIALOG_WND_BACK_BUTTON_CLICKED)),
				value("PDA_DIALOG_WND_MESSAGE_BUTTON_CLICKED",		int(PDA_DIALOG_WND_MESSAGE_BUTTON_CLICKED)),

	// CUIPdaContactsWnd
				value("PDA_CONTACTS_WND_CONTACT_SELECTED",			int(PDA_CONTACTS_WND_CONTACT_SELECTED)),

	// CUITradeWnd
				value("TRADE_WND_CLOSED",							int(TRADE_WND_CLOSED)),

	// CUISleepWnd
				value("SLEEP_WND_PERFORM_BUTTON_CLICKED",			int(SLEEP_WND_PERFORM_BUTTON_CLICKED)),

	// CUIOutfitSlot
				value("UNDRESS_OUTFIT",								int(UNDRESS_OUTFIT)),
				value("OUTFIT_RETURNED_BACK",						int(OUTFIT_RETURNED_BACK)),

	// CUIArtifactMerger
				value("ARTEFACT_MERGER_CLOSE_BUTTON_CLICKED",		int(ARTEFACT_MERGER_CLOSE_BUTTON_CLICKED)),
				value("ARTEFACT_MERGER_PERFORM_BUTTON_CLICKED",		int(ARTEFACT_MERGER_PERFORM_BUTTON_CLICKED)),

	// CUIInventroyWnd
				value("INVENTORY_DROP_ACTION",						int(INVENTORY_DROP_ACTION)),
				value("INVENTORY_EAT_ACTION",						int(INVENTORY_EAT_ACTION)),
				value("INVENTORY_TO_BELT_ACTION ",					int(INVENTORY_TO_BELT_ACTION )),
				value("INVENTORY_TO_SLOT_ACTION",					int(INVENTORY_TO_SLOT_ACTION)),
				value("INVENTORY_TO_BAG_ACTION",					int(INVENTORY_TO_BAG_ACTION)),
				value("INVENTORY_ARTEFACT_MERGER_ACTIVATE",			int(INVENTORY_ARTEFACT_MERGER_ACTIVATE)),
				value("INVENTORY_ARTEFACT_MERGER_DEACTIVATE",		int(INVENTORY_ARTEFACT_MERGER_DEACTIVATE)),
				value("INVENTORY_ATTACH_ADDON ",					int(INVENTORY_ATTACH_ADDON )),
				value("INVENTORY_DETACH_SCOPE_ADDON",				int(INVENTORY_DETACH_SCOPE_ADDON)),
				value("INVENTORY_DETACH_SILENCER_ADDON",			int(INVENTORY_DETACH_SILENCER_ADDON)),
				value("INVENTORY_DETACH_GRENADE_LAUNCHER_ADDON",	int(INVENTORY_DETACH_GRENADE_LAUNCHER_ADDON))
			]
	];
}