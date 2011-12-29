
#include "stdafx.h"
#include "MainFrm.h"
#include "WindowsView.h"
#include "Resource.h"
#include "UiFeatureBuilder.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CWindowsView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// ����/����
//////////////////////////////////////////////////////////////////////

CWindowsView::CWindowsView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	m_hRoot = NULL;
}

CWindowsView::~CWindowsView()
{
}

BEGIN_MESSAGE_MAP(CWindowsView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowsView ��Ϣ��������

int CWindowsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_wndWindowTree.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ���Dui������ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ����ͼ��:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	m_wndWindowTree.ModifyStyle(0, TVS_SHOWSELALWAYS);

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillClassView();

	return 0;
}

void CWindowsView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CWindowsView::FillClassView()
{
/*	m_hRoot = m_wndWindowTree.InsertItem(_T("������-��塿"), 0, 0);
	m_wndWindowTree.SetItemState(m_hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndWindowTree.InsertItem(_T("CFakeAboutDlg"), 1, 1, m_hRoot);
	m_wndWindowTree.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_wndWindowTree.Expand(m_hRoot, TVE_EXPAND);

	hClass = m_wndWindowTree.InsertItem(_T("CFakeApp"), 1, 1, m_hRoot);
	m_wndWindowTree.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_wndWindowTree.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_wndWindowTree.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_wndWindowTree.InsertItem(_T("CFakeAppDoc"), 1, 1, m_hRoot);
	m_wndWindowTree.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_wndWindowTree.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_wndWindowTree.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_wndWindowTree.InsertItem(_T("CFakeAppView"), 1, 1, m_hRoot);
	m_wndWindowTree.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_wndWindowTree.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_wndWindowTree.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_wndWindowTree.Expand(hClass, TVE_EXPAND);

	hClass = m_wndWindowTree.InsertItem(_T("CFakeAppFrame"), 1, 1, m_hRoot);
	m_wndWindowTree.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	m_wndWindowTree.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	m_wndWindowTree.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_wndWindowTree.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_wndWindowTree.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = m_wndWindowTree.InsertItem(_T("Globals"), 2, 2, m_hRoot);
	m_wndWindowTree.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	m_wndWindowTree.Expand(hClass, TVE_EXPAND);*/
}

void CWindowsView::Init(IKernelWindow* pKernelWindow, CPropertyViewCtrl *pPropCtrl)
{
	m_wndWindowTree.Init(pKernelWindow, pPropCtrl);
}

CWindowsViewTree* CWindowsView::GetViewTreeCtrl()
{
	return &m_wndWindowTree;
}

void CWindowsView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndWindowTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CWindowsView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndWindowTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CWindowsView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CWindowsView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CWindowsView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CWindowsView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("���ӳ�Ա����..."));
}

void CWindowsView::OnClassAddMemberVariable()
{
	// TODO: �ڴ˴�����������������
}

void CWindowsView::OnClassDefinition()
{
	// TODO: �ڴ˴�����������������
}

void CWindowsView::OnClassProperties()
{
	// TODO: �ڴ˴�����������������
}

void CWindowsView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndWindowTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CWindowsView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndWindowTree.SetFocus();
}

void CWindowsView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndWindowTree.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* ����*/);
}

void CWindowsView::InitShowNewProject()
{
	m_wndWindowTree.InitShowNewProject();
}