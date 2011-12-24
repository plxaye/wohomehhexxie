
#include "stdafx.h"
#include "..\..\Inc\IPropertyGroup.h"
#include "..\..\Inc\UiFeatureDefs.h"

IPropertyGroup::IPropertyGroup()
{
	SetObjectType("group");
	m_GroupProp.strName = "";
	m_GroupProp.strPropInfo = "";
	m_PropVec.clear();
}

IPropertyGroup::~IPropertyGroup()
{

}

bool IPropertyGroup::IsRightData()
{
	return (m_GroupProp.strName.size() > 0);
}

PROP_TYPE IPropertyGroup::GetPropType()
{
	return PT_GROUP;
}

// 取得Group下的属性列表
GROUP_PROP_VEC* IPropertyGroup::GetPropVec()
{
	return &m_PropVec;
}

bool IPropertyGroup::ReadResourceXmlProperty(XmlNode* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	char* psz_id = JabberXmlGetAttrValue(pXmlNode, SKIN_OBJECT_ID);
	char* psz_name = JabberXmlGetAttrValue(pXmlNode, "name");
	if (psz_id == NULL || psz_name == NULL)
		return false;

	SetObjectId((const char *)psz_id);
	m_GroupProp.strName = psz_name;

	return true;
}

// 追加一个属性
void IPropertyGroup::AppendProperty(IPropertyBase* pProperty)
{
	if (pProperty == NULL)
		return;

	m_PropVec.push_back(pProperty);
}
