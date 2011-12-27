
#pragma once
#include "IWindowBase.h"

class IKernelWindow
{
public:
	virtual IPropertySkinManager* GetSkinManager() = 0;
//////////////////////////////////////////////////////////////////////////
// Builder ʹ�ú���
	// ȡ������֧�ֵĿؼ�
	virtual CONTROL_REG_MAP *BuilderRegisterControl() = 0;
	// ����һ��Builderʹ�õĴ���
	virtual IWindowBase* BuilderCreateWindow(IPropertyGroup *pWindowProp) = 0;

//////////////////////////////////////////////////////////////////////////
	// һ���Ի����һ��Ƥ������ʹ��ָ���ĶԻ���Ƥ����Դ��ʼ���Լ�
	// �����ͷ�Ƥ����Դ���ͷ����ڲ��Զ����
	virtual IWindowBase* InitFeatureSkin(HWND hWnd, char *pszSkinPath, char *pszWndName) = 0;
	// һ���Ի����ͷ�Ƥ����Դ�����Kernel�ں˻���ã��ⲿ�Ի�����Ҫ����
	virtual void ReleaseFeatureSkin(HWND hWnd) = 0;
};