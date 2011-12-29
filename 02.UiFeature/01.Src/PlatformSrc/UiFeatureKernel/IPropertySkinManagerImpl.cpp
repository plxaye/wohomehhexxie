
#include "StdAfx.h"
#include <assert.h>
#include "IPropertySkinManagerImpl.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IPropertyFont.h"
#include "..\..\Inc\IPropertyBool.h"
#include "..\..\Inc\IPropertyColor.h"
#include "..\..\Inc\IPropertyComboBox.h"
#include "..\..\Inc\IPropertyCursor.h"
#include "..\..\Inc\IPropertyImage.h"
#include "..\..\Inc\IPropertyImageBase.h"
#include "..\..\Inc\IPropertyInt.h"
#include "..\..\Inc\IPropertyString.h"
#include "..\..\Inc\IPropertyGroup.h"
#include "IPropertyWindowManagerImpl.h"

IPropertySkinManager *GetSkinManagerInterface()
{
	return IPropertySkinManagerImpl::GetInstance();
}

IPropertySkinManager* IPropertySkinManagerImpl::GetInstance()
{
	static IPropertySkinManagerImpl _PropertySkinManagerInstance;
	return &_PropertySkinManagerInstance;
}

IPropertySkinManagerImpl::IPropertySkinManagerImpl(void)
{
	m_strSkinPath = "";
	m_AllPropMap.clear();
	m_AllCtrlPropMap.clear();
	m_AllWindowPropMap.clear();
	m_LayoutWindowVec.clear();
	m_pImageBasePropMap = NULL;
	m_pFontBasePropMap = NULL;
	m_pCursorBasePropMap = NULL;
	m_pColorBasePropMap = NULL;

	m_nObjectIdInRes = 1;
	m_nEmptyObjectId = 1;
	m_AreaType = AT_CN;

	LoadZipDll();
}

IPropertySkinManagerImpl::~IPropertySkinManagerImpl(void)
{
	ReleaseSkinManagerPropetry();
}

// 清空属性队列
void IPropertySkinManagerImpl::ReleaseSkinManagerPropetry()
{
	SAFE_FREE_LIBRARY(m_hZipModule);

	ReleaseLayoutMap();
	ReleasePropMapItem(&m_AllWindowPropMap);
	ReleasePropMap(m_AllCtrlPropMap);
	ReleasePropMap(m_AllPropMap);
}

void IPropertySkinManagerImpl::ReleaseLayoutMap()
{
	for (PROP_BASE_ITEM::iterator pWndItem = m_LayoutWindowVec.begin(); pWndItem != m_LayoutWindowVec.end(); pWndItem++)
	{
		IPropertyWindow* pWnd = dynamic_cast<IPropertyWindow*>(pWndItem->second);
		if (pWnd != NULL)
		{
			ReleaseLayoutMapChildCtrlVec(pWnd->GetChildControlVec());
			SAFE_DELETE(pWnd);
		}
	}
	m_LayoutWindowVec.clear();
}

void IPropertySkinManagerImpl::ReleaseLayoutMapChildCtrlVec(CHILD_CTRL_PROP_VEC* pCtrlVec)
{
	if (pCtrlVec == NULL)
		return;

	for (CHILD_CTRL_PROP_VEC::iterator pVecItem = pCtrlVec->begin(); pVecItem != pCtrlVec->end(); pVecItem++)
	{
		IPropertyControl* pCtrl = *pVecItem;
		if (pCtrl != NULL)
			ReleaseLayoutMapChildCtrlVec(pCtrl->GetChildControlVec());
		SAFE_DELETE(pCtrl);
	}
	pCtrlVec->clear();
}

void IPropertySkinManagerImpl::ReleasePropMap(PROP_BASE_MAP &PropMap)
{
	for (PROP_BASE_MAP::iterator pGroupItem = PropMap.begin(); pGroupItem != PropMap.end(); pGroupItem++)
	{
		PROP_BASE_ITEM* pGroup = pGroupItem->second;
		if (pGroup != NULL)
		{
			ReleasePropMapItem(pGroup);
			SAFE_DELETE(pGroup);
		}
	}
	PropMap.clear();
}

void IPropertySkinManagerImpl::ReleasePropMapItem(PROP_BASE_ITEM* pPropMapItem)
{
	if (pPropMapItem == NULL)
		return;

	for (PROP_BASE_ITEM::iterator pPropItem = pPropMapItem->begin(); pPropItem != pPropMapItem->end(); pPropItem++)
	{
		IPropertyBase* pProp = pPropItem->second;
		ReleaseBaseProp(pProp);
	}
	pPropMapItem->clear();
}

void IPropertySkinManagerImpl::ReleaseBaseProp(IPropertyBase *pCtrlProp)
{
	if (pCtrlProp == NULL)
		return;

	switch (pCtrlProp->GetObjectTypeId())
	{
	case OTID_BOOL:
		{
			IPropertyBool* pProp = dynamic_cast<IPropertyBool*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_COLOR:
		{
			IPropertyColor* pProp = dynamic_cast<IPropertyColor*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_COMBOBOX:
		{
			IPropertyComboBox* pProp = dynamic_cast<IPropertyComboBox*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CURSOR:
		{
			IPropertyCursor* pProp = dynamic_cast<IPropertyCursor*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_FONT:
		{
			IPropertyFont* pProp = dynamic_cast<IPropertyFont*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_GROUP:
		{
			IPropertyGroup* pProp = dynamic_cast<IPropertyGroup*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_IMAGE:
		{
			IPropertyImage* pProp = dynamic_cast<IPropertyImage*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_IMAGE_BASE:
		{
			IPropertyImageBase* pProp = dynamic_cast<IPropertyImageBase*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_INT:
		{
			IPropertyInt* pProp = dynamic_cast<IPropertyInt*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_STRING:
		{
			IPropertyString* pProp = dynamic_cast<IPropertyString*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_WINDOW:
		{
			IPropertyWindow* pProp = dynamic_cast<IPropertyWindow*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CONTROL:
		{
			IPropertyControl* pProp = dynamic_cast<IPropertyControl*>(pCtrlProp);
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

// 加载zip文件
void IPropertySkinManagerImpl::LoadZipDll()
{
	m_pZipFile = NULL;
	m_hZipModule = NULL;

	string strPath = PathHelper(NAME_ZIP_DLL);
	m_hZipModule = ::LoadLibraryA(strPath.c_str());
	if (m_hZipModule == NULL)
		return;

	GETZIPFILEINTERFACE GetZip = (GETZIPFILEINTERFACE)::GetProcAddress(m_hZipModule, "GetZipFileInterface");
	if (GetZip == NULL)
		return;

	m_pZipFile = GetZip();
}

// 创建一个属性，并将此属性放入队列
IPropertyBase* IPropertySkinManagerImpl::CreateEmptyBaseProp(OBJECT_TYPE_ID propType, char *pszObjectId)
{
	IPropertyBase* pBaseProp = NULL;
	switch (propType)
	{
	case OTID_BOOL:
		{
			IPropertyBool* pProp = new IPropertyBool;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_COLOR:
		{
			IPropertyColor* pProp = new IPropertyColor;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_COMBOBOX:
		{
			IPropertyComboBox* pProp = new IPropertyComboBox;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_CURSOR:
		{
			IPropertyCursor* pProp = new IPropertyCursor;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_FONT:
		{
			IPropertyFont* pProp = new IPropertyFont;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_GROUP:
		{
			IPropertyGroup* pProp = new IPropertyGroup;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_IMAGE:
		{
			IPropertyImage* pProp = new IPropertyImage;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_IMAGE_BASE:
		{
			IPropertyImageBase* pProp = new IPropertyImageBase;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_INT:
		{
			IPropertyInt* pProp = new IPropertyInt;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_STRING:
		{
			IPropertyString* pProp = new IPropertyString;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_WINDOW:
		{
			IPropertyWindow* pProp = new IPropertyWindow;
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	default:
		break;
	}

	if (pBaseProp != NULL)
	{
		char szObjId[MAX_PATH + 1];
		memset(szObjId, 0, MAX_PATH + 1);
		if (pszObjectId == NULL)
			sprintf_s(szObjId, MAX_PATH, "NO_OBJ_ID_%d", m_nEmptyObjectId++);
		else
			strcpy_s(szObjId, MAX_PATH, pszObjectId);
		pBaseProp->SetObjectId((const char*)szObjId);

		if (propType == OTID_WINDOW)
		{
			// 创建窗口属性
			IPropertyWindow* pWndProp = dynamic_cast<IPropertyWindow*>(pBaseProp);
			if (pWndProp == NULL)
				ReleaseBaseProp(pBaseProp);

			m_LayoutWindowVec.insert(pair<string, IPropertyWindow*>(szObjId, pWndProp));
		}
		else if (propType == OTID_CONTROL)
		{
			// 创建控件属性
		}
		else
		{
			// 创建普通属性
			PROP_BASE_ITEM* pOnePropMap = NULL;
			string strTypeName = PropTypeToString(propType);
			PROP_BASE_MAP::iterator pTypeItem = m_AllPropMap.find(strTypeName);
			if (pTypeItem != m_AllPropMap.end())
			{
				// 找到属性组
				pOnePropMap = pTypeItem->second;
			}
			else
			{
				// 创建属性组
				pOnePropMap = new PROP_BASE_ITEM;
				if (pOnePropMap == NULL)
				{
					ReleaseBaseProp(pBaseProp);
					return NULL;
				}

				pOnePropMap->clear();
				m_AllPropMap.insert(pair<string, PROP_BASE_ITEM*>(strTypeName, pOnePropMap));
			}

			if (pOnePropMap != NULL)
				pOnePropMap->insert(pair<string, IPropertyBase*>(szObjId, pBaseProp));
		}
	}

	return pBaseProp;
}

// 查找指定的属性
IPropertyBase* IPropertySkinManagerImpl::FindBaseProperty(OBJECT_TYPE_ID propType, const char* pszPropId)
{
	string strType = PropTypeToString(propType);
	return FindBaseProperty(strType.c_str(), pszPropId);
}

IPropertyBase* IPropertySkinManagerImpl::FindBaseProperty(const char* pszPropType, const char* pszPropId)
{
	if (pszPropType == NULL || strlen(pszPropType) <= 0 || pszPropId == NULL || strlen(pszPropId) <= 0 || m_AllPropMap.size() <= 0)
		return NULL;

	string strType = pszPropType;
	PROP_BASE_MAP::iterator pPropTypeItem = m_AllPropMap.find(strType);
	if (pPropTypeItem == m_AllPropMap.end())
		return NULL;

	PROP_BASE_ITEM* pPropMap = pPropTypeItem->second;
	if (pPropMap == NULL)
		return NULL;

	string strPropId = pszPropId;
	PROP_BASE_ITEM::iterator pPropItem = pPropMap->find(strPropId);
	if (pPropItem == pPropMap->end())
		return NULL;

	return (pPropItem->second);
}

// 初始化皮肤包
bool IPropertySkinManagerImpl::InitSkinPackage(const char *pszSkinPath)
{
	// TBD，从Builder来和皮肤文件来是不一样的
	if (pszSkinPath == NULL || strlen(pszSkinPath) <= 0 || m_pZipFile == NULL)
		return false;

	// 初始化
	if (m_AllWindowPropMap.size() > 0)
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

	m_pZipFile->InitWriteZip((char*)strDir.c_str(), (char*)strUfd.c_str());
	m_pZipFile->WriteToZip(WINDOWS_XML_NAME);
	m_pZipFile->WriteToZip(RESOURCE_XML_NAME);
	m_pZipFile->WriteToZip(LAYOUT_XML_NAME);
	m_pZipFile->WriteToZip(CONTROLS_XML_NAME);
	m_pZipFile->WriteToZip("bk.PNG");
	m_pZipFile->WriteToZip("V5.Dlg.Close.png");
	m_pZipFile->WriteToZip("V5.Dlg.Mini.png");
	m_pZipFile->WriteToZip("V5.Logon.ACDC.png");
	m_pZipFile->WriteToZip("V5.Logon.Set.png");
	m_pZipFile->WriteToZip("V5.LogonBiz.01.jpg");
	m_pZipFile->WriteToZip("V5.LogonBiz.02.png");
	m_pZipFile->WriteToZip("下拉2标注.png");
	m_pZipFile->WriteToZip("下拉菜单效果图_MarkMan.png");
	m_pZipFile->WriteToZip("切图副本.png");
	m_pZipFile->WriteToZip("最新修改.png");
	m_pZipFile->EndWriteZip();
//////////////////////////////////////////////////////////////////////////

	// 从皮肤文件中初始化皮肤队列 TBD
	m_strSkinPath = pszSkinPath;
	if (!m_pZipFile->ReadZipFile(pszSkinPath))
		return false;

	FILE_ITEM * pResurceXml = m_pZipFile->FindUnZipFile(RESOURCE_XML_NAME);
	FILE_ITEM * pControlsXml = m_pZipFile->FindUnZipFile(CONTROLS_XML_NAME);
	FILE_ITEM * pWindowsXml = m_pZipFile->FindUnZipFile(WINDOWS_XML_NAME);
	FILE_ITEM * pLayoutXml = m_pZipFile->FindUnZipFile(LAYOUT_XML_NAME);
	if (pWindowsXml == NULL || pResurceXml == NULL)
		return false;

	// 解读 Resource.xml
	if (!TranslateResourceXml(pResurceXml))
		return false;
	m_pZipFile->RemoveFile(pResurceXml);

	// 解读 Controls.xml
	if (!TranslateControlsXml(pControlsXml))
		return false;
	m_pZipFile->RemoveFile(pControlsXml);

	// 解读 Windows.xml
	if (!TranslateWindowsXml(pWindowsXml))
		return false;
	m_pZipFile->RemoveFile(pWindowsXml);

	// 解析 Layout.xml
	if (!TranslateLayoutXml(pLayoutXml))
		return false;
	m_pZipFile->RemoveFile(pLayoutXml);

	return true;
}

// 初始化皮肤
IPropertyGroup* IPropertySkinManagerImpl::InitWindowSkin(const char *pszSkinPath, const char *pszWndName)
{
	if (pszSkinPath == NULL || strlen(pszSkinPath) <= 0 || pszWndName == NULL || strlen(pszWndName) <= 0)
		return NULL;

	if (!InitSkinPackage(pszSkinPath))
		return NULL;

	string strWndName = pszWndName;
	PROP_BASE_ITEM::iterator pWndPropItem = m_AllWindowPropMap.find(strWndName);
	if (pWndPropItem == m_AllWindowPropMap.end())
		return NULL;

	IPropertyBase* pBaseProp = pWndPropItem->second;
	if (pBaseProp == NULL)
		return NULL;

	IPropertyGroup* pPropGroup = dynamic_cast<IPropertyGroup*>(pBaseProp);
	return pPropGroup;
}

void IPropertySkinManagerImpl::ResetBaseObjectId(int nObjectId)
{
	if (nObjectId >= m_nObjectIdInRes)
		m_nObjectIdInRes = nObjectId + 1;
}

// 取得ID号
int IPropertySkinManagerImpl::GetNewId()
{
	return m_nObjectIdInRes++;
}

// 设置显示的语言种类
void IPropertySkinManagerImpl::SetArea(AREA_TYPE areaType)
{
	PROP_BASE_MAP::iterator pStringItem = m_AllPropMap.find(PROP_TYPE_STRING_NAME);
	if (pStringItem == m_AllPropMap.end())
		return;

	PROP_BASE_ITEM* pPropMap = pStringItem->second;
	if (pPropMap == NULL)
		return;

	for (PROP_BASE_ITEM::iterator pPropItem = pPropMap->begin(); pPropItem != pPropMap->end(); pPropItem++)
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

// 解析Resource.xml
bool IPropertySkinManagerImpl::TranslateResourceXml(FILE_ITEM *pResurceXml)
{
	ReleasePropMap(m_AllPropMap);
	if (pResurceXml == NULL || pResurceXml->pFileData == NULL)
		return false;

	XmlState xmlState = { 0 };
	JabberXmlInitState(&xmlState);
	int bytesParsed = JabberXmlParse(&xmlState, (char *)pResurceXml->pFileData, pResurceXml->dwSrcFileLen);
	XmlNode *pResurceRoot = JabberXmlGetChild(&xmlState.root, "resource");
	if (pResurceRoot != NULL)
	{
		int nItemCount = pResurceRoot->numChild;
		for (int i = 0; i < nItemCount; i++)
		{
			XmlNode* pPropTypeXml = JabberXmlGetNthChildWithoutTag(pResurceRoot, i);
			if (pPropTypeXml != NULL && pPropTypeXml->name != NULL)
			{
				PROP_BASE_ITEM* pOnePropMap = NULL;
				string strTypeName = pPropTypeXml->name;
				PROP_BASE_MAP::iterator pTypeItem = m_AllPropMap.find(strTypeName);
				if (pTypeItem != m_AllPropMap.end())
				{
					// 找到属性组
					pOnePropMap = pTypeItem->second;
				}
				else
				{
					// 创建属性组
					pOnePropMap = new PROP_BASE_ITEM;
					if (pOnePropMap == NULL)
						return false;

					pOnePropMap->clear();
					m_AllPropMap.insert(pair<string, PROP_BASE_ITEM*>(strTypeName, pOnePropMap));
				}

				if (pOnePropMap == NULL)
					return false;

				if (!GeneralCreateBaseProp((char*)strTypeName.c_str(), pPropTypeXml, pOnePropMap))
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

bool IPropertySkinManagerImpl::GeneralCreateBaseProp(char *pPropType, XmlNode* pXmlNode, PROP_BASE_ITEM* pPropMap)
{
	if (pPropType == NULL || pXmlNode == NULL || pPropMap == NULL)
		return false;

	int nItemCount = pXmlNode->numChild;
	for (int i = 0; i < nItemCount; i++)
	{
		XmlNode* pItem = JabberXmlGetNthChildWithoutTag(pXmlNode, i);
		if (pItem != NULL)
		{
			char* psz_id = JabberXmlGetAttrValue(pItem, SKIN_OBJECT_ID);
			if (psz_id == NULL)
				return false;

			string strObjId = psz_id;
			PROP_BASE_ITEM::iterator pPropItem = pPropMap->find(strObjId);
			if (pPropItem != pPropMap->end())
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
			else if (lstrcmpiA(pPropType, PROP_TYPE_BOOL_NAME) == 0)
			{
				IPropertyBool* pProp = new IPropertyBool;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else if (lstrcmpiA(pPropType, PROP_TYPE_INT_NAME) == 0)
			{
				IPropertyInt* pProp = new IPropertyInt;
				if (pProp == NULL)
					return false;
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
				{
					SAFE_DELETE(pProp);
				}
			}
			else if (lstrcmpiA(pPropType, PROP_TYPE_GROUP_NAME) == 0)
			{
				IPropertyGroup* pProp = new IPropertyGroup;
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

			if (!pBaseProp->ReadPropertyFromXmlNode(pItem))
			{
				ReleaseBaseProp(pBaseProp);
				return false;
			}

			pPropMap->insert(pair<string, IPropertyBase*>(strObjId, pBaseProp));
		}
	}

	return true;
}

bool IPropertySkinManagerImpl::TranslateControlsXml(FILE_ITEM *pControlsXml)
{
	ReleasePropMap(m_AllCtrlPropMap);
	if (pControlsXml == NULL || pControlsXml->pFileData == NULL)
		return false;

	XmlState xmlState = { 0 };
	JabberXmlInitState(&xmlState);
	int bytesParsed = JabberXmlParse(&xmlState, (char *)pControlsXml->pFileData, pControlsXml->dwSrcFileLen);
	XmlNode *pControlsRoot = JabberXmlGetChild(&xmlState.root, "controls");
	if (pControlsRoot != NULL)
	{
		int nItemCount = pControlsRoot->numChild;
		for (int i = 0; i < nItemCount; i++)
		{
			XmlNode* pCtrlTypeXml = JabberXmlGetNthChildWithoutTag(pControlsRoot, i);
			if (pCtrlTypeXml != NULL && pCtrlTypeXml->name != NULL)
			{
				PROP_BASE_ITEM* pOneCtrlMap = NULL;
				string strTypeName = pCtrlTypeXml->name;
				PROP_BASE_MAP::iterator pTypeItem = m_AllCtrlPropMap.find(strTypeName);
				if (pTypeItem != m_AllCtrlPropMap.end())
				{
					// 找到属性组
					pOneCtrlMap = pTypeItem->second;
				}
				else
				{
					// 创建属性组
					pOneCtrlMap = new PROP_BASE_ITEM;
					if (pOneCtrlMap == NULL)
						return false;

					pOneCtrlMap->clear();
					m_AllCtrlPropMap.insert(pair<string, PROP_BASE_ITEM*>(strTypeName, pOneCtrlMap));
				}

				if (pOneCtrlMap == NULL)
					return false;

				if (!GeneralCreateSubProp(pCtrlTypeXml, pOneCtrlMap))
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

bool IPropertySkinManagerImpl::GeneralCreateSubProp(XmlNode* pXmlNode, PROP_BASE_ITEM* pCtrlPropMap)
{
	if (pXmlNode == NULL || pCtrlPropMap == NULL)
		return false;

	char* psz_id = JabberXmlGetAttrValue(pXmlNode, SKIN_OBJECT_ID);
	if (psz_id == NULL)
		return false;

	string strObjId = psz_id;
	PROP_BASE_ITEM::iterator pFindGroupItem = pCtrlPropMap->find(strObjId);
	if (pFindGroupItem != pCtrlPropMap->end())
		return false;

	// 创建一个窗口或者一个控件的所有属性节点的根节点：属性组
	IPropertyGroup* pCtrlProp = new IPropertyGroup;
	if (pCtrlProp == NULL)
		return false;
	pCtrlProp->SetObjectId(psz_id);
	pCtrlProp->SetObjectType(PROP_ROOT_TYPE_NAME);

	IPropertyBase* pBaseProp = dynamic_cast<IPropertyBase*>(pCtrlProp);
	if (pBaseProp == NULL)
	{
		SAFE_DELETE(pCtrlProp);
		return false;
	}
	pCtrlPropMap->insert(pair<string, IPropertyBase*>(strObjId, pBaseProp));

	int nPropCount = pXmlNode->numChild;
	for (int nPropNo = 0; nPropNo < nPropCount; nPropNo++)
	{
		XmlNode* pPropNode = JabberXmlGetNthChildWithoutTag(pXmlNode, nPropNo);
		if (pPropNode != NULL && pPropNode->name != NULL)
		{
			psz_id = JabberXmlGetAttrValue(pPropNode, SKIN_OBJECT_ID);
			if (psz_id == NULL)
				return false;

			IPropertyBase* pFindBaseProp = FindBaseProperty(pPropNode->name, psz_id);
			if (pFindBaseProp == NULL)
				return false;

			pCtrlProp->AppendProperty(pFindBaseProp);
			if (pFindBaseProp->GetObjectTypeId() == OTID_GROUP)
			{
				IPropertyGroup *pNewGroup = dynamic_cast<IPropertyGroup*>(pFindBaseProp);
				if (pNewGroup == NULL)
					return false;

				if (!AppendBasePropToGroup(pNewGroup, pPropNode))
					return false;
			}
		}
	}

	return true;
}

bool IPropertySkinManagerImpl::AppendBasePropToGroup(IPropertyGroup *pGroup, XmlNode* pXmlNode)
{
	if (pGroup == NULL || pXmlNode == NULL)
		return false;

	int nItemCount = pXmlNode->numChild;
	for (int i = 0; i < nItemCount; i++)
	{
		XmlNode* pPropNode = JabberXmlGetNthChildWithoutTag(pXmlNode, i);
		if (pPropNode != NULL && pPropNode->name != NULL)
		{
			char* psz_id = JabberXmlGetAttrValue(pPropNode, SKIN_OBJECT_ID);
			if (psz_id == NULL)
				return false;

			IPropertyBase* pFindBaseProp = FindBaseProperty(pPropNode->name, psz_id);
			if (pFindBaseProp == NULL)
				return false;

			pGroup->AppendProperty(pFindBaseProp);
			if (pFindBaseProp->GetObjectTypeId() == OTID_GROUP)
			{
				IPropertyGroup *pNewGroup = dynamic_cast<IPropertyGroup*>(pFindBaseProp);
				if (pNewGroup == NULL)
					return false;

				if (!AppendBasePropToGroup(pNewGroup, pPropNode))
					return false;
			}
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 解析Windows.xml
bool IPropertySkinManagerImpl::TranslateWindowsXml(FILE_ITEM *pWindowsXml)
{
	ReleasePropMapItem(&m_AllWindowPropMap);
	if (pWindowsXml == NULL || pWindowsXml->pFileData == NULL)
		return false;

	XmlState xmlState = { 0 };
	JabberXmlInitState(&xmlState);
	int bytesParsed = JabberXmlParse(&xmlState, (char *)pWindowsXml->pFileData, pWindowsXml->dwSrcFileLen);
	XmlNode *pWindowsRoot = JabberXmlGetChild(&xmlState.root, "windows");
	if (pWindowsRoot != NULL)
	{
		int nItemCount = pWindowsRoot->numChild;
		for (int i = 0; i < nItemCount; i++)
		{
			XmlNode* pWindowNode = JabberXmlGetNthChildWithoutTag(pWindowsRoot, i);
			if (!GeneralCreateSubProp(pWindowNode, &m_AllWindowPropMap))
				return false;
		}
	}
	else
	{
		return false;
	}

	JabberXmlDestroyState(&xmlState);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 解析Windows.xml
bool IPropertySkinManagerImpl::TranslateLayoutXml(FILE_ITEM *pLayoutXml)
{
	ReleaseLayoutMap();
	if (pLayoutXml == NULL || pLayoutXml->pFileData == NULL)
		return false;

	XmlState xmlState = { 0 };
	JabberXmlInitState(&xmlState);
	int bytesParsed = JabberXmlParse(&xmlState, (char *)pLayoutXml->pFileData, pLayoutXml->dwSrcFileLen);
	XmlNode *pLayoutRoot = JabberXmlGetChild(&xmlState.root, "layout");
	if (pLayoutRoot != NULL)
	{
		char* psz_area = JabberXmlGetAttrValue(pLayoutRoot, "area");
		char* psz_lastid = JabberXmlGetAttrValue(pLayoutRoot, "lastid");
		if (psz_area == NULL || psz_lastid == NULL)
			return false;

		AREA_TYPE areaType = (AREA_TYPE)atoi(psz_area);
		SetArea(areaType);
		m_nObjectIdInRes = atoi(psz_lastid);

		int nItemCount = pLayoutRoot->numChild;
		for (int i = 0; i < nItemCount; i++)
		{
			XmlNode* pWindowNode = JabberXmlGetNthChildWithoutTag(pLayoutRoot, i);
			if (pWindowNode != NULL)
			{
				char* psz_id = JabberXmlGetAttrValue(pWindowNode, SKIN_OBJECT_ID);
				if (psz_id == NULL)
					return false;

				string strObjId = psz_id;
				PROP_BASE_ITEM::iterator pWndPropGroupItem = m_AllWindowPropMap.find(strObjId);
				if (pWndPropGroupItem == m_AllWindowPropMap.end())
					return false;

				IPropertyGroup *pWndPropGroup = dynamic_cast<IPropertyGroup*>(pWndPropGroupItem->second);
				if (pWndPropGroup == NULL)
					return false;

				PROP_BASE_ITEM::iterator pWndItem = m_LayoutWindowVec.find(strObjId);
				if (pWndItem != m_LayoutWindowVec.end())
					return false;

				IPropertyWindow* pOneWndLayoutProp = dynamic_cast<IPropertyWindow*>(CreateEmptyBaseProp(OTID_WINDOW, psz_id));
				if (pOneWndLayoutProp == NULL)
					return false;

				pOneWndLayoutProp->SetWindowPropGroup(pWndPropGroup);
				if (!GeneralCreateWindowLayoutProp(pWindowNode, pOneWndLayoutProp->GetChildControlVec(), NULL))
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

bool IPropertySkinManagerImpl::GeneralCreateWindowLayoutProp(XmlNode* pXmlNode, CHILD_CTRL_PROP_VEC* pChildCtrlVec, IPropertyControl* pParentProp)
{
	if (pXmlNode == NULL || pChildCtrlVec == NULL)
		return false;

	int nItemCount = pXmlNode->numChild;
	for (int i = 0; i < nItemCount; i++)
	{
		XmlNode* pCtrlNode = JabberXmlGetNthChildWithoutTag(pXmlNode, i);
		if (pCtrlNode != NULL)
		{
			char* psz_id = JabberXmlGetAttrValue(pCtrlNode, SKIN_OBJECT_ID);
			if (psz_id == NULL)
				return false;

			IPropertyGroup* pCtrlPropGroup = FindControlPropGroup(psz_id);
			if (pCtrlPropGroup == NULL)
				return false;

			IPropertyControl* pCtrlProp = dynamic_cast<IPropertyControl*>(CreateEmptyBaseProp(OTID_CONTROL, psz_id));
			if (pCtrlProp == NULL)
				return false;

			pCtrlProp->SetControlBaseProp(pCtrlPropGroup);
			pCtrlProp->SetParentProp(pParentProp);
			pChildCtrlVec->push_back(pCtrlProp);

			if (pCtrlNode->numChild > 0)
			{
				if (!GeneralCreateWindowLayoutProp(pCtrlNode, pCtrlProp->GetChildControlVec(), pCtrlProp))
					return false;
			}
		}
	}

	return true;
}

IPropertyGroup* IPropertySkinManagerImpl::FindControlPropGroup(char *pszObjectId)
{
	if (pszObjectId == NULL)
		return NULL;

	for (PROP_BASE_MAP::iterator pGroupItem = m_AllCtrlPropMap.begin(); pGroupItem != m_AllCtrlPropMap.end(); pGroupItem++)
	{
		PROP_BASE_ITEM* pGroup = pGroupItem->second;
		if (pGroup != NULL)
		{
			string strObjId = pszObjectId;
			PROP_BASE_ITEM::iterator pPropItem = pGroup->find(strObjId);
			if (pPropItem == pGroup->end())
				continue;
			
			IPropertyBase* pPropBase = pPropItem->second;
			if (pPropBase == NULL)
				continue;

			IPropertyGroup* pFindGroup = dynamic_cast<IPropertyGroup*>(pPropBase);
			return pFindGroup;
		}
	}

	return NULL;
}

// 解析Resource.xml
bool IPropertySkinManagerImpl::BuilderTranslateResourceXml(char *pszXmlPath)
{
	FILE_ITEM FileItem;
	if (!BuilderCreateFileItem(pszXmlPath, FileItem))
		return false;

	bool bRet = TranslateResourceXml(&FileItem);

	BuilderFreeFileItem(FileItem);
	return bRet;
}

// 解析Controls.xml
bool IPropertySkinManagerImpl::BuilderTranslateControlsXml(char *pszXmlPath)
{
	FILE_ITEM FileItem;
	if (!BuilderCreateFileItem(pszXmlPath, FileItem))
		return false;

	bool bRet = TranslateControlsXml(&FileItem);

	BuilderFreeFileItem(FileItem);
	return bRet;
}

// 解析Windows.xml
bool IPropertySkinManagerImpl::BuilderTranslateWindowsXml(char *pszXmlPath)
{
	FILE_ITEM FileItem;
	if (!BuilderCreateFileItem(pszXmlPath, FileItem))
		return false;

	bool bRet = TranslateWindowsXml(&FileItem);

	BuilderFreeFileItem(FileItem);
	return bRet;
}

// 解析Layout.xml
bool IPropertySkinManagerImpl::BuilderTranslateLayoutXml(char *pszXmlPath)
{
	FILE_ITEM FileItem;
	if (!BuilderCreateFileItem(pszXmlPath, FileItem))
		return false;

	bool bRet = TranslateLayoutXml(&FileItem);

	BuilderFreeFileItem(FileItem);
	return bRet;
}

bool IPropertySkinManagerImpl::BuilderCreateFileItem(char *pFilePath, FILE_ITEM &FileItem)
{
	FileItem.strFileName = "";
	FileItem.dwSrcFileLen = 0;
	FileItem.dwZipDatalen = 0;
	FileItem.pFileData = NULL;

	if (pFilePath == NULL)
		return false;

	WIN32_FILE_ATTRIBUTE_DATA FileAttr;
	if (!::GetFileAttributesExA(pFilePath, GetFileExInfoStandard, &FileAttr))
		return false;

	FILE *pFile = NULL;
	fopen_s(&pFile, pFilePath, "rb");
	if (pFile == NULL)
		return false;

	FileItem.pFileData = new BYTE[FileAttr.nFileSizeLow];
	if (FileItem.pFileData == NULL)
	{
		fclose(pFile);
		return false;
	}

	int nReadCtns = 0;
	while(nReadCtns < (int)FileAttr.nFileSizeLow)
	{
		BYTE *pRead = FileItem.pFileData + nReadCtns;
		int nNeedRead = FileAttr.nFileSizeLow - nReadCtns;

		int nRead = fread_s(pRead, nNeedRead, 1, nNeedRead, pFile);
		nReadCtns += nRead;

		if (errno != 0)
		{
			SAFE_DELETE_LIST(FileItem.pFileData);
			fclose(pFile);
			return false;
		}
	}

	if (nReadCtns != (int)FileAttr.nFileSizeLow)
	{
		SAFE_DELETE_LIST(FileItem.pFileData);
		fclose(pFile);
		return false;
	}

	FileItem.dwSrcFileLen = FileAttr.nFileSizeLow;
	return true;
}

void IPropertySkinManagerImpl::BuilderFreeFileItem(FILE_ITEM &FileItem)
{
	if (FileItem.pFileData != NULL)
		SAFE_DELETE_LIST(FileItem.pFileData);

	FileItem.strFileName = "";
	FileItem.dwSrcFileLen = 0;
	FileItem.dwZipDatalen = 0;
	FileItem.pFileData = NULL;
}

PROP_BASE_ITEM* IPropertySkinManagerImpl::BuilderGetWindowPropMap()
{
	return &m_LayoutWindowVec;
}
