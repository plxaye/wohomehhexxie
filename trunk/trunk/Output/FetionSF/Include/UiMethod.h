
#pragma once
#include "FetionSFDefs.h"

class CUiMethod
{
public:
	CUiMethod();
	virtual ~CUiMethod();

	// GDI+ 的坐标类型和windows常用坐标类型转换
	static Rect CRect2Rect(CRect &InRect);
	static RectF CRect2RectF(CRect &InRect);
	static CRect RectF2CRect(RectF &Rf);
	static PointF CPoint2PointF(CPoint &point);

	// 设置安全颜色值 0 <= nColor <= 255
	static void SafeColor(int &nColor);

	// 屏幕坐标转换
	static void ClientToScreen(HWND hWnd, CRect &ClientRect);
	static void ScreenToClient(HWND hWnd, CRect &ScreenRect);

	// 创建圆角矩形区域
	static GraphicsPath* MakeRoundRect(Point topLeft, Point bottomRight, INT percentageRounded);

	// 屏幕指定区域截图
	static HBITMAP WINAPI GetScreenBitmap(CRect &GetRect);

	// 取得图片类型
	static IMAGE_TYPE GetImageTypeFromLocal(CString strPath);
private:

};

