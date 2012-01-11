//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"
#include "CDrawingBoard.h"
#include "CDrawingImage.h"

// 九宫格数据
struct JGG_INFO
{
	// 九宫格边线距离
	int nLeftSpace;
	int nRightSpace;
	int nTopSpace;
	int nBottomSpace;
	// 九宫格是否拉伸，不拉伸则平铺
	bool bLeftStretch;
	bool bRightStretch;
	bool bTopStretch;
	bool bBottomStretch;
	bool bMiddleStretch;
};

enum IMAGE_SHOW_TYPE
{
	// 平铺
	IST_PINGPU		= 0,
	// 全拉伸
	IST_ALL_LASHEN,
	// 九宫格拉伸
	IST_JGG_LASHEN
};

// 图片属性
struct IMAGE_BASE_PROP
{
	// 在压缩文件中的文件名
	bool bIsZipFile;
	string strFileName;
	RECT RectInImage;
	IMAGE_SHOW_TYPE ImgShowType;
	JGG_INFO jggInfo;
};

class IPropertyImageBase : public IPropertyBase
{
	friend class IPropertyImage;
public:
	IPropertyImageBase();
	virtual ~IPropertyImageBase();

	virtual void SetActivePropetry(bool bIsActive);
	virtual bool GetActivePropetry();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(XmlNode* pXmlNode);

//////////////////////////////////////////////////////////////////////////
	// Builder使用函数，即：编辑状态下使用函数

//////////////////////////////////////////////////////////////////////////
	// 运行时态使用函数
	void SetImageProp(IMAGE_BASE_PROP* pImgProp);
	IMAGE_BASE_PROP* GetImageProp();

private:
	bool DrawImage(CDrawingBoard &DstDc, RECT DstRct);

private:
	IMAGE_BASE_PROP m_ImageProp;
	CDrawingImage m_DrawImg;
	// 被使用的计数器
	int m_nUseCtns;
};
