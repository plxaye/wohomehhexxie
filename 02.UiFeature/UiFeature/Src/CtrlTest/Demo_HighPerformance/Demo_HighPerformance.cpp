// UiFeatureRun.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32TestWindow.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwMainThreadId = ::GetCurrentThreadId();

	CWin32TestWindow RunWindow;
	if (!RunWindow.ShowWin32TestWindow(dwMainThreadId))
	{
		MessageBox(NULL, _T("�������Դ��ڴ���"), _T("Ԥ������"), MB_OK | MB_ICONERROR);
		return -1;
	}

	//CWin32TestWindow RunWindow1;
	//if (!RunWindow1.ShowWin32TestWindow(dwMainThreadId))
	//{
	//	MessageBox(NULL, _T("�������Դ��ڴ���"), _T("Ԥ������"), MB_OK | MB_ICONERROR);
	//	return -1;
	//}

	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		if (!::TranslateAccelerator(msg.hwnd, NULL, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	RunWindow.UnInitialized();

	return 0;
}
