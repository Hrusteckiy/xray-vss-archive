// CUIDiaryWnd.h:  ������� � ��� ��� � ��� �������
// 
//////////////////////////////////////////////////////////////////////

#pragma once

#include "UIDialogWnd.h"
#include "UIListWnd.h"
#include "UIFrameWindow.h"
#include "UIFrameLineWnd.h"
#include "UIAnimatedStatic.h"
#include "UINewsWnd.h"
#include "UIJobsWnd.h"
#include "UIContracts.h"
#include "UIActorDiary.h"
#include "UIPdaAux.h"

///////////////////////////////////////
// �������
///////////////////////////////////////

class CUIDiaryWnd: public CUIWindow
{
	typedef CUIWindow inherited;
public:
	// Ctor and Dtor
	CUIDiaryWnd();
	virtual ~CUIDiaryWnd();

	virtual void		Init();
	virtual void		SendMessage(CUIWindow *pWnd, s16 msg, void* pData = NULL);
	virtual void		Show(bool status);

	// ������� ��������� �������� �������
	void				ArticleCaption(LPCSTR caption);

	// ������� �������������� ������ ���
	void				SetActiveSubdialog(EPdaSections section);
	void				OpenDiaryTree(ARTICLE_ID id) { UIActorDiaryWnd.m_pCore->OpenTree(id); }

	void				MarkNewsAsRead (bool status);
	//������ � �������� � ������
	void				ReloadArticles();

public:
	// �������� ���� �������� � ���� ����������
	CUINewsWnd			UINewsWnd;
	CUIJobsWnd			UIJobsWnd;
	CUIContractsWnd		UIContractsWnd;
	CUIActorDiaryWnd	UIActorDiaryWnd;
protected:
	// ������� �������� ���������
	CUIWindow			*m_pActiveSubdialog;
	// TreeView
	CUIListWnd			UITreeView;
	// �������� ��� TreeView
	CUIFrameWindow		UITreeViewBg;
	CUIFrameLineWnd		UITreeViewHeader;
	// ������������ ������
	CUIAnimatedStatic	UIAnimation;

	// �������� ��� �������� ����
	CUIFrameWindow		UIFrameWnd;
	// ����� ��������� ����
	CUIFrameLineWnd		UIFrameWndHeader;
	// ��������� ������������� � �������� ���� �������� �������
	CUIStatic			UIArticleCaption;

	// ����� � ������ ��������� ������
	CGameFont			*m_pTreeRootFont;
	u32					m_uTreeRootColor;
	CGameFont			*m_pTreeItemFont;
	u32					m_uTreeItemColor;
	u32					m_uUnreadColor;

	// ����� ������������� �����
	CUIStatic			*m_pLeftHorisontalLine;

	//������ ���������, ������� ���� ������ �� ���������
	CUITreeViewItem*	m_pContractsTreeItem;
	// ������ ����� ��������
	CUITreeViewItem		*m_pActorDiaryRoot;
	// ��������� �� ������� � ���������
	CUITreeViewItem		*m_pJobsRoot;
	CUITreeViewItem		*m_pActiveJobs;
	CUITreeViewItem		*m_pNews;

	// �������������� TreeView
	void				InitTreeView();
	//������������� ����� �������
	void				InitDiary();

	//id ��������, ��� ������ ����������
	u16					m_TraderID;

	//������������� ���������� � �������� � �������
	void				SetContractTrader();
};