#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"

LRESULT  Sample::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
		case 200:
		{
			char buffer[MAX_PATH] = { 0, };
			SendMessageA(m_edit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
			Packet packet(PACKET_CHAT_MSG);
			packet << "aaa" << "����"<< buffer;
			m_msg.SendMsg(m_net.m_sock, packet.m_upacket);

			SendMessageA(m_edit, WM_SETTEXT, 0, (LPARAM)"");
		}break;
		}
	}break;
	}
	return m_net.MsgProc(hwnd, msg, wparam, lparam);
}
bool Sample::Init()
{
	DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
	m_edit		= CreateWindow(L"edit", NULL, style, 0, g_rtClient.bottom - 50, 450, 50, m_hWnd, (HMENU)100, m_hInsatance, NULL);
	style		= WS_CHILD | WS_VISIBLE;
	m_button	= CreateWindow(L"button", L"����", style, 450, g_rtClient.bottom - 50, 50, 50, m_hWnd, (HMENU)200, m_hInsatance, NULL);
	m_listbox	= CreateWindow(L"listbox", NULL, style, 0, 0, 500, g_rtClient.bottom - 50, m_hWnd, (HMENU)300, m_hInsatance, NULL);

	SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)"ä�ü����� ����Ǿ����ϴ�");

	m_net.Initnetwork();
	m_net.Connect(g_hwnd, SOCK_STREAM, 10000, "127.0.0.1");
	return true;
}
bool Sample::Frame()
{
	int count = m_net.m_user.m_packetPool.size();
	if (count > 0 && m_chatcount != count)
	{
		m_chatcount = count;

		SendMessage(m_listbox, LB_RESETCONTENT, 0, 0);
		list<Packet> ::iterator iter;
		if (m_net.m_user.m_packetPool.size() > 20)
		{
			m_net.m_user.m_packetPool.pop_front();
		}
		for (iter = m_net.m_user.m_packetPool.begin(); iter != m_net.m_user.m_packetPool.end(); iter++)
		{
			ChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			(*iter) >> recvdata.index >> recvdata.name >> recvdata.message;
			SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)recvdata.name);
			SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)recvdata.message);

			(*iter).Reset();
		}
	}
	return true;
}
bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{
	m_net.Closenetwork();
	return true;
}
Sample::Sample()
{

}
Sample:: ~Sample()
{

}
RUN();