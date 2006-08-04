#pragma once
#include "stdafx.h"
#include "UIBuyWnd.h"
#include "xrXMLParser.h"
#include "UIXmlInit.h"
#include "UICellCustomItems.h"
#include "UIDragDropListEx.h"
#include "../ui_base.h"
#include "../UICursor.h"
#include "../customoutfit.h"
#include "../scope.h"
#include "../silencer.h"
#include "../grenadelauncher.h"
#include "../game_cl_Deathmatch.h"
#include "UIOutfitSlot.h"
#include "UIListBoxItem.h"
#include <dinput.h>

#define 	BELT_SLOT			5

CUIBuyWnd::CUIBuyWnd(){
	// background textures
	AttachChild(&m_slotsBack);
	AttachChild(&m_back);

	// buttons
	AttachChild(&m_btnOk);
	AttachChild(&m_btnCancel);
	AttachChild(&m_btnClear);

	AttachChild(&m_btnPistolBullet);
	AttachChild(&m_btnPistolSilencer);
	AttachChild(&m_btnRifleBullet);
	AttachChild(&m_btnRifleSilencer);
	AttachChild(&m_btnRifleScope);
	AttachChild(&m_btnRifleGrenadelauncer);
	AttachChild(&m_btnRifleGrenade);

	//	text
	AttachChild(&m_moneyInfo);
	

	// controls
	AttachChild(&m_tab);
	AttachChild(&m_bag);
	AttachChild(&m_itemInfo);
	m_itemInfo.AttachChild(&m_rankInfo);

	for (int i = 0; i < MP_SLOT_NUM; i++)
	{
		if (MP_SLOT_OUTFIT == i)
			m_list[i] = xr_new<CUIOutfitDragDropList>();
		else
			m_list[i] = xr_new<CUIDragDropListEx>();
        AttachChild(m_list[i]);
	}

	AttachChild				(&m_propertiesBox);
	m_propertiesBox.Init	(0,0,300,300);
	m_propertiesBox.Hide	();

	m_pCurrentCellItem = NULL;
}

CUIBuyWnd::~CUIBuyWnd(){

}

void CUIBuyWnd::Init(LPCSTR sectionName, LPCSTR sectionPrice){
	m_sectionName = sectionName;
	m_sectionPrice = sectionPrice;

	CUIXml xml_doc;
	R_ASSERT(xml_doc.Init(CONFIG_PATH, UI_PATH, "inventoryMP.xml"));

	CUIWindow::Init(0,0,UI_BASE_WIDTH,UI_BASE_WIDTH);

	// background textures
	CUIXmlInit::InitStatic(xml_doc, "slots_background", 0, &m_slotsBack);
	CUIXmlInit::InitStatic(xml_doc, "background", 0, &m_back);

	// buttons
	CUIXmlInit::Init3tButton(xml_doc, "ok_button", 0, &m_btnOk);
	CUIXmlInit::Init3tButton(xml_doc, "cancel_button", 0, &m_btnCancel);
	CUIXmlInit::Init3tButton(xml_doc, "clear_button", 0, &m_btnClear);

	CUIXmlInit::Init3tButton(xml_doc, "btn_pistol_silencer",		0, &m_btnPistolSilencer);
	CUIXmlInit::Init3tButton(xml_doc, "btn_pistol_bullets",			0, &m_btnPistolBullet);
	CUIXmlInit::Init3tButton(xml_doc, "btn_rifle_scope",			0, &m_btnRifleScope);
	CUIXmlInit::Init3tButton(xml_doc, "btn_rifle_silencer",			0, &m_btnRifleSilencer);
	CUIXmlInit::Init3tButton(xml_doc, "btn_riffle_granadelauncher",	0, &m_btnRifleGrenadelauncer);
	CUIXmlInit::Init3tButton(xml_doc, "btn_riffle_granades",		0, &m_btnRifleGrenade);
	CUIXmlInit::Init3tButton(xml_doc, "btn_rifle_bullets",			0, &m_btnRifleBullet);

    // hide buttons... (we still have some design problems)
	m_btnPistolSilencer.Enable(false);
	m_btnPistolSilencer.SetVisible(false);
	m_btnRifleSilencer.Enable(false);
	m_btnRifleSilencer.SetVisible(false);
	m_btnRifleScope.Enable(false);
	m_btnRifleScope.SetVisible(false);
	m_btnRifleGrenadelauncer.Enable(false);
	m_btnRifleGrenadelauncer.SetVisible(false);

	// text
	CUIXmlInit::InitStatic(xml_doc, "money_info", 0, &m_moneyInfo);
	

	// controls
	m_tab.Init(&xml_doc, "tab");
	m_bag.Init(xml_doc, "bag_background_static", sectionName, sectionPrice);
	CUIXmlInit::InitDragDropListEx(xml_doc, "pistol_list",	0, m_list[MP_SLOT_PISTOL]);
	CUIXmlInit::InitDragDropListEx(xml_doc, "rifle_list",	0, m_list[MP_SLOT_RIFLE]);
	CUIXmlInit::InitDragDropListEx(xml_doc, "belt_list",	0, m_list[MP_SLOT_BELT]);
	CUIXmlInit::InitDragDropListEx(xml_doc, "outfit_list",	0, m_list[MP_SLOT_OUTFIT]);

	BindDragDropListEvents(m_list[MP_SLOT_PISTOL]);
	BindDragDropListEvents(m_list[MP_SLOT_RIFLE]);
	BindDragDropListEvents(m_list[MP_SLOT_BELT]);
	BindDragDropListEvents(m_list[MP_SLOT_OUTFIT]);

	m_list[MP_SLOT_OUTFIT]->SetItem	(NULL);

	CUIXmlInit::InitWindow(xml_doc, "desc_static", 0, &m_itemInfo);
	CUIXmlInit::InitStatic(xml_doc, "desc_static:rank_icon", 0, &m_rankInfo);

	m_itemInfo.Init("buy_mp_item.xml");

	
}

bool CUIBuyWnd::OnKeyboard(int dik, EUIMessages keyboard_action){
	if (WINDOW_KEY_RELEASED == keyboard_action) 
		return true;

	if (m_propertiesBox.GetVisible())
		m_propertiesBox.OnKeyboard(dik, keyboard_action);

	switch (m_bag.GetMenuLevel())
	{
	case mlRoot:
		if (m_tab.OnKeyboard(dik, keyboard_action))
			return true;
		break;
	default:
		if (m_bag.OnKeyboard(dik, keyboard_action))
			return true;
	}

	if (DIK_ESCAPE == dik)
		m_btnCancel.OnClick();
	else if (DIK_SPACE == dik)
		m_btnOk.OnClick();
	else if (DIK_B == dik)
		m_tab.SetActiveState();
	else if (DIK_C == dik)
		m_btnClear.OnClick();
	else if (DIK_Q == dik)
		m_btnPistolBullet.OnClick();
	else if (DIK_W == dik)
		m_btnRifleBullet.OnClick();
	else if (DIK_E == dik)
		m_btnRifleGrenade.OnClick();

	return true;
}

void CUIBuyWnd::SendMessage(CUIWindow* pWnd, s16 msg, void* pData){
	switch (msg)
	{
	case TAB_CHANGED:
        OnTabChange();	break;
	case XR_MENU_LEVEL_CHANGED:
		OnMenuLevelChange(); break;
	case BUTTON_CLICKED:
		if (&m_btnOk == pWnd)
			OnBtnOk();
		else if (&m_btnCancel == pWnd)
			OnBtnCancel();
		else if (&m_btnClear == pWnd)
			OnBtnClear();
		else if (&m_btnPistolBullet == pWnd)
			OnBtnBulletBuy(MP_SLOT_PISTOL);
		else if (&m_btnRifleBullet == pWnd)
			OnBtnBulletBuy(MP_SLOT_RIFLE);
		else if (&m_btnRifleGrenade)
			OnBtnRifleGrenade();
		break;
	case PROPERTY_CLICKED:
        if(pWnd == &m_propertiesBox)
            ProcessPropertiesBoxClicked();
		break;
	case MP_MONEY_CHANGE:
		OnMoneyChange();
		break;
	}
}

#include "../../xr_input.h"
void CUIBuyWnd::OnBtnBulletBuy(int slot){
	if (m_list[slot]->ItemsCount()){
		CUICellItem*	itm		= m_list[slot]->GetItemIdx(0);
		CInventoryItem*	iitm	= (CInventoryItem*)itm->m_pData;

		R_ASSERT(pSettings->section_exist(*iitm->object().cNameSect()));

		shared_str itemsList; 
		string256 single_item;

		itemsList = pSettings->r_string(*iitm->object().cNameSect(), "ammo_class");
		int n = 0;
		if (pInput->iGetAsyncKeyState(DIK_LSHIFT))
			n = 1;
		if (_GetItemCount(itemsList.c_str())<2)
			n = 0;
		_GetItem(itemsList.c_str(), n, single_item);


		CUICellItem* ammo = m_bag.GetItemBySectoin(single_item);
		if (m_bag.CanBuy(ammo)){
			if (ammo)
				ammo->GetMessageTarget()->SendMessage(ammo, DRAG_DROP_ITEM_DB_CLICK, NULL);	
		}
	}
}

void CUIBuyWnd::HightlightCurrAmmo(){
	m_bag.ClearAmmoHighlight();
	Highlight(MP_SLOT_PISTOL);
	Highlight(MP_SLOT_RIFLE);
}

void CUIBuyWnd::Highlight(int slot){
	R_ASSERT(MP_SLOT_PISTOL == slot || MP_SLOT_RIFLE == slot);

	
	CUICellItem*	item	= NULL;
	CInventoryItem*	iitem	= NULL;

	if (m_list[slot]->ItemsCount())
	{
		item = m_list[slot]->GetItemIdx(0);
		iitem = (CInventoryItem*)item->m_pData;
	}
	else
		return;


	R_ASSERT(pSettings->section_exist(*iitem->object().cNameSect()));

	shared_str itemsList; 
	string256 single_item;

	itemsList = pSettings->r_string(*iitem->object().cNameSect(), "ammo_class");
		
	int c = _GetItemCount(itemsList.c_str());
	for (int i = 0; i<c; i++){
        _GetItem(itemsList.c_str(), i, single_item);
		m_bag.HightlightAmmo(single_item);
	}
}

void CUIBuyWnd::OnBtnRifleGrenade(){
	if (m_list[MP_SLOT_RIFLE]->ItemsCount()){
		CUICellItem*	itm		= m_list[MP_SLOT_RIFLE]->GetItemIdx(0);
		CWeapon*		wpn		= (CWeapon*)itm->m_pData;

		if (wpn->IsGrenadeLauncherAttached()){
            shared_str itemsList; 
			string256 single_item;

			itemsList = pSettings->r_string(*wpn->cNameSect(), "grenade_class");
			_GetItem(*itemsList, 0, single_item);

			CUICellItem* grenade = m_bag.GetItemBySectoin(single_item);
			if (m_bag.CanBuy(grenade)){
				if (grenade)
					grenade->GetMessageTarget()->SendMessage(grenade, DRAG_DROP_ITEM_DB_CLICK, NULL);	
			}
		}
	}
}

void CUIBuyWnd::OnMoneyChange(){
	string256 buff;
	itoa(m_bag.m_money, buff, 10);
	m_moneyInfo.SetText(buff);
}

void CUIBuyWnd::AfterBuy(){
	int iActiveIndex = m_tab.GetActiveIndex();

	if (2 == iActiveIndex || 4 == iActiveIndex)
	{
		; // do nothing
	}
	else
		m_tab.SetActiveState();
}

void CUIBuyWnd::ProcessPropertiesBoxClicked	()
{
	if(m_propertiesBox.GetClickedItem())
	{
		CUICellItem* itm = NULL;
		switch(m_propertiesBox.GetClickedItem()->GetID())
		{
		//case INVENTORY_SELL_ITEM:
		//	SellItem();
		//	break;
		case INVENTORY_TO_SLOT_ACTION:	
			ToSlot(CurrentItem(), true);
			break;
		case INVENTORY_TO_BELT_ACTION:	
			ToBelt(CurrentItem(),false);
			break;
		case INVENTORY_TO_BAG_ACTION:	
			ToBag(CurrentItem(),false);
			break;
		//case INVENTORY_DROP_ACTION:
		//	{
		//		void* d = UIPropertiesBox.GetClickedItem()->GetData();
		//		bool b_all = (d==(void*)33);

		//		DropCurrentItem(b_all);
		//	}break;
		//case INVENTORY_EAT_ACTION:
		//	EatCurrentItem();
		//	break;
		case INVENTORY_ATTACH_ADDON:
			AttachAddon((PIItem)(m_propertiesBox.GetClickedItem()->GetData()));
			break;

		case INVENTORY_ATTACH_SCOPE_ADDON:
			itm = (CUICellItem*)(m_propertiesBox.GetClickedItem()->GetData());
			m_bag.AttachAddon(itm, SCOPE, false);
			break;
		case INVENTORY_DETACH_SCOPE_ADDON:
			DetachAddon(*(smart_cast<CWeapon*>(CurrentIItem()))->GetScopeName());
			break;

		case INVENTORY_ATTACH_SILENCER_ADDON:
			itm = (CUICellItem*)(m_propertiesBox.GetClickedItem()->GetData());
			m_bag.AttachAddon(itm, SILENCER, false);
			break;
		case INVENTORY_DETACH_SILENCER_ADDON:
			DetachAddon(*(smart_cast<CWeapon*>(CurrentIItem()))->GetSilencerName());
			break;

		case INVENTORY_ATTACH_GRENADE_LAUNCHER_ADDON:
			itm = (CUICellItem*)(m_propertiesBox.GetClickedItem()->GetData());
			m_bag.AttachAddon(itm, GRENADELAUNCHER, false);
			break;
		case INVENTORY_DETACH_GRENADE_LAUNCHER_ADDON:
			DetachAddon(*(smart_cast<CWeapon*>(CurrentIItem()))->GetGrenadeLauncherName());
			break;
		//case INVENTORY_ACTIVATE_ARTEFACT_ACTION:
		//	Activate_Artefact();
		//	break;
	/*	case INVENTORY_RELOAD_MAGAZINE:
			(smart_cast<CWeapon*>(CurrentIItem()))->Action(kWPN_RELOAD, CMD_START);
			break;
		case INVENTORY_UNLOAD_MAGAZINE:
			{
				CUICellItem * itm = CurrentItem();
				(smart_cast<CWeaponMagazined*>((CWeapon*)itm->m_pData))->UnloadMagazine();
				for(u32 i=0; i<itm->ChildsCount(); ++i)
				{
					CUICellItem * child_itm			= itm->Child(i);
					(smart_cast<CWeaponMagazined*>((CWeapon*)child_itm->m_pData))->UnloadMagazine();
				}
			}break;*/
		}
	
	}
}

void CUIBuyWnd::OnBtnOk(){
	if (!CanBuyAllItems())
        return;

	Game().StartStopMenu(this,true);
	game_cl_Deathmatch * dm = smart_cast<game_cl_Deathmatch *>(&(Game()));
	dm->OnBuyMenu_Ok();
}

void CUIBuyWnd::OnBtnCancel(){
	Game().StartStopMenu(this,true);
}

bool CUIBuyWnd::ClearTooExpensiveItems(){
	bool f = false;
	bool res;

    res = ClearSlot_ifTooExpensive(MP_SLOT_PISTOL);
	f = f ? true : res;
	res = ClearSlot_ifTooExpensive(MP_SLOT_RIFLE);
	f = f ? true : res;
	res = ClearSlot_ifTooExpensive(MP_SLOT_OUTFIT);
	f = f ? true : res;

	u32 c = m_list[MP_SLOT_BELT]->ItemsCount();

	for (u32 i = 0; i < c; i++)
	{
		CUICellItem* itm = m_list[MP_SLOT_BELT]->GetItemIdx(i);
		if (!m_bag.IsActive(itm))
		{
			itm->GetMessageTarget()->SendMessage(itm, DRAG_DROP_ITEM_DB_CLICK, NULL);
			f = true;
		}
	}

	return f;
}

bool CUIBuyWnd::ClearSlot_ifTooExpensive(int slot){
	R_ASSERT(slot <= MP_SLOT_NUM);

	if (m_list[slot]->ItemsCount())
	{
		CUICellItem *itm = m_list[slot]->GetItemIdx(0);
		if (!m_bag.IsActive(itm))
		{
			itm->GetMessageTarget()->SendMessage(itm, DRAG_DROP_ITEM_DB_CLICK, NULL);
			return true;
		}
	}

	return false;
}

bool CUIBuyWnd::SlotToSection(int slot){
	R_ASSERT(slot <= MP_SLOT_NUM);

	while (m_list[slot]->ItemsCount())
	{
		CUICellItem *itm = m_list[slot]->GetItemIdx(0);
		itm->GetMessageTarget()->SendMessage(itm, DRAG_DROP_ITEM_DB_CLICK, NULL);
	}

	return true;
}

void CUIBuyWnd::OnBtnClear(){
    if (ClearTooExpensiveItems())
		return;

	SlotToSection(MP_SLOT_PISTOL);
	SlotToSection(MP_SLOT_RIFLE);
	SlotToSection(MP_SLOT_BELT);
	SlotToSection(MP_SLOT_OUTFIT);

	game_cl_Deathmatch * dm = smart_cast<game_cl_Deathmatch *>(&(Game()));
	if (dm) 
		dm->OnBuyMenu_DefaultItems();
}

void CUIBuyWnd::OnTabChange(){
	m_bag.ShowSection(m_tab.GetActiveIndex());
}

void CUIBuyWnd::OnMenuLevelChange(){
	if (m_bag.GetMenuLevel() == mlRoot)
		m_tab.SetActiveState();
}

CUICellItem* CUIBuyWnd::CurrentItem()
{
	return m_pCurrentCellItem;
}

CInventoryItem* CUIBuyWnd::CurrentIItem()
{
	return	(m_pCurrentCellItem)?(CInventoryItem*)m_pCurrentCellItem->m_pData : NULL;
}

void CUIBuyWnd::SetCurrentItem(CUICellItem* itm)
{
	if(m_pCurrentCellItem == itm) return;
	m_pCurrentCellItem				= itm;

	if (m_pCurrentCellItem)
	{
		string256			str;
		m_itemInfo.InitItem			(CurrentIItem());
		sprintf				(str, "%d RU", m_bag.GetItemPrice(itm));
		m_itemInfo.UICost->SetText(str);

		string64 tex_name;
		string64 team;
		if (m_bag.IsBlueTeamItem(itm))
			strcpy(team, "blue");
		else 
			strcpy(team, "green");

		sprintf(tex_name, "ui_hud_status_%s_0%d", team, m_bag.GetItemRank(m_pCurrentCellItem)+1);
				
		//strconcat(tex_name,"ui_hud_status_",itoa(pDDItemMP->m_iRank,foo,10));
		m_rankInfo.InitTexture(tex_name);
	}
}

#include "Restrictions.h"
extern CRestrictions g_mp_restrictions;

bool CUIBuyWnd::OnItemStartDrag(CUICellItem* itm)
{
	return !m_bag.CanBuy(itm);
}

bool CUIBuyWnd::OnItemSelected(CUICellItem* itm)
{
	SetCurrentItem		(itm);
	return				false;
}

void CUIBuyWnd::BindDragDropListEvents(CUIDragDropListEx* lst)
{
	lst->m_f_item_drop				= CUIDragDropListEx::DRAG_DROP_EVENT(this,&CUIBuyWnd::OnItemDrop);
//	lst->m_f_item_start_drag		= CUIDragDropListEx::DRAG_DROP_EVENT(this,&CUIBuyWnd::OnItemStartDrag);
	lst->m_f_item_db_click			= CUIDragDropListEx::DRAG_DROP_EVENT(this,&CUIBuyWnd::OnItemDbClick);
	lst->m_f_item_selected			= CUIDragDropListEx::DRAG_DROP_EVENT(this,&CUIBuyWnd::OnItemSelected);
	lst->m_f_item_rbutton_click		= CUIDragDropListEx::DRAG_DROP_EVENT(this,&CUIBuyWnd::OnItemRButtonClick);
}

bool CUIBuyWnd::OnItemDrop(CUICellItem* itm)
{
	CUIDragDropListEx*	old_owner		= itm->OwnerList();
	CUIDragDropListEx*	new_owner		= CUIDragDropListEx::m_drag_item->BackList();
	if(old_owner==new_owner || !old_owner || !new_owner)
					return false;

	EListType t_new		= GetType(new_owner);
	EListType t_old		= GetType(old_owner);
	if(t_new == t_old)	return true;

	switch(t_new){
		case iwSlot:{
			if(GetSlotList(GetLocalSlot(CurrentIItem()->GetSlot()))==new_owner)
				ToSlot	(itm, true);
		}break;
		case iwBag:{
			ToBag	(itm, true);
		}break;
		case iwBelt:{
			ToBelt	(itm, true);
		}break;
	};

	return true;
}

CUIDragDropListEx* CUIBuyWnd::GetSlotList(u32 slot_idx)
{
//	if(slot_idx == NO_ACTIVE_SLOT || GetInventory()->m_slots[slot_idx].m_bPersistent)	return NULL;
	switch (slot_idx)
	{
		case MP_SLOT_PISTOL:
			return m_list[MP_SLOT_PISTOL];
			break;

		case MP_SLOT_RIFLE:
			return m_list[MP_SLOT_RIFLE];
			break;

		case MP_SLOT_OUTFIT:
			return m_list[MP_SLOT_OUTFIT];
			break;
		case MP_SLOT_BELT:
			return m_list[MP_SLOT_BELT];
			break;
		default:
			NODEFAULT;

	};
	return NULL;
}

EListType CUIBuyWnd::GetType(CUIDragDropListEx* l)
{
	if(l==m_list[MP_SLOT_BELT])		
		return iwBelt;
	if(l==m_list[MP_SLOT_PISTOL])
		return iwSlot;
	if(l==m_list[MP_SLOT_RIFLE])
		return iwSlot;
	if(l==m_list[MP_SLOT_OUTFIT])
		return iwSlot;
	if(m_bag.IsChild(l))
		return iwBag;

	NODEFAULT;
	return iwSlot;
}


bool CUIBuyWnd::OnItemDbClick(CUICellItem* itm)
{

		CUIDragDropListEx*	old_owner		= itm->OwnerList();
		EListType t_old						= GetType(old_owner);

		switch(t_old){
			case iwSlot:{
				ToBag	(itm, false);
			}break;

			case iwBag:{
				if (m_bag.CanBuy(itm))
				{
					if(!ToSlot(itm, false))
					{
						if( !ToBelt(itm, false) )
							ToSlot	(itm, true);
					}
				}
				else
					return false;
			}break;

			case iwBelt:{
				ToBag	(itm, false);
			}break;
		};

		return true;
}


bool CUIBuyWnd::OnItemRButtonClick(CUICellItem* itm)
{
	SetCurrentItem				(itm);
	ActivatePropertiesBox		();
	return						false;
}

MP_BUY_SLOT CUIBuyWnd::GetLocalSlot(u32 slot){
	switch (slot){
		case PISTOL_SLOT: return MP_SLOT_PISTOL;
		case RIFLE_SLOT:	return MP_SLOT_RIFLE;		
		case OUTFIT_SLOT:	return MP_SLOT_OUTFIT;
		default:			/*NODEFAULT;*/ return MP_SLOT_BELT;
	}
}

void CUIBuyWnd::ActivatePropertiesBox()
{
	float x,y;
	// ����-������� ��� ���������� ������ ������������ ����: Dreess Outfit, ���� ������ ��� �����
	bool bAlreadyDressed = false; 

	Frect rect = GetAbsoluteRect();
	GetUICursor()->GetPos(x,y);
		
	m_propertiesBox.RemoveAll();
	
	CCustomOutfit* pOutfit				= smart_cast<CCustomOutfit*>	(CurrentIItem());
	CWeapon* pWeapon					= smart_cast<CWeapon*>			(CurrentIItem());
	CScope* pScope						= smart_cast<CScope*>			(CurrentIItem());
	CSilencer* pSilencer				= smart_cast<CSilencer*>		(CurrentIItem());
	CGrenadeLauncher* pGrenadeLauncher	= smart_cast<CGrenadeLauncher*>	(CurrentIItem());
    


	if((pWeapon || pOutfit ) && m_bag.IsInBag(CurrentItem()) /*&& m_list[GetLocalSlot(CurrentIItem()->GetSlot())]->ItemsCount()*/)
	{
		m_propertiesBox.AddItem("st_move_to_slot",  NULL, INVENTORY_TO_SLOT_ACTION);
	}
	if(CurrentIItem()->Belt() && CanPutInBelt(CurrentIItem()))
	{
		m_propertiesBox.AddItem("st_move_on_belt",  NULL, INVENTORY_TO_BELT_ACTION);
	}
	if(!m_bag.IsInBag(CurrentItem()))
	{
		if(!pOutfit)
			m_propertiesBox.AddItem("st_move_to_bag",  NULL, INVENTORY_TO_BAG_ACTION);
		else
			m_propertiesBox.AddItem("st_undress_outfit",  NULL, INVENTORY_TO_BAG_ACTION);
		bAlreadyDressed = true;
	}
	if(pOutfit  && !bAlreadyDressed )
	{
		m_propertiesBox.AddItem("Dress in outfit",  NULL, INVENTORY_TO_SLOT_ACTION);
	}
	//
	//������������ ������� �� ����
	if(pWeapon)
	{
		if(pWeapon->GrenadeLauncherAttachable())
		{
			if (pWeapon->IsGrenadeLauncherAttached())
                m_propertiesBox.AddItem("st_detach_gl", GetRifle(), INVENTORY_DETACH_GRENADE_LAUNCHER_ADDON);
			else if (m_list[MP_SLOT_RIFLE]->IsOwner(CurrentItem())){

				if (m_bag.CanBuy(*pWeapon->GetGrenadeLauncherName()))
                    m_propertiesBox.AddItem("st_attach_gl_to_rifle", CurrentItem(), INVENTORY_ATTACH_GRENADE_LAUNCHER_ADDON);
			}

		}
		if(pWeapon->ScopeAttachable())
		{
			if (pWeapon->IsScopeAttached())
                m_propertiesBox.AddItem("st_detach_scope",  NULL, INVENTORY_DETACH_SCOPE_ADDON);
			else{

				if (m_bag.CanBuy(*pWeapon->GetScopeName()))
				{
					if (m_list[MP_SLOT_PISTOL]->IsOwner(CurrentItem()))
						m_propertiesBox.AddItem("st_attach_scope_to_pistol",  CurrentItem(), INVENTORY_DETACH_SCOPE_ADDON);
					else if (m_list[MP_SLOT_RIFLE]->IsOwner(CurrentItem()))
						m_propertiesBox.AddItem("st_attach_scope_to_rifle",  CurrentItem(), INVENTORY_ATTACH_SCOPE_ADDON);
				}
			}
		}
		if(pWeapon->SilencerAttachable())
		{
			if (pWeapon->IsSilencerAttached())
                m_propertiesBox.AddItem("st_detach_silencer",  NULL, INVENTORY_DETACH_SILENCER_ADDON);
			else {

				if (m_bag.CanBuy(*pWeapon->GetSilencerName()))
				{
					if (m_list[MP_SLOT_PISTOL]->IsOwner(CurrentItem()))
						m_propertiesBox.AddItem("st_attach_silencer_to_pistol",  CurrentItem(), INVENTORY_DETACH_SILENCER_ADDON);
					else if (m_list[MP_SLOT_RIFLE]->IsOwner(CurrentItem()))
						m_propertiesBox.AddItem("st_attach_silencer_to_rifle",  CurrentItem(), INVENTORY_ATTACH_SILENCER_ADDON);
				}
			}
		}
	}
	if(pScope)
	{
		CInventoryItem* pIItem = NULL;
		if (m_list[MP_SLOT_PISTOL]->ItemsCount())
		{
			pIItem = (CInventoryItem*)((m_list[MP_SLOT_PISTOL]->GetItemIdx(0))->m_pData);

			if (pIItem->CanAttach(pScope))
				m_propertiesBox.AddItem("st_attach_scope_to_pistol",  (void*)pIItem, INVENTORY_ATTACH_ADDON);

		}
		if (m_list[MP_SLOT_RIFLE]->ItemsCount())
		{
			pIItem = (CInventoryItem*)((m_list[MP_SLOT_RIFLE]->GetItemIdx(0))->m_pData);

			if (pIItem->CanAttach(pScope))
				m_propertiesBox.AddItem("st_attach_scope_to_rifle",  (void*)pIItem, INVENTORY_ATTACH_ADDON);

		}		
	}
	else if(pSilencer)
	{
		CInventoryItem* pIItem = NULL;
		if (m_list[MP_SLOT_PISTOL]->ItemsCount())
		{
			pIItem = (CInventoryItem*)((m_list[MP_SLOT_PISTOL]->GetItemIdx(0))->m_pData);

			if (pIItem->CanAttach(pSilencer))
				m_propertiesBox.AddItem("st_attach_silencer_to_pistol",  (void*)pIItem, INVENTORY_ATTACH_ADDON);
		}

		if (m_list[MP_SLOT_RIFLE]->ItemsCount())
		{
			pIItem = (CInventoryItem*)((m_list[MP_SLOT_RIFLE]->GetItemIdx(0))->m_pData);

			if (pIItem->CanAttach(pSilencer))
				m_propertiesBox.AddItem("st_attach_silencer_to_rifle",  (void*)pIItem, INVENTORY_ATTACH_ADDON);
		}
	}
	else if(pGrenadeLauncher)
	{
		CInventoryItem* pIItem = NULL;
		if (m_list[MP_SLOT_RIFLE]->ItemsCount())
		{
			pIItem = (CInventoryItem*)((m_list[MP_SLOT_RIFLE]->GetItemIdx(0))->m_pData);

			if (pIItem->CanAttach(pGrenadeLauncher))
				m_propertiesBox.AddItem("st_attach_silencer_to_rifle",  (void*)pIItem, INVENTORY_ATTACH_ADDON);
		}
	}
	else if (pOutfit)
	{

	}

	m_propertiesBox.AutoUpdateSize	();
	m_propertiesBox.BringAllToTop	();
	m_propertiesBox.Show			(x-rect.left, y-rect.top);
//	PlaySnd							(eInvProperties);
}

bool CUIBuyWnd::CanPutInSlot(CInventoryItem* iitm){
	if (PISTOL_SLOT == iitm->GetSlot() || RIFLE_SLOT == iitm->GetSlot() || OUTFIT_SLOT == iitm->GetSlot())
	{
		u32 _slot = GetLocalSlot(iitm->GetSlot());
		return m_list[_slot]->ItemsCount() == 0;
	}
	else 
		return false;
}

bool CUIBuyWnd::CanPutInBag	(CInventoryItem* iitm){
	return true;
}

bool CUIBuyWnd::CanPutInBelt(CInventoryItem* iitem){
	if (PISTOL_SLOT == iitem->GetSlot())
		return false;
	else if (RIFLE_SLOT == iitem->GetSlot())
		return false;
	else if (OUTFIT_SLOT == iitem->GetSlot())
		return false;
	return true;
}

CWeapon* CUIBuyWnd::GetPistol(){
	if (m_list[MP_SLOT_PISTOL]->ItemsCount() > 0)
	{
		CWeapon* pistol = (CWeapon*) (m_list[MP_SLOT_PISTOL]->GetItemIdx(0)->m_pData);
		VERIFY(pistol);
		return pistol;
	}
	else return NULL;
}

CWeapon* CUIBuyWnd::GetRifle(){
    if (m_list[MP_SLOT_RIFLE]->ItemsCount() > 0)
	{
		CWeapon* rifle = (CWeapon*) (m_list[MP_SLOT_RIFLE]->GetItemIdx(0)->m_pData);
		VERIFY(rifle);
		return rifle;
	}
	else return NULL;
}

bool CUIBuyWnd::ToSlot(CUICellItem* itm, bool force_place)
{
//	CUIDragDropListEx*	old_owner			= itm->OwnerList();
	PIItem	iitem							= (PIItem)itm->m_pData;
	u32 _slot								= GetLocalSlot(iitem->GetSlot());

	CScope* pScope						= smart_cast<CScope*>			(iitem);
	CSilencer* pSilencer				= smart_cast<CSilencer*>		(iitem);
	CGrenadeLauncher* pGrenadeLauncher	= smart_cast<CGrenadeLauncher*>	(iitem);

	CWeapon*	pPistol = GetPistol();
	CWeapon*	pRifle	= GetRifle();
	
	if (pSilencer){		// try to buy silencer
		if (pRifle && pRifle->SilencerAttachable() && !pRifle->IsSilencerAttached()){
			VERIFY(pRifle->Attach(pSilencer));	// ATTACH
			m_bag.BuyItem(itm);					// BUY
			return true;
		}
		
		if (pPistol && pPistol->SilencerAttachable() && !pPistol->IsSilencerAttached()){
			VERIFY(pPistol->Attach(pSilencer));	// ATTACH
			m_bag.BuyItem(itm);					// BUY
			return true;
		}
	}
	if (pScope){
		if (pRifle && pRifle->ScopeAttachable() && !pRifle->IsScopeAttached()){
			VERIFY(pRifle->Attach(pScope));		// ATTACH
			m_bag.BuyItem(itm);					// BUY
			return true;
		}

		if (pPistol && pPistol->ScopeAttachable() && !pPistol->IsScopeAttached()){
			VERIFY(pPistol->Attach(pScope));	// ATTACH
			m_bag.BuyItem(itm);					// BUY
			return true;
		}
	}
	if (pGrenadeLauncher && pRifle && pRifle->GrenadeLauncherAttachable() && !pRifle->IsGrenadeLauncherAttached()){
		VERIFY(pRifle->Attach(pGrenadeLauncher));
		m_bag.BuyItem(itm);
		return true;
	}

	if(CanPutInSlot(iitem)){
//		CUIDragDropListEx* new_owner		= GetSlotList(_slot);
//		VERIFY								(new_owner);

//		CUICellItem* i						= old_owner->RemoveItem(itm, (old_owner==new_owner) );
		
//		new_owner->SetItem					(i);
//		m_bag.BuyItem(itm);

		CUIDragDropListEx* slot				= GetSlotList(_slot);
		VERIFY								(slot);

		CUICellItem* i = m_bag.CreateItem(*iitem->object().cNameSect());
		i->m_index = itm->m_index;

		slot->SetItem(i);
		m_bag.BuyItem(i);

		AfterBuy();
		HightlightCurrAmmo();
		SetCurrentItem(itm);

		return								true;
	}else
	{ // in case slot is busy
		if(!force_place ||  iitem->GetSlot()==NO_ACTIVE_SLOT ) return false;

		CUIDragDropListEx* slot	= GetSlotList(_slot);
		VERIFY					(slot->ItemsCount()==1);

		CUICellItem* i			= slot->GetItemIdx(0);
		slot->RemoveItem		(i, true);

		m_bag.SellItem(i);
		m_bag.DestroyItem(i);

//		bool result							= ToBag(slot_cell, false);
//		VERIFY								(result);

		return ToSlot						(itm, false);
	}
}

bool CUIBuyWnd::ToBag(CUICellItem* itm, bool b_use_cursor_pos)
{
	PIItem	iitem						= (PIItem)itm->m_pData;

	if(CanPutInBag(iitem))
	{
		// if it is Pistol, Rifle or Outfit
		if (MP_SLOT_BELT != GetLocalSlot(iitem->GetSlot()))
		{
			CUIDragDropListEx* slot	= GetSlotList(GetLocalSlot(iitem->GetSlot()));
			VERIFY					(slot->ItemsCount()==1);

			CUICellItem* i			= slot->GetItemIdx(0);
			slot->RemoveItem		(i, MP_SLOT_OUTFIT!= GetLocalSlot(iitem->GetSlot()));

			if (MP_SLOT_OUTFIT != GetLocalSlot(iitem->GetSlot()))
			{
				SetCurrentItem(itm);								// set current item
				CWeapon* wpn = (CWeapon*)CurrentIItem();
				if (wpn->SilencerAttachable() && wpn->IsSilencerAttached())
					DetachAddon(*wpn->GetSilencerName());			// detach addon from current item
				if (wpn->ScopeAttachable() && wpn->IsScopeAttached())
					DetachAddon(*wpn->GetScopeName());				// detach addon from current item
				if (wpn->GrenadeLauncherAttachable() && wpn->IsGrenadeLauncherAttached())
					DetachAddon(*wpn->GetGrenadeLauncherName());	// detach addon from current item
			}

			m_bag.SellItem(i);
			m_bag.DestroyItem(i);

			return true;
		}

		// else if it is Belt
		CUIDragDropListEx*	old_owner		= itm->OwnerList();
		CUIDragDropListEx*	new_owner		= NULL;
		if(b_use_cursor_pos){
				new_owner					= CUIDragDropListEx::m_drag_item->BackList();
		}else
				new_owner					= m_bag.GetItemList(itm);

		CUICellItem* i						= old_owner->RemoveItem(itm, (old_owner==new_owner) );
		
		if(b_use_cursor_pos)
			new_owner->SetItem				(i,old_owner->GetDragItemPosition());
		else
			new_owner->SetItem				(i);

		m_bag.SellItem(itm);
		HightlightCurrAmmo();
		return true;
	}
	return false;
}

bool CUIBuyWnd::ToBelt(CUICellItem* itm, bool b_use_cursor_pos)
{
	PIItem	iitem						= (PIItem)itm->m_pData;

	if(CanPutInBelt(iitem))
	{
		CUIDragDropListEx*	old_owner		= itm->OwnerList();
		CUIDragDropListEx*	new_owner		= NULL;
		if(b_use_cursor_pos){
				new_owner					= CUIDragDropListEx::m_drag_item->BackList();
				VERIFY						(new_owner==m_list[MP_SLOT_BELT]);
		}else
				new_owner					= m_list[MP_SLOT_BELT];

		CUICellItem* i						= old_owner->RemoveItem(itm, (old_owner==new_owner) );
		
	//.	UIBeltList.RearrangeItems();
		if(b_use_cursor_pos)
			new_owner->SetItem				(i,old_owner->GetDragItemPosition());
		else
			new_owner->SetItem				(i);
		UNHIGHTLIGHT_ITEM(i);
		m_bag.BuyItem(itm);
		return								true;
	}
	return									false;
}

void CUIBuyWnd::SetRank(int rank)
{
	m_bag.SetRank(rank);
}

const u8 CUIBuyWnd::GetItemIndex(u32 slotNum, u32 idx, u8 &sectionNum)
{
	CUICellItem *itm = NULL;
	if (m_list[GetLocalSlot(slotNum)]->ItemsCount())
        itm = m_list[GetLocalSlot(slotNum)]->GetItemIdx(idx);

	return m_bag.GetItemIndex(itm, sectionNum);	
}

const u8 CUIBuyWnd::GetWeaponIndexInBelt(u32 indexInBelt, u8 &sectionId, u8 &itemId){
	itemId = GetItemIndex(BELT_SLOT, indexInBelt, sectionId);
	return itemId;
}

const u8 CUIBuyWnd::GetWeaponIndex(u32 slotNum)
{
	u8 tmp;
	return GetItemIndex(slotNum, 0, tmp);
}

void CUIBuyWnd::GetWeaponIndexByName(const xr_string sectionName, u8 &grpNum, u8 &idx){
	m_bag.GetWeaponIndexByName(sectionName, grpNum, idx);
}

char* CUIBuyWnd::GetWeaponNameByIndex(u8 grpNum, u8 idx){
	return m_bag.GetWeaponNameByIndex(grpNum, idx);
}

const u8 CUIBuyWnd::GetWeaponAddonInfoByIndex(u8 idx){
	return (idx & 0xe0) >> 5;
}

const u8 CUIBuyWnd::GetBeltSize(){
	return static_cast<u8>(m_list[MP_SLOT_BELT]->ItemsCount());	
}

void CUIBuyWnd::SectionToSlot(const u8 grpNum, u8 uIndexInSlot, bool bRealRepresentationSet){
	// "����������" ����� �������
	u8 addon_info = GetWeaponAddonInfoByIndex(uIndexInSlot);
	uIndexInSlot &= 0x1f; // 0x1f = 00011111;

	CUICellItem* itm = m_bag.GetItemBySectoin(grpNum, uIndexInSlot);
	VERIFY(itm);

	if (m_bag.IsInBag(itm))
//		if (UITopList[pDDItem->GetSlot()].GetDragDropItemsList().empty() || GRENADE_SLOT == pDDItem->GetSlot() || NO_ACTIVE_SLOT == pDDItem->GetSlot())
		{
			m_bag.SetExternal(itm,bRealRepresentationSet);
			ToSlot(itm, false);
			//itm->GetMessageTarget()->SendMessage(itm, DRAG_DROP_ITEM_DB_CLICK, NULL);
			// ��������� ������ �� ������� ������ �������, � ���� ��� ����, �� 
			// ������� ������ � ��������� ������
			if (addon_info)
			{
				if (addon_info & 1)
					m_bag.AttachAddon(itm, SCOPE, bRealRepresentationSet); // attach scope
				if (addon_info & 2)
					m_bag.AttachAddon(itm, GRENADELAUNCHER, bRealRepresentationSet); // attach grenade_launcher
				if (addon_info & 4)
					m_bag.AttachAddon(itm, SILENCER, bRealRepresentationSet); // attach silencer
			}
		}

}

void CUIBuyWnd::SectionToSlot(LPCSTR name, const u8 addon_info, bool bRealRepresentationSet){
	CUICellItem* itm = m_bag.GetItemBySectoin(name, /* bCreateOnFail = */true );

	if (m_bag.IsInBag(itm))
	{
		m_bag.SetExternal(itm,bRealRepresentationSet);
		itm->GetMessageTarget()->SendMessage(itm, DRAG_DROP_ITEM_DB_CLICK, NULL);

		if (addon_info)
		{
			if (addon_info & 1)
				m_bag.AttachAddon(itm, SCOPE, bRealRepresentationSet); // attach scope
			if (addon_info & 2)
				m_bag.AttachAddon(itm, GRENADELAUNCHER, bRealRepresentationSet); // attach grenade_launcher
			if (addon_info & 4)
				m_bag.AttachAddon(itm, SILENCER, bRealRepresentationSet); // attach silencer
		}
	}
	
	
	//CUIDragDropItemMP* pDDItem = UIBagWnd.GetItemBySectoin(sectionName, /* bCreateOnFail = */true);
	//int iSlot = pDDItem->GetSlot();
	//if (UIBagWnd.IsItemInBag(pDDItem))
	//	if (UITopList[iSlot].GetDragDropItemsList().empty() || GRENADE_SLOT == iSlot)
	//	{
	//		pDDItem->m_bHasRealRepresentation = bRealRepresentationSet;
	//		SendMessage(pDDItem, DRAG_DROP_ITEM_DB_CLICK, NULL);
	//		if (addon_info != 0)
	//		{
	//			if ((addon_info & 1) != 0)
	//				pDDItem->AttachDetachAddon(CUIDragDropItemMP::ID_SCOPE, true, bRealRepresentationSet);
	//			if ((addon_info & 2) != 0)
	//				pDDItem->AttachDetachAddon(CUIDragDropItemMP::ID_GRENADE_LAUNCHER, true, bRealRepresentationSet);
	//			if ((addon_info & 4) != 0)
	//				pDDItem->AttachDetachAddon(CUIDragDropItemMP::ID_SILENCER, true, bRealRepresentationSet);
	//		}
	//	}
}
void CUIBuyWnd::SectionToSlot(const char *sectionName, bool bRealRepresentationSet){
	CUICellItem* itm = m_bag.GetItemBySectoin(sectionName);	
//	int iSlot = m_info[itm->m_index].spDDItem->GetSlot();
	if (m_bag.IsInBag(itm))
	{
		m_bag.SetExternal(itm,bRealRepresentationSet);
		itm->GetMessageTarget()->SendMessage(itm, DRAG_DROP_ITEM_DB_CLICK, NULL);
	}
//		if (UITopList[iSlot].GetDragDropItemsList().empty() || GRENADE_SLOT == iSlot)
//		{
//			pDDItem->m_bHasRealRepresentation = bRealRepresentationSet;
//			SendMessage(pDDItem, DRAG_DROP_ITEM_DB_CLICK, NULL);
//		}
}

void CUIBuyWnd::ClearSlots(){
	IgnoreMoney(true);

	SlotToSection(MP_SLOT_PISTOL);
	SlotToSection(MP_SLOT_RIFLE);
	SlotToSection(MP_SLOT_BELT);
	SlotToSection(MP_SLOT_OUTFIT);
	
	IgnoreMoney(false);
}

void CUIBuyWnd::ClearRealRepresentationFlags(){
	m_bag.ClearExternalStatus();
}

void CUIBuyWnd::ReloadItemsPrices	()
{
	m_bag.ReloadItemsPrices();
}

void CUIBuyWnd::AttachAddon(CInventoryItem* item_to_upgrade)
{
//	PlaySnd										(eInvAttachAddon);
	R_ASSERT									(item_to_upgrade);
	//if (OnClient())
	//{
	//	NET_Packet								P;
	//	item_to_upgrade->object().u_EventGen	(P, GE_ADDON_ATTACH, item_to_upgrade->object().ID());
	//	P.w_u32									(item_to_upgrade->object().ID());
	//	item_to_upgrade->object().u_EventSend	(P);
	//};

	item_to_upgrade->Attach						(CurrentIItem());
	m_bag.BuyItem(CurrentItem());


	//�������� ���� �� ��������� ����� � ��������� �� ����� ������ �������
	//CActor *pActor								= smart_cast<CActor*>(Level().CurrentEntity());
	//if(pActor && item_to_upgrade == pActor->inventory().ActiveItem())
	//{
	//		m_iCurrentActiveSlot				= pActor->inventory().GetActiveSlot();
	//		pActor->inventory().Activate		(NO_ACTIVE_SLOT);
	//}

//.	(smart_cast<CUIDragDropList*>(m_pCurrentDragDropItem->GetParent()))->DetachChild(m_pCurrentDragDropItem);
//.	m_pCurrentDragDropItem			= NULL;
	SetCurrentItem								(NULL);
}

void CUIBuyWnd::DetachAddon(const char* addon_name)
{
//	PlaySnd										(eInvDetachAddon);
	//if (OnClient())
	//{
	//	NET_Packet								P;
	//	CurrentIItem()->object().u_EventGen		(P, GE_ADDON_DETACH, CurrentIItem()->object().ID());
	//	P.w_stringZ								(addon_name);
	//	CurrentIItem()->object().u_EventSend	(P);
	//};
	if (CurrentIItem()->Detach(addon_name))
	{
		CUICellItem* itm = m_bag.GetItemBySectoin	(addon_name);
		R_ASSERT(itm);
		m_bag.SellItem(itm);
	}

	//�������� ���� �� ��������� ����� � ��������� �� ����� ������ �������
	//CActor *pActor								= smart_cast<CActor*>(Level().CurrentEntity());
	//if(pActor && CurrentIItem() == pActor->inventory().ActiveItem())
	//{
	//		m_iCurrentActiveSlot				= pActor->inventory().GetActiveSlot();
	//		pActor->inventory().Activate		(NO_ACTIVE_SLOT);
	//}
}

int CUIBuyWnd::GetMoneyAmount() const{
	return m_bag.m_money;
}

void CUIBuyWnd::SetSkin(u8 SkinID)
{
	LPCSTR skins = pSettings->r_string(m_sectionName, "skins");

	R_ASSERT(_GetItemCount(skins) > SkinID);

	string256 item;
	_GetItem(skins, SkinID, item);

	CUIOutfitDragDropList* lst = (CUIOutfitDragDropList*)m_list[MP_SLOT_OUTFIT];
	lst->SetDefaultOutfit(item);
}

void CUIBuyWnd::SetMoneyAmount(int money){
	m_bag.m_money = money;
	SendMessage(this, MP_MONEY_CHANGE);
}

bool CUIBuyWnd::CanBuyAllItems(){
	return m_bag.m_money > 0;
}


//bool CUIBuyWnd::CanBuyAllItems()
//{
//	for (int slot = 0; slot < MP_SLOT_NUM; slot++)
//		while (m_list[slot]->ItemsCount())
//		{
//			CUICellItem *itm = m_list[slot]->GetItemIdx(0);
//			itm->GetMessageTarget()->SendMessage(itm, DRAG_DROP_ITEM_DB_CLICK, NULL);
//		}
//
//	return true;
//}

bool CUIBuyWnd::CheckBuyAvailabilityInSlots()
{
	int priorityArr[] =
	{
		MP_SLOT_RIFLE,
		MP_SLOT_PISTOL,
		MP_SLOT_OUTFIT,
	};

	bool status = true;

	for (int j = 0; j < 3; ++j)
	{
		// ���� ���� ����
		if (m_list[priorityArr[j]]->ItemsCount())
		{
			CUICellItem* itm = m_list[priorityArr[j]]->GetItemIdx(0);
			if (!m_bag.GetExternal(itm))
			{
				if (m_bag.HasEnoughtMoney(itm))
				{
					m_bag.BuyItem(itm);
					SET_NO_RESTR_COLOR(itm);
				}
				else
					SET_PRICE_RESTR_COLOR(itm);

			}
			else
			{
					SET_EXTERNAL_COLOR(itm);
			}
		}
	}
//
//	// � ����� ���������� ���������
//	for (DRAG_DROP_LIST_it it = UITopList[BELT_SLOT].GetDragDropItemsList().begin();
//		 it != UITopList[BELT_SLOT].GetDragDropItemsList().end();
//		 ++it)
//	{
//		CUIDragDropItemMP *pDDItemMP = smart_cast<CUIDragDropItemMP*>(*it);
//		R_ASSERT(pDDItemMP);
//
//		if (!pDDItemMP->m_bHasRealRepresentation)
//		{
//			if (HasEnoughMoney(pDDItemMP)/*pDDItemMP->GetCost() <= GetMoneyAmount()*/)
//			{
////				SetMoneyAmount(GetMoneyAmount() - pDDItemMP->GetCost());
//				SetMoneyAmount(GetMoneyLeft(- pDDItemMP->GetCost()));
//				pDDItemMP->SetColor(cAbleToBuy);
//			}
//			else
//			{
//				pDDItemMP->SetColor(cUnableToBuy);
//				pDDItemMP->m_bAlreadyPaid = false;
//				status = false;
//			}
//		}
//		else
//			pDDItemMP->SetColor(cAbleToBuyOwned);
//	}

	return status;
}

void CUIBuyWnd::IgnoreMoneyAndRank(bool ignore)
{
	m_bIgnoreMoneyAndRank = ignore;
	m_bag.IgnoreRank(ignore);
	m_bag.IgnoreMoney(ignore);
};

void CUIBuyWnd::IgnoreMoney(bool ignore)
{
	if (m_bIgnoreMoneyAndRank)
	{		
		m_bag.IgnoreMoney(true);
	}
	else
	{		
		m_bag.IgnoreMoney(ignore);
	};
}