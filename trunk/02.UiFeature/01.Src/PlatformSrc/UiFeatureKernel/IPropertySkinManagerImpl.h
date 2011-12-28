
#pragma once
#include "..\..\Inc\IPropertySkinManager.h"
#include "..\..\Inc\IPropertyWindow.h"
#include "..\..\Inc\IZipFileList.h"
#include "..\..\Inc\IPropertyGroup.h"

//////////////////////////////////////////////////////////////////////////
// ��Դ���Զ���


class IPropertySkinManagerImpl : public IPropertySkinManager
{
public:
	IPropertySkinManagerImpl(void);
	virtual ~IPropertySkinManagerImpl(void);

	static IPropertySkinManager* GetInstance();

	// ��ʼ��WindowƤ��
	virtual IPropertyGroup* InitWindowSkin(const char *pszSkinPath, const char *pszWndName);
	// ����ָ��������
	virtual IPropertyBase* FindBaseProperty(OBJECT_TYPE_ID propType, const char* pszPropId);
	virtual IPropertyBase* FindBaseProperty(const char* pszPropType, const char* pszPropId);
	// ������ʾ����������
	virtual void SetArea(AREA_TYPE areaType);
	// ����һ�����ԣ����������Է������
	virtual IPropertyBase* CreateEmptyBaseProp(OBJECT_TYPE_ID propType);
	// ȡ��ID��
	virtual int GetNewId();

private:
	void ReleaseSkinManager();
	// ��ʼ��Ƥ����
	bool InitSkinPackage(const char *pszSkinPath);
	// ����zip�ļ�
	void LoadZipDll();
	void ResetBaseObjectId(int nObjectId);

//////////////////////////////////////////////////////////////////////////
	// Resource.xml���
	// ����Resource.xml
	bool TranslateResourceXml(FILE_ITEM *pResurceXml);
	void ReleasePropMap(PROP_BASE_MAP &PropMap);
	void ReleaseBaseProp(IPropertyBase *pCtrlProp);
	void ReleasePropMapItem(PROP_BASE_ITEM* pPropMapItem);
	// ͨ�õĴ���Դxml�д�������
	bool GeneralCreateBaseProp(char *pPropType, XmlNode* pXmlNode, PROP_BASE_ITEM* pPropMap);

//////////////////////////////////////////////////////////////////////////
	// Controls.xml ���
	bool TranslateControlsXml(FILE_ITEM *pControlsXml);
	bool GeneralCreateSubProp(XmlNode* pXmlNode, PROP_BASE_ITEM* pCtrlPropMap);
	bool AppendBasePropToGroup(IPropertyGroup *pGroup, XmlNode* pXmlNode);

//////////////////////////////////////////////////////////////////////////
	// Windows.xml ���
	// ����Windows.xml
	bool TranslateWindowsXml(FILE_ITEM *pWindowsXml);

//////////////////////////////////////////////////////////////////////////
	// Layout.xml ���
	bool TranslateLayoutXml(FILE_ITEM *pLayoutXml);
	bool GeneralCreateWindowLayoutProp(XmlNode* pXmlNode, CHILD_CTRL_PROP_VEC* pChildCtrlVec, IPropertyControl* pParentProp);
	IPropertyGroup* FindControlPropGroup(char *pszObjectId);
	void ReleaseLayoutMap();
	void ReleaseLayoutMapChildCtrlVec(CHILD_CTRL_PROP_VEC* pCtrlVec);

private:
	// Ƥ��·��
	string m_strSkinPath;
	// ������������ԵĶ���
	PROP_BASE_MAP m_AllPropMap;
	// ���пؼ���������ԵĶ��У�KEY���ؼ��������ƣ��磺Button
	PROP_BASE_MAP m_AllCtrlPropMap;
	// ���д��ڵ����Թ�����map
	PROP_BASE_ITEM m_AllWindowPropMap;
	// ���ֶ���
	WINDOW_PROP_MAP m_LayoutWindowVec;

	// zip�ļ�
	IZipFileList *m_pZipFile;
	HMODULE m_hZipModule;

	AREA_TYPE m_AreaType;

	// ������ԴObjectId
	int m_nObjectIdInRes;
	int m_nEmptyObjectId;
};