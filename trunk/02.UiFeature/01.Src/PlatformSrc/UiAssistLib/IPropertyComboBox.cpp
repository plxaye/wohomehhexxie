
#include "stdafx.h"
#include "..\..\Inc\IPropertyComboBox.h"
#include "..\..\Inc\UiFeatureDefs.h"

IPropertyComboBox::IPropertyComboBox()
{
	SetObjectType(PROP_TYPE_COMBOBOX_NAME);
	Clear();
}

IPropertyComboBox::~IPropertyComboBox()
{
	Clear();
}

void IPropertyComboBox::Clear()
{
	m_ComboBoxPro.nSelect = -1;
	m_ComboBoxPro.DataVec.clear();
}

bool IPropertyComboBox::IsRightData()
{
	return (m_ComboBoxPro.DataVec.size() > 0 && m_ComboBoxPro.nSelect >= 0);
}

COMBOBOX_PROP* IPropertyComboBox::GetComboBoxData()
{
	return &m_ComboBoxPro;
}

bool IPropertyComboBox::ReadResourceXmlProperty(XmlNode* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	char* psz_id = JabberXmlGetAttrValue(pXmlNode, SKIN_OBJECT_ID);
	char* psz_name = JabberXmlGetAttrValue(pXmlNode, "name");
	char* psz_counts = JabberXmlGetAttrValue(pXmlNode, "counts");
	char* psz_select = JabberXmlGetAttrValue(pXmlNode, "select");
	if (psz_id == NULL || psz_name == NULL || psz_counts == NULL || psz_select == NULL)
		return false;

	int nCtns = atoi(psz_counts);

	SetObjectId((const char *)psz_id);
	SetObjectName((const char *)psz_name);
	m_ComboBoxPro.nSelect = atoi(psz_select);
	if (m_ComboBoxPro.nSelect < 0 || m_ComboBoxPro.nSelect >= nCtns)
		m_ComboBoxPro.nSelect = 0;

	for (int i = 0; i < nCtns; i++)
	{
		char pszDataName[MAX_PATH + 1];
		memset(pszDataName, 0, MAX_PATH + 1);
		sprintf_s(pszDataName, MAX_PATH, "data%d", i);

		char* psz_data = JabberXmlGetAttrValue(pXmlNode, pszDataName);
		if (psz_data == NULL)
		{
			Clear();
			return false;
		}

		string strData = psz_data;
		m_ComboBoxPro.DataVec.push_back(strData);
	}

	return true;
}