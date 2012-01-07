
// UiFeatureBuilderView.cpp : CUiFeatureBuilderView 类的实现
//

#include "stdafx.h"
#include "UiFeatureBuilder.h"

#include "UiFeatureBuilderDoc.h"
#include "UiFeatureBuilderView.h"
#include "MainFrm.h"
#include "FeatureControlList.h"
#include "WindowsViewTree.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 窗口显示距离view的边框距离
#define SHOW_WINDOW_SPACE						(50)


// CUiFeatureBuilderView

IMPLEMENT_DYNCREATE(CUiFeatureBuilderView, CFormView)

BEGIN_MESSAGE_MAP(CUiFeatureBuilderView, CFormView)
	// 标准打印命令
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CUiFeatureBuilderView 构造/析构

CUiFeatureBuilderView::CUiFeatureBuilderView() : CFormView(CUiFeatureBuilderView::IDD)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_pUiEngine = NULL;
	m_pUiKernel = NULL;
	m_pSkinManager = NULL;
	m_pControlList = NULL;
	m_pWindowViewTree = NULL;
	m_bInitOk = false;
	m_pCurrentWnd = NULL;
	m_pSelectControl = NULL;
	m_pCreateCtrlParentCtrl = NULL;
	m_bIsLButtonDown = false;
	m_bMoveInWndFangKuai8 = false;
	m_bMoveInCtrlFangKuai8 = false;
	m_bCreateNewCtrl = false;

	m_ScrollOffset.cx = m_ScrollOffset.cy = 0;
	m_LBtnDownPos.x = m_LBtnDownPos.y = 0;
	m_MouseMovePos.x = m_MouseMovePos.y = 0;
	m_LBtnUpPos.x = m_LBtnUpPos.y = 0;
}

CUiFeatureBuilderView::~CUiFeatureBuilderView()
{
	GdiplusShutdown(m_gdiplusToken);
}

void CUiFeatureBuilderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CUiFeatureBuilderView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// 没错,相信你的眼睛,就是下面这两行代码了,干掉他们就能达到你的目的！可以记录关闭Builder布局了
//	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();
}

BOOL CUiFeatureBuilderView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CUiFeatureBuilderView::ResetShowWindow(IWindowBase *pCurrentWnd, IControlBase *pCtrlBase)
{
	m_pCurrentWnd = pCurrentWnd;
	m_pSelectControl = pCtrlBase;

	ResetViewShowSize();
	this->RedrawWindow();
}

BOOL CUiFeatureBuilderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CUiFeatureBuilderView::Init(IUiFeatureKernel* pKernelWindow, CFeatureControlList *pCtrlList, CWindowsViewTree *pWndTree)
{
	m_pUiKernel = pKernelWindow;
	m_pControlList = pCtrlList;
	m_pWindowViewTree = pWndTree;
	if (m_pUiKernel != NULL)
	{
		m_pSkinManager = m_pUiKernel->GetSkinManager();
		m_pUiEngine = m_pUiKernel->GetUiEngine();
	}
}

void CUiFeatureBuilderView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUiFeatureBuilderView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CUiFeatureBuilderView 诊断

#ifdef _DEBUG
void CUiFeatureBuilderView::AssertValid() const
{
	CFormView::AssertValid();
}

void CUiFeatureBuilderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CUiFeatureBuilderDoc* CUiFeatureBuilderView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUiFeatureBuilderDoc)));
	return (CUiFeatureBuilderDoc*)m_pDocument;
}
#endif //_DEBUG


// CUiFeatureBuilderView 消息处理程序

void CUiFeatureBuilderView::SetProjectInitState(bool bInitOk)
{
	m_bInitOk = bInitOk;
}

void CUiFeatureBuilderView::SetNewControl(bool bIsNew)
{
	m_bCreateNewCtrl = bIsNew;
}

void CUiFeatureBuilderView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (pMain != NULL)
		pMain->SetView(this);
}

void CUiFeatureBuilderView::OnDraw(CDC* pDC)
{
	if (pDC == NULL || !IS_SAFE_HANDLE(pDC->GetSafeHdc()) || m_pUiEngine == NULL)
		return;

	CSize ViewSize, sizePage, sizeLine;
	int nMode = 0;
	this->GetDeviceScrollSizes(nMode, ViewSize, sizePage, sizeLine);
	m_MemDc.Create(ViewSize.cx, ViewSize.cy, RGB(255,255,255), false, true);
	if (!IS_SAFE_HANDLE(m_MemDc.GetSafeHdc()))
		return;

	DrawWindowView();

	// 绘制创建新控件时的矩形
	DrawCreateCtrlRect();

	::BitBlt(pDC->GetSafeHdc(), 0, 0, ViewSize.cx, ViewSize.cy, m_MemDc.GetSafeHdc(), 0, 0, SRCCOPY);
}

// 绘制创建新控件时的矩形
void CUiFeatureBuilderView::DrawCreateCtrlRect()
{
	if (!m_bCreateNewCtrl || !m_bIsLButtonDown)
		return;

	Graphics DoGrap(m_MemDc.GetSafeHdc());

	Pen LinePen(Color(255, 255, 0, 0), 1.5f);
	REAL dashVals[2] = {0.8f, 2.0f};
	LinePen.SetDashPattern(dashVals, 2);
	LinePen.SetDashOffset(30);

	DoGrap.DrawLine(&LinePen, m_LBtnDownPos.x, m_LBtnDownPos.y, m_MouseMovePos.x, m_LBtnDownPos.y);
	DoGrap.DrawLine(&LinePen, m_MouseMovePos.x, m_LBtnDownPos.y, m_MouseMovePos.x, m_MouseMovePos.y);
	DoGrap.DrawLine(&LinePen, m_MouseMovePos.x, m_MouseMovePos.y, m_LBtnDownPos.x, m_MouseMovePos.y);
	DoGrap.DrawLine(&LinePen, m_LBtnDownPos.x, m_MouseMovePos.y, m_LBtnDownPos.x, m_LBtnDownPos.y);
}

void CUiFeatureBuilderView::DrawWindowView()
{
	CRect ViewRct(0, 0, 0, 0);
	this->GetClientRect(&ViewRct);

	// 背景
	Graphics DoGrap(m_MemDc.GetSafeHdc());
	SolidBrush sBrush(Color(MAX_ALPHA, 255, 255, 255));
	DoGrap.FillRectangle(&sBrush, 0, 0, ViewRct.Width(), ViewRct.Height());

	// 窗口绘制
	if (m_pCurrentWnd != NULL)
		m_pCurrentWnd->BD_DrawWindowView(m_MemDc);
}

void CUiFeatureBuilderView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	m_ScrollOffset.cx = this->GetScrollPos(SB_HORZ);
}

void CUiFeatureBuilderView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
	m_ScrollOffset.cy = this->GetScrollPos(SB_VERT);
}

void CUiFeatureBuilderView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	ResetViewShowSize();
	this->RedrawWindow();
}

void CUiFeatureBuilderView::ResetViewShowSize()
{
	int nWidth = 500, nHeight = 500;
	if (m_pCurrentWnd != NULL)
	{
		CRect ViewRct(0, 0, 0, 0);
		::GetClientRect(m_hWnd, &ViewRct);
		CSize WndSize = m_pCurrentWnd->PP_GetWindowPropSize();

		// 设置View的大小
		nWidth = ViewRct.Width();
		if (nWidth < WndSize.cx + SHOW_WINDOW_SPACE)
			nWidth = WndSize.cx + SHOW_WINDOW_SPACE;

		 nHeight = ViewRct.Height();
		if (nHeight < WndSize.cy + SHOW_WINDOW_SPACE)
			nHeight = WndSize.cy + SHOW_WINDOW_SPACE;
	}
	else
	{
		CRect ViewRct;
		::GetClientRect(m_hWnd, ViewRct);
		nWidth = ViewRct.Width();
		nHeight = ViewRct.Height();
	}

	this->SetScrollSizes(MM_TEXT, CSize(nWidth, nHeight));
}

void CUiFeatureBuilderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CUiFeatureBuilderView::OnMouseMove(UINT nFlags, CPoint point)
{
	CFormView::OnMouseMove(nFlags, point);
	m_MouseMovePos = point;

	if (!m_bInitOk || m_pSkinManager == NULL || m_pUiKernel == NULL)
		return;

	if (m_bIsLButtonDown)
	{
		OnMouseMove_LButtonDown(point);
		return;
	}

	// 如果需要创建一个新控件
	if (m_bCreateNewCtrl)
		return;

	m_bMoveInWndFangKuai8 = OnMouseMove_FangKuai8(point, true);
	if (m_bMoveInWndFangKuai8)
		return;

	m_bMoveInCtrlFangKuai8 = OnMouseMove_FangKuai8(point, false);
	if (m_bMoveInCtrlFangKuai8)
		return;

	SetViewCursor(UF_IDC_ARROW);
}

void CUiFeatureBuilderView::OnMouseMove_LButtonDown(CPoint point)
{
	// 如果需要创建一个新控件
	if (m_bCreateNewCtrl)
	{
		// 绘制创建新控件时的矩形
		this->RedrawWindow();
	}
	else if (m_bMoveInWndFangKuai8)
	{
		// 拖动窗口
	}
	else if (m_bMoveInCtrlFangKuai8)
	{
		// 拖动控件，改变控件大小
	}
}

bool CUiFeatureBuilderView::OnMouseMove_FangKuai8(CPoint point, bool bIsWnd)
{
	point.x += m_ScrollOffset.cx;
	point.y += m_ScrollOffset.cy;

	FANGKUAI_8* pFk8 = NULL;
	if (bIsWnd)
	{
		if (m_pCurrentWnd != NULL)
			pFk8 = m_pCurrentWnd->BD_GetFangKuai8Rect();
	}
	else
	{
		if (m_pSelectControl != NULL)
			pFk8 = m_pSelectControl->BD_GetFangKuai8Rect();
	}

	if (pFk8 == NULL)
		return false;

	if (PtInRect(&pFk8->LeftTop, point))
	{
		SetViewCursor(UF_IDC_SIZENWSE);
		return true;
	}
	else if (PtInRect(&pFk8->LeftMid, point))
	{
		SetViewCursor(UF_IDC_SIZEWE);
		return true;
	}
	else if (PtInRect(&pFk8->LeftBottom, point))
	{
		SetViewCursor(UF_IDC_SIZENESW);
		return true;
	}
	else if (PtInRect(&pFk8->MidTop, point))
	{
		SetViewCursor(UF_IDC_SIZENS);
		return true;
	}
	else if (PtInRect(&pFk8->MidBottom, point))
	{
		SetViewCursor(UF_IDC_SIZENS);
		return true;
	}
	else if (PtInRect(&pFk8->RightTop, point))
	{
		SetViewCursor(UF_IDC_SIZENESW);
		return true;
	}
	else if (PtInRect(&pFk8->RightMid, point))
	{
		SetViewCursor(UF_IDC_SIZEWE);
		return true;
	}
	else if (PtInRect(&pFk8->RightBottom, point))
	{
		SetViewCursor(UF_IDC_SIZENWSE);
		return true;
	}

	return false;
}

// 设置光标
void CUiFeatureBuilderView::SetViewCursor(int nCursor)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (pMain != NULL)
		pMain->SetViewCursor(nCursor);
}

void CUiFeatureBuilderView::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetViewCursor(UF_IDC_ARROW);
	CFormView::OnLButtonUp(nFlags, point);
	m_LBtnUpPos = point;
	m_bIsLButtonDown = false;

	if (!m_bInitOk)
		return;


	// 如果需要创建一个新控件
	if (m_bCreateNewCtrl)
	{
		CreateNewControl();
		m_bCreateNewCtrl = false;
		this->RedrawWindow();
		return;
	}
}

void CUiFeatureBuilderView::CreateNewControl()
{
	USES_CONVERSION;
	// 创建新控件
	if (m_pControlList == NULL || m_pUiKernel == NULL || m_pCurrentWnd == NULL)
		return;

	IControlBase* pParentCtrl = GetSelectControl(m_LBtnDownPos);
	CString strCtrlTypeName = m_pControlList->GetSelectCtrlTypeName();
	m_pSelectControl = m_pUiKernel->BD_CreateControlEmptyPropetry(m_pCurrentWnd, pParentCtrl, W2A(strCtrlTypeName));
	if (m_pSelectControl == NULL)
	{
		CString strInfo(_T(""));
		strInfo.Format(_T("创建控件【%s】失败！"), strCtrlTypeName);
		AfxMessageBox(strInfo, MB_OK | MB_ICONERROR);
		return;
	}

	// 计算新控件的位置和大小

	// 重新绘制WindowTree
	if (m_pWindowViewTree != NULL)
		m_pWindowViewTree->AddNewControlToWindowTreeNode(m_pCurrentWnd, pParentCtrl, m_pSelectControl);
}

void CUiFeatureBuilderView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFormView::OnLButtonDown(nFlags, point);
	m_LBtnDownPos = point;
	m_bIsLButtonDown = true;
	m_pCreateCtrlParentCtrl = NULL;

	if (m_pSkinManager == NULL || m_pUiKernel == NULL || m_pCurrentWnd == NULL)
		return;

	FANGKUAI_8* pFk8 = m_pCurrentWnd->BD_GetFangKuai8Rect();
	if (pFk8 == NULL)
		return;

	// 如果需要创建一个新控件
	if (m_bCreateNewCtrl)
	{
		m_bCreateNewCtrl = PtInWindow(point);
		if (!m_bCreateNewCtrl)
		{
			SetViewCursor(UF_IDC_ARROW);
		}
		else
		{
			// 取得选中的父控件
			m_pCreateCtrlParentCtrl = m_pCurrentWnd->BD_Get
		}
		return;
	}


	// 判断数遍左键点击在哪个控件上
}

bool CUiFeatureBuilderView::PtInWindow(CPoint point)
{
	if (m_pCurrentWnd == NULL)
		return false;

	FANGKUAI_8* pFk8 = m_pCurrentWnd->BD_GetFangKuai8Rect();
	if (pFk8 == NULL)
		return false;

	point.x += m_ScrollOffset.cx;
	point.y += m_ScrollOffset.cy;

	if (::PtInRect(&(pFk8->EntityRct), point))
		return true;

	return false;
}

// 取得选择的控件s
IControlBase* CUiFeatureBuilderView::GetSelectControl(CPoint point)
{
	point.x += m_ScrollOffset.cx;
	point.y += m_ScrollOffset.cy;


	return NULL;
}
