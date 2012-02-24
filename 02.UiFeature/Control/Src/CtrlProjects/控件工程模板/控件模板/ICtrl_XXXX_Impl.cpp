
#include "StdAfx.h"
#include "ICtrl_XXXX_Impl.h"
#include "..\..\Inc\IControlManager.h"

ICtrl_XXXX_Impl::ICtrl_XXXX_Impl(void)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_BASE_PANEL);

	m_pPropImage = NULL;
}

ICtrl_XXXX_Impl::~ICtrl_XXXX_Impl(void)
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrl_XXXX_Impl::CreateControlPropetry(bool bNeedSetDftProp)
{
	m_pPropImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "Image", "背景图片");
	IPropertyGroup* pPropGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "BkGroup", "背景Group");
	CreatePropetry(pPropGroup, OTID_CURSOR, "BkCursor", "背景Cursor");
//	CreatePropetry(pPropGroup, OTID_FONT, "BkFont", "背景font");
//	CreatePropetry(pPropGroup, OTID_FONT, "BkFont1", "背景font1");
	return true;
}

// 初始化控件
void ICtrl_XXXX_Impl::OnCreate()
{
}

// 控件初始化完毕
void ICtrl_XXXX_Impl::OnFinalCreate()
{
}

// 销毁控件
void ICtrl_XXXX_Impl::OnDestroy()
{
}

// 绘制控件
void ICtrl_XXXX_Impl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (m_pPropImage != NULL)
		m_pPropImage->DrawImage(DrawBoard, this->GetClientRect());
}
