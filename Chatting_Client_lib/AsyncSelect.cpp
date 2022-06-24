#include "AsyncSelect.h"
bool AsyncSelect:: Connect(HWND hwnd, int protocol, int port, const char* ip)
{
	m_sock = socket(AF_INET, protocol, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);
	m_user.m_sock = m_sock;


	if (WSAAsyncSelect(m_sock, hwnd,NETWORK_MSG,FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		return false;
	}
	int ret = WSAConnect(m_sock, (sockaddr*)&sa,sizeof(sa),NULL, NULL, NULL, NULL);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
LRESULT AsyncSelect:: MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case NETWORK_MSG:
	{
		WORD ret = WSAGETSELECTEVENT(lparam);
		switch (ret)
		{
		case FD_CONNECT:
		{
			m_connect = true;
		}break;
		case FD_READ:
		{
			RecvUser(m_user);
		}break;
		case FD_WRITE:
		{
			m_connect = false;
		}break;
		case FD_CLOSE:
		{
			m_connect = false;
		}break;
		}
	}break;
	}
	return 0;
}