//////////////////////////////////////////////////////////////////////////
// ��ǰƤ�����д��������пؼ������Թ�����

#pragma once
#include "IPropertyWindowManager.h"
#include "IPropertyBase.h"

class IPropertySkinManager
{
public:
	// ��ʼ��WindowƤ��
	virtual IPropertyWindowManager* InitWindowSkin(const char *pszSkinPath, const char *pszWndName) = 0;
//	// ����ָ��������
//	virtual IPropertyBase* FindBaseProperty(PROP_TYPE propType, const char* pszPropId) = 0;
//	virtual IPropertyBase* FindBaseProperty(const char* pszPropType, const char* pszPropId) = 0;
	// ������ʾ����������
	virtual void SetArea(AREA_TYPE areaType) = 0;
	// ����һ�����ԣ����������Է������
	virtual IPropertyBase* CreateEmptyBaseProp(PROP_TYPE propType) = 0;
	// ȡ��ID��
	virtual int GetNewId() = 0;
};