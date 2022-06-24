#include "Message.h"
int Message::ReadMsg(char* recvbuffer, int recvbyte)
{
	if (m_writePos + recvbyte >= 2048)
	{
		if (m_readPos > 0)
		{
			memmove(&m_recvBuffer[0], &m_recvBuffer[m_packetPos], m_readPos);
		}
		m_packetPos = 0;
		m_writePos = m_readPos;
	}
	memcpy(&m_recvBuffer[m_writePos], recvbuffer, recvbyte);
	m_writePos += recvbyte;
	m_readPos += recvbyte;

	if (m_readPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* upacket = (UPACKET*)&m_recvBuffer[m_packetPos];
		while (upacket->p_header.len <= m_readPos)
		{
			Packet packet(upacket->p_header.type);
			memcpy(&packet.m_upacket, &m_recvBuffer[m_packetPos], upacket->p_header.len);
			m_packetPool.push_back(packet);

			m_packetPos += upacket->p_header.len;
			m_readPos -= upacket->p_header.len;
			if (m_readPos < PACKET_HEADER_SIZE)
			{
				break;
			}
			upacket = (UPACKET*)m_recvBuffer[m_packetPos];
		}
	}
	return 1;
}
int Message::SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.p_header.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.p_header.type = type;
	memcpy(packet.msg, msg, strlen(msg));

	char* pmsg = (char*)&packet;
	int sendsize = 0;
	do
	{
		int sendbyte = send(sock, &pmsg[sendsize], packet.p_header.len - sendsize, 0);
		if (sendbyte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		sendsize += sendbyte;
	} while (sendsize < packet.p_header.len);
	return sendsize;
}
int Message::SendMsg(SOCKET sock, UPACKET& upacket)
{
	char* msg = (char*)&upacket;
	int sendsize = 0;
	do {
		int iSendByte = send(sock, &msg[sendsize],
			upacket.p_header.len - sendsize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		sendsize += iSendByte;
	} while (sendsize < upacket.p_header.len);
	return sendsize;
}