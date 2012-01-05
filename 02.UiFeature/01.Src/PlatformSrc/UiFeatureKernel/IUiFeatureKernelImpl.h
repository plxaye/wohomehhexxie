
#pragma once
#include "..\..\Inc\IUiFeatureKernel.h"
#include "IPropertySkinManagerImpl.h"
#include "..\..\Inc\IControlManager.h"

class IWindowBaseImpl;
typedef map<HWND, IWindowBaseImpl*>		WINDOW_IMPL_MAP;

class IUiFeatureKernelImpl : public IUiFeatureKernel
{
public:
	IUiFeatureKernelImpl(void);
	virtual ~IUiFeatureKernelImpl(void);

	static IUiFeatureKernel* GetInstance();

	virtual IPropertySkinManager* GetSkinManager();
	virtual IUiEngine* GetUiEngine();

//////////////////////////////////////////////////////////////////////////
	// Builder 使用函数
	// 创建/打开一个新的皮肤工程
	virtual bool BD_OpenProject(char *pszSkinDir, char *pszSkinName);
	// 取得所有支持的控件
	virtual CONTROL_REG_MAP *BD_GetRegisterControl();
	// 创建一个Builder使用的空的窗口
	virtual IWindowBase* BD_CreateWindowEmptyPropetry();
	// 创建一个Builder使用的窗口，并配置上属性
	virtual IWindowBase* BD_CreateWindowByPropetry(IPropertyWindow *pPropWnd);
	// 创建一个Builder使用的空的控件
	virtual IControlBase* BD_CreateControlEmptyPropetry(IWindowBase *pParentWnd, IControlBase *pParentCtrl, char *pszNewCtrlTypeName);
	// 保存皮肤包
	virtual bool BD_SaveProject(char *pszSkinDir, char *pszSkinName);
	// 关闭一个工程
	virtual bool BD_CloseProject();

//////////////////////////////////////////////////////////////////////////
	// 一个对话框从一个皮肤包里使用指定的对话框皮肤资源初始化自己
	virtual IWindowBase* PG_InitFeatureSkin(HWND hWnd, char *pszSkinPath, char *pszWndName);
	// 一个对话框释放皮肤资源，这个Kernel内核会调用，外部对话框不需要调用
	virtual void PG_ReleaseFeatureSkin(HWND hWnd);

	// 从zip文件中找到指定文件
	virtual bool FindUnZipFile(const char *pFileName, BYTE **ppOutBuffer, int *pnFileLen);
private:
	void ReleaseKernelWindow();

private:
	HMODULE m_hControlDll;
	IControlManager *m_pControlMgr;

	HMODULE m_hUiEngineDll;
	IUiEngine *m_pUiEngine;

	WINDOW_IMPL_MAP m_WndImplMap;
	// 所有支持的控件
	CONTROL_REG_MAP m_CtrlRegMap;
	// Builder中使用的hwnd的索引值
	int m_nBuilderHwnd;
	IPropertySkinManagerImpl *m_pSkinMgr;
};
