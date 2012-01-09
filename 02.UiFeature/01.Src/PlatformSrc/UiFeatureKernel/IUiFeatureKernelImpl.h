
#pragma once
#include "..\..\Inc\IUiFeatureKernel.h"
#include "IUiEngineImpl.h"
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
	// Builder ʹ�ú���
	// ����/��һ���µ�Ƥ������
	virtual bool BD_OpenProject(char *pszSkinDir, char *pszSkinName);
	// ȡ������֧�ֵĿؼ�
	virtual CONTROL_REG_MAP *BD_GetRegisterControl();
	// ����һ��Builderʹ�õĿյĴ���
	virtual IWindowBase* BD_CreateWindowEmptyPropetry();
	// ����һ��Builderʹ�õĴ��ڣ�������������
	virtual IWindowBase* BD_CreateWindowByPropetry(IPropertyWindow *pPropWnd);
	// ����һ��Builderʹ�õĿյĿؼ�
	virtual IControlBase* BD_CreateControlEmptyPropetry(IWindowBase *pParentWnd, IControlBase *pParentCtrl, char *pszNewCtrlTypeName);
	// ����һ��Builderʹ�õĿؼ���������������
	virtual IControlBase* BD_CreateControlByPropetry(IWindowBase *pParentWnd, IControlBase *pParentCtrl, IPropertyControl *pPropCtrl);
	// ����Ƥ����
	virtual bool BD_SaveProject(char *pszSkinDir, char *pszSkinName);
	// �ر�һ������
	virtual bool BD_CloseProject();

//////////////////////////////////////////////////////////////////////////
	// һ���Ի����һ��Ƥ������ʹ��ָ���ĶԻ���Ƥ����Դ��ʼ���Լ�
	virtual IWindowBase* PG_InitFeatureSkin(HWND hWnd, char *pszSkinPath, char *pszWndName);
	// һ���Ի����ͷ�Ƥ����Դ�����Kernel�ں˻���ã��ⲿ�Ի�����Ҫ����
	virtual void PG_ReleaseFeatureSkin(HWND hWnd);

	// ��zip�ļ����ҵ�ָ���ļ�
	virtual bool FindUnZipFile(const char *pFileName, BYTE **ppOutBuffer, int *pnFileLen);
private:
	void ReleaseKernelWindow();

private:
	HMODULE m_hControlDll;
	IControlManager *m_pControlMgr;

	IUiEngineImpl *m_pUiEngine;
	IPropertySkinManagerImpl *m_pSkinMgr;

	WINDOW_IMPL_MAP m_WndImplMap;
	// ����֧�ֵĿؼ�
	CONTROL_REG_MAP m_CtrlRegMap;
	// Builder��ʹ�õ�hwnd������ֵ
	int m_nBuilderHwnd;
};