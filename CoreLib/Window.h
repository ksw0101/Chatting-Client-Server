#pragma once
#include "std.h"
class Window
{
public:
	
	HINSTANCE m_hInsatance;
	HWND m_hWnd;
	RECT m_rtClient;
	RECT m_rtWindow;

	BOOL  SetWinClass(HINSTANCE hInstance);
	BOOL  SetWindow(const WCHAR* szTitle = L"Chatting", int iWidth = 500, int iHeight = 700);
	virtual LRESULT  MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool  WinRun();

	Window();
	virtual ~Window() {}
};

