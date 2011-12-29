
#include "StdAfx.h"
#include "..\..\Inc\IPropertyWindow.h"

IPropertyWindow::IPropertyWindow(void)
{
	SetObjectType(PROP_TYPE_WINDOW_NAME);
	m_ChildCtrlPropVec.clear();
	m_pWindowProp = NULL;
}

IPropertyWindow::~IPropertyWindow(void)
{
}

CHILD_CTRL_PROP_VEC* IPropertyWindow::GetChildControlVec()
{
	return &m_ChildCtrlPropVec;
}

void IPropertyWindow::SetWindowPropGroup(IPropertyGroup *pWindowProp)
{
	if (pWindowProp == NULL)
		return;

	m_pWindowProp = pWindowProp;
}

IPropertyGroup* IPropertyWindow::GSetWindowPropGroup()
{
	return m_pWindowProp;
}
