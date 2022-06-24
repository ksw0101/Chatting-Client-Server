#include "Window.h"
RECT g_rtClient;
HWND g_hwnd;
Window* g_pWindow = nullptr;

LRESULT  CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    g_pWindow -> MsgProc(hwnd, msg, wparam, lparam);
    switch (msg)
    {
    case WM_DESTROY: 
    {
        PostQuitMessage(0);
    }break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}

LRESULT  Window::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return 0;
}
BOOL Window::SetWinClass(HINSTANCE hInstance)
{
    m_hInsatance = hInstance;
    WNDCLASS wc;

    ZeroMemory(&wc, sizeof(WNDCLASS));

    wc.lpfnWndProc = WndProc;

    wc.hInstance = hInstance;

    wc.lpszClassName = L"LJS_WINDOW";

    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (RegisterClass(&wc) == false)
    {
        return FALSE;
    }
    return TRUE;
}
BOOL Window::SetWindow(const WCHAR* szTitle, int iWidth, int iHeight)
{
    RECT rt = { 0,0, iWidth , iHeight };

    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindow(
        L"LJS_WINDOW",
        szTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rt.right - rt.left,
        rt.bottom - rt.top,
        NULL,
        NULL,
        m_hInsatance,
        NULL);
    if (m_hWnd == NULL)
    {
        return FALSE;
    }
    g_hwnd = m_hWnd;
    GetClientRect(m_hWnd, &m_rtClient);

    GetWindowRect(m_hWnd, &m_rtWindow);

    g_rtClient = m_rtClient;

    ShowWindow(m_hWnd, SW_SHOW);
    return TRUE;
}
bool Window::WinRun()
{
    MSG msg;

    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg); 
    }

    return true;
}
Window:: Window()
{
    g_pWindow = this;
}