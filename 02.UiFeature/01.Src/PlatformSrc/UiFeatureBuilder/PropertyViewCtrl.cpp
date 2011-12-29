#include "StdAfx.h"
#include "PropertyViewCtrl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "PropetryDialogGridProperty.h"
#include "WindowsViewTree.h"

CPropertyViewCtrl::CPropertyViewCtrl(void)
{
	m_pCurrentPropGroup = NULL;
	m_pKernelWindow = NULL;
	m_pViewTree = NULL;
}

CPropertyViewCtrl::~CPropertyViewCtrl(void)
{
}

void CPropertyViewCtrl::Init(IKernelWindow* pKernelWindow, CWindowsViewTree *pViewTree)
{
	if (pKernelWindow == NULL || pViewTree == NULL)
		return;

	m_pKernelWindow = pKernelWindow;
	m_pViewTree = pViewTree;
}

void CPropertyViewCtrl::SetShowPropGroup(IPropertyGroup *pPropGroup)
{
	ClearAll();
	if (pPropGroup == NULL)
		return;

	m_pCurrentPropGroup = pPropGroup;
	AppendPropGroup(NULL, m_pCurrentPropGroup);
	this->RedrawWindow();
}

void CPropertyViewCtrl::AppendPropGroup(CMFCPropertyGridProperty* pParentPropGroup, IPropertyGroup *pPropGroup)
{
	USES_CONVERSION;
	if (pPropGroup == NULL)
		return;

	GROUP_PROP_VEC *pPropVec = pPropGroup->GetPropVec();
	if (pPropVec == NULL)
		return;

	for (GROUP_PROP_VEC::iterator pPropItem = pPropVec->begin(); pPropItem != pPropVec->end(); pPropItem++)
	{
		IPropertyBase* pProp = *pPropItem;
		if (pProp == NULL)
			continue;

		OBJECT_TYPE_ID ObjTypeId = pProp->GetObjectTypeId();
		if (ObjTypeId == OTID_GROUP)
		{
			// ����һ��������
			CString strName = A2W(pProp->GetObjectName());
			CMFCPropertyGridProperty* pShowPropGroup = new CMFCPropertyGridProperty(strName);
			if (pShowPropGroup == NULL)
				continue;

			pShowPropGroup->SetData((DWORD_PTR)pProp);
			AppendPropGroup(pShowPropGroup, (IPropertyGroup*)pProp);

			if (pParentPropGroup != NULL)
				pParentPropGroup->AddSubItem(pShowPropGroup);
			else
				this->AddProperty(pShowPropGroup);
			continue;
		}

		// ��ͨ����
		switch (ObjTypeId)
		{
		case OTID_BOOL:
			AppendBoolProp(pParentPropGroup, (IPropertyBool*)pProp);
			break;

		case OTID_COLOR:
			AppendColorProp(pParentPropGroup, (IPropertyColor*)pProp);
			break;

		case OTID_COMBOBOX:
			AppendComboBoxProp(pParentPropGroup, (IPropertyComboBox*)pProp);
			break;

		case OTID_CURSOR:
			AppendCursorProp(pParentPropGroup, (IPropertyCursor*)pProp);
			break;

		case OTID_FONT:
			AppendFontProp(pParentPropGroup, (IPropertyFont*)pProp);
			break;

		case OTID_IMAGE:
			AppendImageProp(pParentPropGroup, (IPropertyImage*)pProp);
			break;

		case OTID_INT:
			AppendIntProp(pParentPropGroup, (IPropertyInt*)pProp);
			break;

		case OTID_STRING:
			AppendStringProp(pParentPropGroup, (IPropertyString*)pProp);
			break;

		default:
			continue;
		}
	}
}

void CPropertyViewCtrl::AppendBoolProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyBool *pBoolProp)
{
	USES_CONVERSION;
	if (pParentPropGroup == NULL || pBoolProp == NULL)
		return;

	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pBoolProp);
	if (pPropBase == NULL)
		return;

	CString strName = A2W(pPropBase->GetObjectName());
	CString strInfo = A2W(pPropBase->GetObjectInfo());

	CMFCPropertyGridProperty *pNewProp = new CMFCPropertyGridProperty(strName, (_variant_t)pBoolProp->GetValue(), strInfo);
	if (pNewProp == NULL)
		return;

	pNewProp->SetData((DWORD_PTR)pPropBase);
	pParentPropGroup->AddSubItem(pNewProp);
}

void CPropertyViewCtrl::AppendColorProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyColor *pColorProp)
{
	USES_CONVERSION;
	if (pParentPropGroup == NULL || pColorProp == NULL)
		return;

	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pColorProp);
	if (pPropBase == NULL)
		return;

	CString strName = A2W(pPropBase->GetObjectName());
	CString strInfo = A2W(pPropBase->GetObjectInfo());

	//CMFCPropertyGridProperty *pNewProp = new CMFCPropertyGridProperty(strName, (_variant_t)pColorProp->GetValue(), strInfo);
	//if (pNewProp == NULL)
	//	return;

	//pNewProp->SetData((DWORD_PTR)pPropBase);
	//pParentPropGroup->AddSubItem(pNewProp);
}

void CPropertyViewCtrl::ClearAll()
{
	m_pCurrentPropGroup = NULL;
	this->RemoveAll();
	this->RedrawWindow();
}

void CPropertyViewCtrl::AppendComboBoxProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyComboBox *pComboboxProp)
{
	USES_CONVERSION;
	if (pParentPropGroup == NULL || pComboboxProp == NULL)
		return;

	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pComboboxProp);
	if (pPropBase == NULL)
		return;

	CString strName = A2W(pPropBase->GetObjectName());
	CString strInfo = A2W(pPropBase->GetObjectInfo());

	COMBOBOX_PROP* pComboData = pComboboxProp->GetComboBoxData();
	if (pComboData == NULL)
		return;

	string strData = pComboData->DataVec[pComboData->nSelect];
	CMFCPropertyGridProperty* pNewProp = new CMFCPropertyGridProperty(strName, A2W(strData.c_str()), strInfo);
	for (STRING_VEC::iterator pVecItem = pComboData->DataVec.begin(); pVecItem != pComboData->DataVec.end(); pVecItem++)
	{
		strData = *pVecItem;
		pNewProp->AddOption(A2W(strData.c_str()));
	}
	pNewProp->AllowEdit(FALSE);

	pNewProp->SetData((DWORD_PTR)pPropBase);
	pParentPropGroup->AddSubItem(pNewProp);
}

void CPropertyViewCtrl::AppendCursorProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyCursor *pCursorProp)
{
	if (pParentPropGroup == NULL || pCursorProp == NULL)
		return;
}

void CPropertyViewCtrl::AppendFontProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyFont *pFontProp)
{
	if (pParentPropGroup == NULL || pFontProp == NULL)
		return;

}

void CPropertyViewCtrl::AppendImageProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyImage *pImageProp)
{
	USES_CONVERSION;
	if (pParentPropGroup == NULL || pImageProp == NULL)
		return;

	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pImageProp);
	if (pPropBase == NULL)
		return;

	CString strName = A2W(pPropBase->GetObjectName());
	CString strInfo = A2W(pPropBase->GetObjectInfo());
	CString strData = A2W(pImageProp->GetImageBaseName());

	CPropetryDialogGridProperty *pNewProp = new CPropetryDialogGridProperty(strName, (_variant_t)strData, strInfo);
	if (pNewProp == NULL)
		return;
	pNewProp->AllowEdit(FALSE);

	pNewProp->SetData((DWORD_PTR)pPropBase);
	pParentPropGroup->AddSubItem(pNewProp);
}

void CPropertyViewCtrl::AppendIntProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyInt *pIntProp)
{
	USES_CONVERSION;
	if (pParentPropGroup == NULL || pIntProp == NULL)
		return;

	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pIntProp);
	if (pPropBase == NULL)
		return;

	CString strName = A2W(pPropBase->GetObjectName());
	CString strInfo = A2W(pPropBase->GetObjectInfo());

	CMFCPropertyGridProperty *pNewProp = new CMFCPropertyGridProperty(strName, (_variant_t)pIntProp->GetValue(), strInfo);
	if (pNewProp == NULL)
		return;

	pNewProp->EnableSpinControl(TRUE, -(0x7FFFFFFF), 0x7FFFFFFF);
	pNewProp->SetData((DWORD_PTR)pPropBase);
	pParentPropGroup->AddSubItem(pNewProp);
}

void CPropertyViewCtrl::AppendStringProp(CMFCPropertyGridProperty* pParentPropGroup, IPropertyString *pStringProp)
{
	USES_CONVERSION;
	if (pParentPropGroup == NULL || pStringProp == NULL)
		return;

	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pStringProp);
	if (pPropBase == NULL)
		return;

	CString strName = A2W(pPropBase->GetObjectName());
	CString strInfo = A2W(pPropBase->GetObjectInfo());
	CString strData = A2W(pStringProp->GetString());

	CMFCPropertyGridProperty *pNewProp = new CMFCPropertyGridProperty(strName, (_variant_t)strData, strInfo);
	if (pNewProp == NULL)
		return;

	if ((lstrcmpA(pPropBase->GetObjectName(), NAME_SKIN_PROP_NAME_OBJ_ID) == 0)
	|| (lstrcmpA(pPropBase->GetObjectName(), NAME_SKIN_PROP_NAME_TYPE) == 0))
	{
		pNewProp->Enable(FALSE);
		pNewProp->AllowEdit(FALSE);
	}

	pNewProp->SetData((DWORD_PTR)pPropBase);
	pParentPropGroup->AddSubItem(pNewProp);
}

void CPropertyViewCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProperty)
{
	if (pProperty == NULL)
		return;

	IPropertyBase *pPropBase = (IPropertyBase*)pProperty->GetData();
	if (pPropBase == NULL)
		return;

	OBJECT_TYPE_ID ObjectTypeId = pPropBase->GetObjectTypeId();
	switch (ObjectTypeId)
	{
	case OTID_BOOL:
		RefreshBoolProp(pProperty, (IPropertyBool*)pPropBase);
		break;

	case OTID_COLOR:
		RefreshColorProp(pProperty, (IPropertyColor*)pPropBase);
		break;

	case OTID_COMBOBOX:
		RefreshComboBoxProp(pProperty, (IPropertyComboBox*)pPropBase);
		break;

	case OTID_CURSOR:
		RefreshCursorProp(pProperty, (IPropertyCursor*)pPropBase);
		break;

	case OTID_FONT:
		RefreshFontProp(pProperty, (IPropertyFont*)pPropBase);
		break;

	case OTID_IMAGE:
		RefreshImageProp(pProperty, (IPropertyImage*)pPropBase);
		break;

	case OTID_INT:
		RefreshIntProp(pProperty, (IPropertyInt*)pPropBase);
		break;

	case OTID_STRING:
		RefreshStringProp(pProperty, (IPropertyString*)pPropBase);
		break;

	default:
		return;
	}

	// ˢ�������б�
	if (m_pViewTree != NULL)
		m_pViewTree->Refresh(m_pCurrentPropGroup);

	// ˢ�½��� TBD

}

void CPropertyViewCtrl::RefreshBoolProp(CMFCPropertyGridProperty* pProperty, IPropertyBool *pBoolProp)
{
	if (pProperty == NULL || pBoolProp == NULL)
		return;

	COleVariant NewVariant = pProperty->GetValue();
	pBoolProp->SetValue(NewVariant.boolVal != VARIANT_FALSE);
}

void CPropertyViewCtrl::RefreshColorProp(CMFCPropertyGridProperty* pProperty, IPropertyColor *pColorProp)
{

}

void CPropertyViewCtrl::RefreshComboBoxProp(CMFCPropertyGridProperty* pProperty, IPropertyComboBox *pComboboxProp)
{

}

void CPropertyViewCtrl::RefreshCursorProp(CMFCPropertyGridProperty* pProperty, IPropertyCursor *pCursorProp)
{

}

void CPropertyViewCtrl::RefreshFontProp(CMFCPropertyGridProperty* pProperty, IPropertyFont *pFontProp)
{

}

void CPropertyViewCtrl::RefreshImageProp(CMFCPropertyGridProperty* pProperty, IPropertyImage *pImageProp)
{

}

void CPropertyViewCtrl::RefreshIntProp(CMFCPropertyGridProperty* pProperty, IPropertyInt *pIntProp)
{

}

void CPropertyViewCtrl::RefreshStringProp(CMFCPropertyGridProperty* pProperty, IPropertyString *pStringProp)
{
	USES_CONVERSION;
	if (pProperty == NULL || pStringProp == NULL)
		return;

	CString strName = pProperty->GetName();
	COleVariant NewVariant = pProperty->GetValue();
	if (NewVariant.bstrVal == NULL)
		return;

	CString strValue(NewVariant.bstrVal);
	::SysFreeString(NewVariant.bstrVal);

	if (strName.CompareNoCase(_T(NAME_SKIN_PROP_NAME)) == 0)
	{
		if (strValue.GetLength() <= 0)
		{
			string strOld = pStringProp->GetString();
			COleVariant OldVariant(A2W(strOld.c_str()));
			pProperty->SetValue(OldVariant);
			AfxMessageBox(_T("��Name���ֶβ�����Ϊ�գ�"), MB_OK | MB_ICONERROR);
			return;
		}

		if (m_pCurrentPropGroup != NULL)
			m_pCurrentPropGroup->SetObjectName(W2A(strValue));
	}

	pStringProp->SetString(W2A(strValue));
}