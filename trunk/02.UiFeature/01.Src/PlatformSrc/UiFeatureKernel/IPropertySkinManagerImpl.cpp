
#include "StdAfx.h"
#include "IPropertySkinManagerImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IPropertyFont.h"
#include "..\..\Inc\IPropertyBool.h"
#include "..\..\Inc\IPropertyColor.h"
#include "..\..\Inc\IPropertyComboBox.h"
#include "..\..\Inc\IPropertyCursor.h"
#include "..\..\Inc\IPropertyImage.h"
#include "..\..\Inc\IPropertyInt.h"
#include "..\..\Inc\IPropertyString.h"

// ������Դxml�ļ�
#define RESOURCE_XML_NAME				("Resource.xml")

IPropertySkinManagerImpl::IPropertySkinManagerImpl(void)
{
	m_strSkinPath = "";
	m_WndPropMap.clear();
	m_AllPropMap.clear();
}

IPropertySkinManagerImpl::~IPropertySkinManagerImpl(void)
{
	Release();
}

void IPropertySkinManagerImpl::Release()
{
	for (ALL_PROP_MAP::iterator pGroupItem = m_AllPropMap.begin(); pGroupItem != m_AllPropMap.end(); pGroupItem++)
	{
		PROP_MAP* pGroup = pGroupItem->second;
		if (pGroup != NULL)
		{
			for (PROP_MAP::iterator pPropItem = pGroup->begin(); pPropItem != pGroup->end(); pPropItem++)
			{
				IPropertyBase* pProp = pPropItem->second;
				ReleaseProp(pProp);
			}
			SAFE_DELETE(pGroup);
		}
	}
	m_AllPropMap.clear();
}

void IPropertySkinManagerImpl::ReleaseProp(IPropertyBase *pCtrlProp)
{
	if (pCtrlProp == NULL)
		return;

	switch (pCtrlProp->GetPropType())
	{
	case PT_BOOL:
		{
			IPropertyBool* pProp = dynamic_cast<IPropertyBool*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case PT_COLOR:
		{
			IPropertyColor* pProp = dynamic_cast<IPropertyColor*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case PT_COMBOBOX:
		{
			IPropertyComboBox* pProp = dynamic_cast<IPropertyComboBox*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case PT_CURSOR:
		{
			IPropertyCursor* pProp = dynamic_cast<IPropertyCursor*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case PT_FONT:
		{
			IPropertyFont* pProp = dynamic_cast<IPropertyFont*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case PT_IMAGE:
		{
			IPropertyImage* pProp = dynamic_cast<IPropertyImage*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case PT_INT:
		{
			IPropertyInt* pProp = dynamic_cast<IPropertyInt*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case PT_STRING:
		{
			IPropertyString* pProp = dynamic_cast<IPropertyString*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	default:
		break;
	}

	SAFE_DELETE(pCtrlProp);
}

// �������Թ���
// 1. �������Եļ�¼����
// 2. �������Ե�Ĭ��ֵ
// 3. ����������ʾ������ֵ
// 4. ������ʾ�õ�����


IPropertySkinManager* IPropertySkinManagerImpl::GetInstance()
{
	static IPropertySkinManagerImpl _PropertySkinManagerInstance;
	return &_PropertySkinManagerInstance;
}

// ����ָ��������
IPropertyBase* IPropertySkinManagerImpl::FindProperty(char* pszPropType, char* pszPropName)
{
	if (pszPropType == NULL || pszPropName == NULL || m_AllPropMap.size() <= 0)
		return NULL;

	string strType(pszPropType);
	ALL_PROP_MAP::iterator pPropTypeItem = m_AllPropMap.find(strType);
	if (pPropTypeItem == m_AllPropMap.end())
		return NULL;

	PROP_MAP* pPropMap = pPropTypeItem->second;
	if (pPropMap == NULL)
		return NULL;

	string strPropName(pszPropName);
	PROP_MAP::iterator pPropItem = pPropMap->find(strPropName);
	if (pPropItem == pPropMap->end())
		return NULL;

	return (pPropItem->second);
}

// ��ʼ��Ƥ����
bool IPropertySkinManagerImpl::InitSkinPackage(const char *pszSkinPath)
{
	// TBD����Builder����Ƥ���ļ����ǲ�һ����
	if (pszSkinPath == NULL || strlen(pszSkinPath) <= 0)
		return false;

	// ��ʼ��
	if (m_WndPropMap.size() > 0)
		return true;

//////////////////////////////////////////////////////////////////////////
	// TBD
	char szPath[MAX_PATH + 1];
	memset(szPath, 0, MAX_PATH + 1);
	::GetModuleFileNameA(NULL, szPath, MAX_PATH);
	while (strlen(szPath) > 0 && szPath[strlen(szPath) - 1] != '\\')
		szPath[strlen(szPath) - 1] = '\0';

	string strDir = szPath;
	strDir += SKIN_DATA_DIR;

	string strUfd = szPath;
	strUfd += SKIN_DATA_DIR;
	strUfd += "skintest.ufd";

	m_ZipFile.InitWriteZip((char*)strDir.c_str(), (char*)strUfd.c_str());
	m_ZipFile.WriteToZip("Layout.xml");
	m_ZipFile.WriteToZip("Resource.xml");
	m_ZipFile.WriteToZip("bk.PNG");
	m_ZipFile.WriteToZip("V5.Dlg.Close.png");
	m_ZipFile.WriteToZip("V5.Dlg.Mini.png");
	m_ZipFile.WriteToZip("V5.Logon.ACDC.png");
	m_ZipFile.WriteToZip("V5.Logon.Set.png");
	m_ZipFile.WriteToZip("V5.LogonBiz.01.jpg");
	m_ZipFile.WriteToZip("V5.LogonBiz.02.png");
	m_ZipFile.WriteToZip("����2��ע.png");
	m_ZipFile.WriteToZip("�����˵�Ч��ͼ_MarkMan.png");
	m_ZipFile.WriteToZip("��ͼ����.png");
	m_ZipFile.WriteToZip("�����޸�.png");
	m_ZipFile.EndWriteZip();
//////////////////////////////////////////////////////////////////////////

	// ��Ƥ���ļ��г�ʼ��Ƥ������ TBD
	m_strSkinPath = pszSkinPath;
	bool bOK = m_ZipFile.ReadZipFile(pszSkinPath);
	if (bOK)
	{
		// �������
		FILE_ITEM * pResurceXml = m_ZipFile.FindUnZipFile(RESOURCE_XML_NAME);
		bOK = TranslateResourceXml(pResurceXml);
	}

	return true;
}

// ����Resource.xml
bool IPropertySkinManagerImpl::TranslateResourceXml(FILE_ITEM *pResurceXml)
{
	if (pResurceXml == NULL || pResurceXml->pFileData == NULL)
		return false;

	XmlState xmlState = { 0 };
	JabberXmlInitState(&xmlState);
	int bytesParsed = JabberXmlParse(&xmlState, (char *)pResurceXml->pFileData, pResurceXml->dwSrcFileLen);
	XmlNode *pResurceRoot  = JabberXmlGetChild(&xmlState.root, "resource");
	if (pResurceRoot != NULL)
	{
		int nItemCount = pResurceRoot->numChild;
		for (int i = 0; i < nItemCount; i++)
		{
			XmlNode* pPropType = JabberXmlGetNthChildWithoutTag(pResurceRoot, i);
			if (pPropType != NULL && pPropType->name != NULL)
			{
				PROP_MAP* pOnePropMap = NULL;
				string strTypeName = pPropType->name;
				ALL_PROP_MAP::iterator pTypeItem = m_AllPropMap.find(strTypeName);
				if (pTypeItem != m_AllPropMap.end())
				{
					// �ҵ�������
					pOnePropMap = pTypeItem->second;
				}
				else
				{
					// ����������
					pOnePropMap = new PROP_MAP;
					if (pOnePropMap == NULL)
						return false;

					pOnePropMap->clear();
					m_AllPropMap.insert(pair<string, PROP_MAP*>(strTypeName, pOnePropMap));
				}

				if (pOnePropMap == NULL)
					return false;

				if (!GeneralCreateProp((char*)strTypeName.c_str(), pPropType, pOnePropMap))
					return false;
			}
		}
	}
	else
	{
		return false;
	}

	JabberXmlDestroyState(&xmlState);
	return true;
}

// �ж������Ƿ��Ѿ�����
bool IPropertySkinManagerImpl::IsPropExist(XmlNode* pXmlNode, PROP_MAP* pPropMap, string &strPropName)
{
	strPropName = "";
	if (pXmlNode == NULL || pPropMap == NULL)
		return false;

	char* psz_name = JabberXmlGetAttrValue(pXmlNode, SKIN_PROP_NAME);
	if (psz_name == NULL)
		return false;

	strPropName = psz_name;
	PROP_MAP::iterator pPropItem = pPropMap->find(strPropName);
	if (pPropItem == pPropMap->end())
		return false;

	return true;
}

// ��ʼ��Ƥ��
IPropertyWindow* IPropertySkinManagerImpl::InitWindowSkin(const char *pszSkinPath, const char *pszWndName)
{
	if (pszSkinPath == NULL || strlen(pszSkinPath) <= 0 || pszWndName == NULL || strlen(pszWndName) <= 0)
		return NULL;

	if (!InitSkinPackage(pszSkinPath))
		return NULL;

	string strWndName = pszWndName;
	WND_PROP_MAP::iterator pWndPropItem = m_WndPropMap.find(strWndName);
	if (pWndPropItem == m_WndPropMap.end())
		return NULL;

	IPropertyWindow* pWndProp = pWndPropItem->second;
	return pWndProp;
}

bool IPropertySkinManagerImpl::GeneralCreateProp(char *pPropType, XmlNode* pXmlNode, PROP_MAP* pPropMap)
{
	if (pPropType == NULL || pXmlNode == NULL || pPropMap == NULL)
		return false;

	int nItemCount = pXmlNode->numChild;
	for (int i = 0; i < nItemCount; i++)
	{
		XmlNode* pItem = JabberXmlGetNthChildWithoutTag(pXmlNode, i);
		if (pItem != NULL)
		{
			string strPropName = "";
			if (IsPropExist(pItem, pPropMap, strPropName))
				return false;

			IPropertyBase* pBaseProp = NULL;
			if (lstrcmpiA(pPropType, PROP_TYPE_FONT_NAME) == 0)
			{
				IPropertyFont* pProp = new IPropertyFont;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else if (lstrcmpiA(pPropType, PROP_TYPE_STRING_NAME) == 0)
			{
				IPropertyString* pProp = new IPropertyString;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else if (lstrcmpiA(pPropType, PROP_TYPE_COMBOBOX_NAME) == 0)
			{
				IPropertyComboBox* pProp = new IPropertyComboBox;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else if (lstrcmpiA(pPropType, PROP_TYPE_IMAGE_NAME) == 0)
			{
				IPropertyImage* pProp = new IPropertyImage;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else if (lstrcmpiA(pPropType, PROP_TYPE_CURSOR_NAME) == 0)
			{
				IPropertyCursor* pProp = new IPropertyCursor;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else if (lstrcmpiA(pPropType, PROP_TYPE_COLOR_NAME) == 0)
			{
				IPropertyColor* pProp = new IPropertyColor;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else
			{
				return false;
			}

			if (pBaseProp == NULL)
				return false;

			if (!pBaseProp->ReadResourceXmlProperty(pItem))
			{
				ReleaseProp(pBaseProp);
				return false;
			}

			pPropMap->insert(pair<string, IPropertyBase*>(strPropName, pBaseProp));
		}
	}

	return true;
}

// ������ʾ����������
void IPropertySkinManagerImpl::SetArea(AREA_TYPE areaType)
{
	ALL_PROP_MAP::iterator pStringItem = m_AllPropMap.find(PROP_TYPE_STRING_NAME);
	if (pStringItem == m_AllPropMap.end())
		return;

	PROP_MAP* pPropMap = pStringItem->second;
	if (pPropMap == NULL)
		return;

	for (PROP_MAP::iterator pPropItem = pPropMap->begin(); pPropItem != pPropMap->end(); pPropItem++)
	{
		IPropertyBase* pProp = pPropItem->second;
		if (pProp == NULL)
			continue;

		IPropertyString *pStringProp = dynamic_cast<IPropertyString*>(pProp);
		if (pStringProp == NULL)
			continue;

		pStringProp->SetArea(areaType);
	}
}