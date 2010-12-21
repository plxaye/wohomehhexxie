
#pragma once

#include "MmxBase.h"


class CMmxRender : public CMmxBase
{
public:
	CMmxRender(void);
	~CMmxRender(void);

	// 使用32位带透明值颜色值填充一段位图数据
	virtual void RGBA32_FillBitmapBuffer(DWORD *pBmpData, CSize BmpSize, BYTE byA, BYTE byR, BYTE byG, BYTE byB);
	// 32位带透明值色块花刷
//	virtual void SolidBrush32ARGB(DWORD *pImgData, CSize ImgSize, CRect DrawRect, DWORD dwColor);
};
