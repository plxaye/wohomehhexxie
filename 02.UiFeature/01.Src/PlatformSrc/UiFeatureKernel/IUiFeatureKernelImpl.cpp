
#include "StdAfx.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IFeatureObject.h"
#include "..\..\Inc\ICommonFun.h"
#include "IUiFeatureKernelImpl.h"
#include "IWindowBaseImpl.h"
#include "WindowSubclass.h"
#include "ControlImpl.h"
#include "IPropertySkinManagerImpl.h"
#include "..\..\Inc\ICtrlInterface.h"
#include "..\..\Inc\IPropertyControl.h"

// 内核对【对话框】的接口
IUiFeatureKernel *GetKernelWindowInterface()
{
	return IUiFeatureKernelImpl::GetInstance();
}

IUiFeatureKernelImpl::IUiFeatureKernelImpl(void)
{
	m_nBuilderHwnd = 1;
	m_CtrlRegMap.clear();
	m_pSkinMgr = (IPropertySkinManagerImpl *)GetSkinManager();
	m_pUiEngine = (IUiEngineImpl *)GetSkinManager();

	m_pControlMgr = NULL;
	m_hControlDll = NULL;
	string strPath = PathHelper(NAME_CONTROL_DLL);
	if (strPath.size() > 0)
	{
		m_hControlDll = ::LoadLibraryA(strPath.c_str());
		if (m_hControlDll != NULL)
		{
			GETCONTROLMANAGER GetControl = (GETCONTROLMANAGER)::GetProcAddress(m_hControlDll, "GetControlManager");
			if (GetControl != NULL)
				m_pControlMgr = GetControl();
		}
	}
}

IUiFeatureKernelImpl::~IUiFeatureKernelImpl(void)
{
	SAFE_FREE_LIBRARY(m_hControlDll);
	ReleaseKernelWindow();
}

void IUiFeatureKernelImpl::ReleaseKernelWindow()
{
	for (WINDOW_IMPL_MAP::iterator pWndItem = m_WndImplMap.begin(); pWndItem != m_WndImplMap.end(); pWndItem++)
	{
		IWindowBaseImpl* pDelWnd = pWndItem->second;
		SAFE_DELETE(pDelWnd);
	}
	m_WndImplMap.clear();
	m_CtrlRegMap.clear();
}

// 从zip文件中找到指定文件
bool IUiFeatureKernelImpl::FindUnZipFile(const char *pFileName, BYTE **ppOutBuffer, int *pnFileLen)
{
	CZipFileList* pZip = CZipFileList::GetInstance();
	if (pZip == NULL || pFileName == NULL || strlen(pFileName) <= 0 || ppOutBuffer == NULL || pnFileLen == NULL)
		return false;

	*ppOutBuffer = NULL;
	*pnFileLen = 0;

	ZIP_FILE *pZipFile = pZip->FindUnZipFile((char*)pFileName);
	if (pZipFile == NULL)
		return false;

	*ppOutBuffer = pZipFile->pFileData;
	*pnFileLen = pZipFile->dwSrcFileLen;

	return true;
}

IUiFeatureKernel* IUiFeatureKernelImpl::GetInstance()
{
	static IUiFeatureKernelImpl _KernelWindowInstance;
	return &_KernelWindowInstance;
}

// 取得所有支持的控件
CONTROL_REG_MAP* IUiFeatureKernelImpl::BD_GetRegisterControl()
{
	CControlImpl::GetInstance()->SetRegControlMap(&m_CtrlRegMap);
	return &m_CtrlRegMap;
}

IPropertySkinManager* IUiFeatureKernelImpl::GetSkinManager()
{
	return IPropertySkinManagerImpl::GetInstance();
}

// 一个对话框释放皮肤资源
void IUiFeatureKernelImpl::PG_ReleaseFeatureSkin(HWND hWnd)
{
	WINDOW_IMPL_MAP::iterator pWndImplItem = m_WndImplMap.find(hWnd);
	if (pWndImplItem != m_WndImplMap.end())
	{
		IWindowBaseImpl* pWndImpl = pWndImplItem->second;
		SAFE_DELETE(pWndImpl);
		m_WndImplMap.erase(pWndImplItem);
	}
}

// 关闭一个工程
bool IUiFeatureKernelImpl::BD_CloseProject()
{
	ReleaseKernelWindow();
	if (m_pSkinMgr != NULL)
		m_pSkinMgr->ReleaseSkinManagerPropetry();

	return true;
}

// 一个对话框从一个皮肤包里使用指定的对话框皮肤资源初始化自己
IWindowBase* IUiFeatureKernelImpl::PG_InitFeatureSkin(HWND hWnd, char *pszSkinPath, char *pszWndName)
{
	if (!::IsWindow(hWnd) || pszSkinPath == NULL || pszWndName == NULL || strlen(pszSkinPath) <= 0 || strlen(pszWndName) <= 0)
		return NULL;

	// 开始子类化
	if (CWindowSubclass::GetInstance()->FindSubWindow(hWnd) != NULL)
		return NULL;

	IWindowBaseImpl *pWndBaseImpl = new IWindowBaseImpl;
	if (pWndBaseImpl == NULL)
		return NULL;

	IWindowBase* pWndBase = (dynamic_cast<IWindowBase*>(pWndBaseImpl));
	if (!CWindowSubclass::GetInstance()->SubWindow(hWnd, pWndBase))
	{
		SAFE_DELETE(pWndBaseImpl);
		return NULL;
	}

	// 初始化窗口
	pWndBase->PG_InitWindowBase(hWnd, pszSkinPath, pszWndName);

	// 记录到窗口队列中
	m_WndImplMap.insert(pair<HWND, IWindowBaseImpl*>(hWnd, pWndBaseImpl));
	return pWndBase;
}

// 创建一个Builder使用的空的窗口
IWindowBase* IUiFeatureKernelImpl::BD_CreateWindowEmptyPropetry()
{
	if (m_pSkinMgr == NULL)
		return NULL;

	IPropertyGroup *pWindowPropGroup = dynamic_cast<IPropertyGroup*>(m_pSkinMgr->CreateEmptyBaseProp(OTID_GROUP));
	if (pWindowPropGroup == NULL)
		return NULL;

	IPropertyWindow *pPropWindow = dynamic_cast<IPropertyWindow*>(m_pSkinMgr->CreateEmptyBaseProp(OTID_WINDOW));
	if (pPropWindow == NULL)
		return NULL;

	pPropWindow->SetWindowPropGroup(pWindowPropGroup);

	// 设置objecid
	char szId[MAX_PATH];
	memset(szId, 0, MAX_PATH);
	sprintf_s(szId, MAX_PATH-1, "%s%d", PROP_TYPE_WINDOW_NAME, m_pSkinMgr->GetNewId());
	pWindowPropGroup->SetObjectId(szId);
	pPropWindow->SetObjectId(szId);

	IWindowBaseImpl *pWndBaseImpl = new IWindowBaseImpl;
	if (pWndBaseImpl == NULL)
		return NULL;

	IWindowBase* pWndBase = (dynamic_cast<IWindowBase*>(pWndBaseImpl));
	if (pWndBase == NULL)
	{
		SAFE_DELETE(pWndBaseImpl);
		return NULL;
	}
	// 初始化在builder中的属性
	pWndBase->BD_InitWindowBase(pPropWindow, true);

	// 记录到窗口队列中
	m_WndImplMap.insert(pair<HWND, IWindowBaseImpl*>((HWND)m_nBuilderHwnd++, pWndBaseImpl));
	m_pSkinMgr->GetAllWindowPropMap()->insert(pair<string, IPropertyBase*>(pWndBase->GetObjectId(), pWindowPropGroup));
	return pWndBase;
}

// 创建一个Builder使用的空的窗口
IWindowBase* IUiFeatureKernelImpl::BD_CreateWindowByPropetry(IPropertyWindow *pPropWnd)
{
	if (pPropWnd == NULL)
		return NULL;

	IWindowBaseImpl *pWndBaseImpl = new IWindowBaseImpl;
	if (pWndBaseImpl == NULL)
		return NULL;

	IWindowBase* pWndBase = (dynamic_cast<IWindowBase*>(pWndBaseImpl));
	if (pWndBase == NULL)
	{
		SAFE_DELETE(pWndBaseImpl);
		return NULL;
	}
	// 初始化在builder中的属性
	pWndBase->BD_InitWindowBase(pPropWnd, false);

	// 记录到窗口队列中
	m_WndImplMap.insert(pair<HWND, IWindowBaseImpl*>((HWND)m_nBuilderHwnd++, pWndBaseImpl));
	m_pSkinMgr->GetAllWindowPropMap()->insert(pair<string, IPropertyBase*>(pWndBase->GetObjectId(), pWndBase->PP_GetWindowPropetryGroup()));
	return pWndBase;
}

// 保存皮肤包
bool IUiFeatureKernelImpl::BD_SaveProject(char *pszSkinDir, char *pszSkinName)
{
	if (m_pSkinMgr == NULL)
		return false;
	
	return m_pSkinMgr->BD_SaveProject(pszSkinDir, pszSkinName);
}

// 创建/打开一个新的皮肤工程
bool IUiFeatureKernelImpl::BD_OpenProject(char *pszSkinDir, char *pszSkinName)
{
	if (pszSkinDir == NULL || pszSkinName == NULL || m_pSkinMgr == NULL)
		return false;

	m_pSkinMgr->ReleaseSkinManagerPropetry();

	// 导入zip文件
	string strZipPath = pszSkinDir;
	if (pszSkinDir[strlen(pszSkinDir) - 1] != '\\')
		strZipPath += "\\";
	strZipPath += pszSkinName;
	strZipPath += NAME_SKIN_FILE_EX_NAME;

	if (!CZipFileList::GetInstance()->ReadZipFile(strZipPath.c_str()))
		return false;

	return true;
}

// 创建一个Builder使用的空的控件
IControlBase* IUiFeatureKernelImpl::BD_CreateControlEmptyPropetry(IWindowBase *pParentWnd, IControlBase *pParentCtrl, char *pszNewCtrlTypeName)
{
	if (pParentWnd == NULL || pParentWnd->PP_GetWindowPropetry() == NULL || m_pControlMgr == NULL || m_pSkinMgr == NULL || pszNewCtrlTypeName == NULL || strlen(pszNewCtrlTypeName) <= 0)
		return NULL;

	// 设置新控件的ObjectID
	string strObjId(pParentWnd->GetObjectId());
	if (pParentCtrl != NULL)
	{
		strObjId = pParentCtrl->GetObjectId();
	}

	char szId[1024];
	memset(szId, 0, 1024);
	sprintf_s(szId, 1023, "%s.%s%d", strObjId.c_str(), pszNewCtrlTypeName, m_pSkinMgr->GetNewId());

	// 创建新控件
	ICtrlInterface* pCtrlIfc = m_pControlMgr->CreateCtrl(pszNewCtrlTypeName, szId);
	if (pCtrlIfc == NULL)
		return NULL;

	IControlBase *pCtrlBase = dynamic_cast<IControlBase*>(pCtrlIfc);
	if (pCtrlBase == NULL)
	{
		m_pControlMgr->ReleaseCtrl(&pCtrlIfc);
		return NULL;
	}
	pCtrlBase->SetPropertySkinManager(m_pSkinMgr);

	IPropertyControl *pPropCtrl = dynamic_cast<IPropertyControl*>(m_pSkinMgr->CreateEmptyBaseProp(OTID_CONTROL));
	if (pPropCtrl == NULL)
	{
		m_pControlMgr->ReleaseCtrl(&pCtrlIfc);
		return NULL;
	}

	// 设置新控件的属性
	IPropertyGroup *pCtrlPropGroup = dynamic_cast<IPropertyGroup*>(m_pSkinMgr->CreateEmptyBaseProp(OTID_GROUP));
	if (pCtrlPropGroup == NULL)
	{
		m_pSkinMgr->ReleaseBaseProp((IPropertyBase*)pPropCtrl);
		m_pControlMgr->ReleaseCtrl(&pCtrlIfc);
		return NULL;
	}
	pPropCtrl->SetCtrlGroupProp(pCtrlPropGroup);
	pCtrlBase->SetUiEngine(GetUiEngine());
	pCtrlBase->SetPropertySkinManager(m_pSkinMgr);
	pCtrlBase->SetOwnerWindow(pParentWnd);
	pCtrlBase->SetParentControl(pParentCtrl);

	m_pSkinMgr->BD_AppendControlToVec(pszNewCtrlTypeName, pCtrlPropGroup);
	// 插入控件队列
	if (pParentCtrl != NULL)
	{
		pParentCtrl->AppendChildContrl(pCtrlBase);
		pParentCtrl->PP_GetControlPropetry()->AppendChildCtrlProp(pPropCtrl);
	}
	else
	{
		pParentWnd->AppendChildContrl(pCtrlBase);
		pParentWnd->PP_GetWindowPropetry()->AppendChildCtrlProp(pPropCtrl);
	}

	pCtrlBase->BD_InitControlBase(pPropCtrl, true);
	pCtrlBase->OnFinalCreate();
	return pCtrlBase;
}

IUiEngine* IUiFeatureKernelImpl::GetUiEngine()
{
	return IUiEngineImpl::GetInstance();
}
