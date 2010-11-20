
#pragma once

#include "AggRenderDefs.h"

class CAggDraw2DTo3D
{
public:
	CAggDraw2DTo3D();
	virtual ~CAggDraw2DTo3D();

	// 将2D的图以3D形式展现
	void Draw2DTo3D(HDC hMemoryDC, HBITMAP hMemoryBitmap, unsigned char* pMemoryBitmapBits, CRect &PicRect,
		CPoint &ptLeftUp, CPoint &ptRightUp, CPoint &ptLeftDown, CPoint &ptRightDown);

private:
	bool CAggDraw2DTo3D::load_pmap(const char* fn, unsigned idx, rendering_buffer* dst)
};
