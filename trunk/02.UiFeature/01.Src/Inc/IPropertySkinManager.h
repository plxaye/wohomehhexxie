//////////////////////////////////////////////////////////////////////////
// 当前皮肤所有窗口中所有控件的属性管理类

#pragma once
#include "IPropertyWindow.h"
#include "IPropertyBase.h"

class IPropertySkinManager
{
public:
	// 初始化Window皮肤
	virtual IPropertyWindow* InitWindowSkin(const char *pszSkinPath, const char *pszWndName) = 0;
	// 查找指定的属性
	virtual IPropertyBase* FindProperty(PROP_TYPE propType, int nPropId) = 0;
	// 设置显示的语言种类
	virtual void SetArea(AREA_TYPE areaType) = 0;
	// 创建一个属性，并将次属性放入队列
	virtual IPropertyBase* CreateProperty(PROP_TYPE propType, int nPropId) = 0;
};
