
#pragma once
#include "Fetion.h"

typedef std::vector<CResImage*> RES_IMAGE_LIST;


class CRgnDlg : public CHighEfficiencyDlg
{
public:
	CRgnDlg(HINSTANCE hInstance, HWND hParentWnd, int nIconId);
	~CRgnDlg(void);

protected:
	virtual void OnCreate();
	virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam);

	BLENDFUNCTION m_Blend;//�ṹ��BLENDFUNCTION����ָ������DC(��ͼ�豸)���ںϷ�ʽ��

	virtual void OnPaint(HDC hPaintDc);
	virtual LRESULT OnSize(HDWP hWinPoslnfo, WPARAM wParam, LPARAM lParam);

	// ������̧����Ϣ
	virtual void DUI_OnLButtonUp(WPARAM wParam, LPARAM lParam);

protected:
	double m_dbFactor;
	int m_nTimerId;

	POINT_VET m_PointList;

	CResImage m_TImage;

private:
	CBfNormalGraduatedButton *m_pZhuanBtn;
	CBfGifStatic *m_pGifStatic;
	CBfPictureButton *m_pMinButton;
	CBfPictureButton *m_pCloseButton;
	CBaseCtrlManager m_UiManager;
};