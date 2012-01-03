
#include "StdAfx.h"
#include "..\..\Inc\ICtrlBasePanel.h"
#include "..\..\Inc\IControlManager.h"

ICtrlBasePanel::ICtrlBasePanel(void)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_BASE_PANEL);
}

ICtrlBasePanel::~ICtrlBasePanel(void)
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlBasePanel::CreateControlPropetry()
{
	return true;
}

// 初始化控件
void ICtrlBasePanel::OnCreate()
{
}

// 控件初始化完毕
void ICtrlBasePanel::OnFinalCreate()
{
}

// 销毁控件
void ICtrlBasePanel::OnDestroy()
{
}

// 绘制控件
void ICtrlBasePanel::OnPaint()
{

}
