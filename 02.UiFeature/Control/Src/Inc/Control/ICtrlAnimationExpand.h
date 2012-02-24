
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_ANIMATION_EXPAND					("AnimationExpand")

class ICtrlAnimationExpand : public ICtrlInterface
{
public:
	ICtrlAnimationExpand();
	virtual ~ICtrlAnimationExpand();

	// 开始动画
	void BeginAnimation();
	// 取得用于绘制背景的图片DC
	CDrawingBoard* GetAnimationBackgroudMemDc();

protected:
	// 派生控件用于创建属于自己的控件属性
	// bNeedSetDftProp：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bNeedSetDftProp);
	// 初始化控件
	virtual void OnCreate();
	// 控件初始化完毕
	virtual void OnFinalCreate();
	// 销毁控件
	virtual void OnDestroy();
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard);

private:
	// 背景图的内存DC
	CDrawingBoard m_BkImageDc;
	// 容器名称
	IPropertyString* m_pPropPanelName;
	// 是否在动画过程中显示容器中的内容
	IPropertyBool* m_pPropIsShowPanel;
	IPropertyGroup* m_pPropSjGroup;
	// 三角形箭头：上
	IPropertyImage* m_pPropUpSjImg;
	// 三角形箭头Mark：上
	IPropertyImage* m_pPropUpSjMarkImg;
	// 三角形箭头：下
	IPropertyImage* m_pPropDownSjImg;
	// 三角形箭头Mark：下
	IPropertyImage* m_pPropDownSjMarkImg;
	// 展开的边框
	IPropertyImage* m_pPropFrameImg;
	// 展开的内容
	IPropertyImage* m_pPropFrameMidImg;
};
