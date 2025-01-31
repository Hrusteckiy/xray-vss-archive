// File:		UIBag.h
// Description:	Bag for BuyWeaponWnd
// Created:		10.02.2005
// Author:		Serge Vynnychenko
// Mail:		narrator@gsc-game.kiev.ua

// Copyright 2005 GSC Game World

#pragma once

#include "UIStatic.h"
#include "xrXMLParser.h"
#include "UIDragDropList.h"
#include "UIDragDropItemMP.h"
#include "UITabButtonMP.h"

#include "UIBuyWeaponStd.h"

enum Groups {
	GROUP_2,
	GROUP_BOXES,		
	GROUP_4,
	GROUP_5,
	GROUP_6,
	GROUP_31,
	GROUP_32,
	GROUP_33,
	GROUP_34,
	GROUP_DEFAULT,
	NUMBER_OF_GROUPS
};

enum MENU_LEVELS { 
	mlRoot = 0, 
	mlBoxes, 
	mlWpnSubType 
};

struct BoxInfo
{
	shared_str			xmlTag;
	shared_str			filterString;
	int					gridWidth, gridHeight;
	CUITabButtonMP*		pButton;
};

class CUIBag : public CUIStatic {	
public:
	CUIBag(CHECK_PROC proc);
	virtual ~CUIBag();
	virtual void	Init(CUIXml& pXml, const char* path, LPCSTR strSectionName, LPCSTR strPricesSection);
	virtual void	Init(int x, int y, int width, int height);
	virtual bool	OnKeyboard(int dik, EUIMessages keyboard_action);
			void	OnBtnShotgunsClicked();
			void	OnBtnMachinegunsClicked();
			void	OnBtnSniperClicked();
			void	OnBtnHeavyClicked();
			void	UpdateMoney(int iMoney);
			void	UpdateBuyPossibility();
	virtual void	Draw();
	virtual void	SendMessage(CUIWindow* pWnd, s16 msg, void* pData = 0);

			void	BuyReaction();
			void	HideAll();
			void	ShowSection(int iSection);			
			bool	IsItemInBag(CUIDragDropItemMP* pDDItem);
			void	ClearRealRepresentationFlags();
			void	GetWeaponIndexByName(const xr_string sectionName, u8 &grpNum, u8 &idx);
			u8		GetItemIndex(CUIDragDropItemMP* pDDItem, u8 &sectionNum);

CUIDragDropItemMP*	GetItemBySectoin(const char *sectionName);
CUIDragDropItemMP*	GetItemBySectoin(const u8 grpNum, u8 uIndexInSlot);
CUIDragDropItemMP*	GetAddonByID(CUIDragDropItemMP *pAddonOwner, CUIDragDropItemMP::AddonIDs ID);

CUIDragDropItemMP*	CreateCopy(CUIDragDropItemMP *pDDItem);
CUIDragDropItemMP*	GetItemByKey(int dik, int section);
			void	DeleteCopy(CUIDragDropItemMP *pDDItem);
			bool	IsItemAnAddonSimple(CUIDragDropItemMP *pPossibleAddon) const;
			
	MENU_LEVELS		GetMenuLevel();
protected:
			bool	SetMenuLevel(MENU_LEVELS level);
			void	ShowSectionEx(int iSection);
			void	EnableDDItem(CUIDragDropItemMP* pDDItem, bool bEnable = true);			
			void	OnItemSelect();			
			void	OnItemDrop(CUIDragDropItemMP* pItem);
			void	OnBackClick();
    		int		GetMoneyAmount();

	// INIT Functions
			void	InitBoxes(CUIXml& pXml);
			void	InitAddonsInfo(CUIDragDropItemMP &DDItemMP, const xr_string &sectioName);
			void	PutItemToGroup(CUIDragDropItemMP* pDDItem, int iGroup);
			void	InitWpnSectStorage();
			void	FillUpGroups();
			void	FillUpGroup(const u32 group);
			void	FillUpItem(CUIDragDropItemMP* pDDItem, int iGroup, int j);
			void	FillUpInfiniteItemsList();
			bool	IsItemInfinite(CUIDragDropItemMP* pDDItem);
CUIDragDropList*	GetCurrentGroup();
			int		GetCurrentGroupIndex();

	CUI3tButton					m_btnBack;
	shared_str					m_StrSectionName;
	shared_str					m_StrPricesSection;
	int							m_iMoneyAmount;
	Irect						m_rectWorkSpace;
	MENU_LEVELS					m_mlCurrLevel;
	CUIDragDropItemMP*			m_pCurrentDDItem;
	xr_list<shared_str>			m_vInfiniteItemsList;
	xr_list<CUIDragDropItemMP*>	m_vCopyList;

	BoxInfo m_boxesDefs[4];

	DEF_VECTOR(WPN_SECT_NAMES, xr_string); // vector of weapons. it represents ONE section 
	DEF_VECTOR(WPN_LISTS, WPN_SECT_NAMES); // vector of sections
	WPN_LISTS	m_wpnSectStorage;

	xr_list<CUIDragDropItemMP*> m_allItems;

	// ������� ����������� ����� ������ � ������� ������� ����������. ����������� �� ����� ���������� 
	// ���������� �� ltx ����� ���������������� ���� ������� ����
	typedef xr_vector<std::pair<shared_str, shared_str> >	CONFORMITY_TABLE;
	typedef CONFORMITY_TABLE::iterator						CONFORMITY_TABLE_it;
	CONFORMITY_TABLE										m_ConformityTable;
   
	CUIDragDropList m_groups[GROUP_DEFAULT + 1];
	int subSection_group3[4];
};