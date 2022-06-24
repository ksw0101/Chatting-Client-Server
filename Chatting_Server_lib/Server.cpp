#include "Server.h"
bool Server:: InitServer(int port)
{
	//InitializeCriticalSection(&m_cs);
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	//���� ����
	m_socket = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	//���Ͽ� �ּ��Ҵ�
	int ret = bind(m_socket,(sockaddr*)&sa,sizeof(sa));
	if (ret == SOCKET_ERROR)return false;
	//���� ��û�� ����ϴ� �Լ�
	ret = listen(m_socket, SOMAXCONN);
	if (ret == SOCKET_ERROR)return false;
	cout << "���� ������!" << endl;
	cout << m_userlist.size() << "�� �������Դϴ�." << endl;

	//non-blocking ����
	u_long on = 1;
	ioctlsocket(m_socket, FIONBIO, &on);

	//m_server = this;

	return true;
}
bool Server:: Run()
{
	return  true;
}
bool Server:: Release()
{
	closesocket(m_socket);
	WSACleanup();
	//DeleteCriticalSection(&m_cs);
	return true;
}
bool Server:: AddUser(SOCKET sock, SOCKADDR_IN clientaddr)
{
	return true;
}
//�� ������ �Է��� �޼����� �ٸ� �������� �� �����ϴ� �Լ�
bool Server::Broadcast(User* user)
{
	//������ ��ŶǮ�� ����� 0���� ũ�� ������ ��Ŷ�� �ִٴ� �Ҹ�
	if (user->m_packetPool.size() > 0)
	{
		//iterator - �ݺ���
		list<Packet>::iterator iter;
		//��Ŷ�� �������� �ɰ��� ���۵Ǵ� �� �ɰ��� ��� ��Ŷ ��ŶǮ�� �ִµ� �װ� ÷���� ������ �ȱ�
		for (iter = user->m_packetPool.begin(); iter != user->m_packetPool.end();)
		{
			list<User*>::iterator suser;
			//��Ŷ1 ��� �������� ������ ->��Ŷ2 ��� �������� ������ -> ~ -> ������ ��Ŷ����
			for (suser = m_userlist.begin(); suser != m_userlist.end();)
			{
				User* senduser = (User*)*suser;
				Message m;
				int i = m.SendMsg(senduser->m_sock, (*iter).m_upacket);
				//������ 0�̸� ���� ������ �ȵǾ� �ִٴ°�
				if (i <= 0)
				{
					senduser->m_connect = false;
					suser = m_userlist.erase(suser);
				}
				suser++;
			}
			//���� ��Ŷ�� ������
			iter = user->m_packetPool.erase(iter);
		}
	}
	return true;
}
Server:: Server()
{
	
}
Server:: ~Server()
{

}