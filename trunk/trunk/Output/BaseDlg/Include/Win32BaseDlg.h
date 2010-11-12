
#pragma once
#include "GlobalDef.h"
#include "WindowBase.h"

#pragma warning(disable:4800)


//////// Win32BaseDlg �������� //////////////////////////////////////////////////////////////
// �Ƿ�����С����ť
#define BFS_HAVE_MIN_BTN								(0x00000001)
// �Ƿ�����󻯰�ť
#define BFS_HAVE_MAX_BTN								(0x00000002)
// �Ƿ�Ϊ���
#define BFS_MAX_WND										(0x00000004)
// ����Ƿ��һ�ν��봰��
#define BFS_FIRST_IN_WND								(0x00000008)
// �Ƿ�֧������߿����
#define BFS_CAN_DRAW									(0x00000010)
// ģʽ�Ի���
#define BFS_MODAL_DLG									(0x00000020)
//////////////////////////////////////////////////////////////////////////////////////////////

// ��Ϣ�������໯
LRESULT CALLBACK PuppetWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class CWin32BaseDlg : public CWindowBase
{
public:
	CWin32BaseDlg(HINSTANCE hInstance, HWND hParentWnd, int nIconId);
	~CWin32BaseDlg(void);

	// ��Ϣ����
	virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

	// ģʽ�Ի������ʵ��ʽ
	int DoModal();

	// ��ģʽ��ʾ�Ի���
	void OpenDialog();

	// �Ի����������ú��ж�
	bool IsSetBfStyle(DWORD dwStyle) { return (m_dwBfStyle & dwStyle); };
	void AddBfStyle(DWORD dwStyle) { m_dwBfStyle |= dwStyle; };
	void DeleteBfStyle(DWORD dwStyle) { m_dwBfStyle &= (~dwStyle); };

	// ���ö��ᴰ��״̬������״̬�£�����Ӧ����ƶ���Ϣ��
	void SetFreezeDlg(bool bIsFreeze) { m_bIsFreeze = bIsFreeze; };

	// ���ô��ڱ�������
	void SetWindowText(CString strText) { m_strWindowText = strText; };

protected:
	virtual void OnPaint(HDC hPaintDc) {};
	virtual void OnDestroy() {};
	virtual void OnCreate() {};
	virtual LRESULT OnNcActive(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNcCalcSize(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnEraseBkgnd(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNcPaint(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNcHitTest(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnActivateApp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSizeProc(WPARAM wParam, LPARAM lParam);

protected:
	bool CreateDlg();
	ATOM RegisterBfoClass();
	bool InitInstance();
	void EndThisDialog();

protected:
	// �����ھ��
	HWND m_hParent;
	// ��������
	DWORD m_dwBfStyle;
	// �û�ǿ���˳�
	bool m_bCoerceEnd;
	// ����ͼ��ID
	int m_nIconId;
	// ���ڱ�������
	CString m_strWindowText;
	// �Ƿ񶳽ᴰ��
	bool m_bIsFreeze;
	// ��������
	static CString m_strWindowClass;
};