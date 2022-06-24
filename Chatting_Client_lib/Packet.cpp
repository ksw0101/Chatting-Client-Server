#include "Packet.h"

void Packet:: PutData(const char* data, int size)
{
	m_upacket.p_header.len += size;
	memcpy(m_offset, data, size);
	m_offset += size;
}
void Packet:: GetData(const char* data, int size)
{
	memcpy(const_cast<char*>(data), m_offset, size);
	m_offset += size;
}
void Packet:: Reset()
{
	m_offset = m_upacket.msg;
}
Packet& Packet:: operator << (int data)
{
	PutData((char *)&data, sizeof(int));
	return *this;
}
Packet& Packet:: operator << (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
Packet& Packet:: operator << (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
Packet& Packet:: operator << (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
Packet& Packet:: operator << (char* data)
{
	PutData(data, strlen(data)+1);
	return *this;
}
Packet& Packet:: operator << (string data)
{
	PutData(data.c_str(), data.size()+1);
	return *this;
}
Packet& Packet:: operator >> (int data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
Packet& Packet:: operator >> (long data)
{
	GetData((char*)&data, sizeof(long));
	return *this;
}
Packet& Packet::Packet:: operator >> (short data)
{
	GetData((char*)&data, sizeof(short));
	return *this;
}
Packet& Packet:: operator >> (float data)
{
	GetData((char*)&data, sizeof(float));
	return *this;
}
Packet& Packet:: operator >> (char* data)
{
	GetData(data, strlen(m_offset)+1);
	return *this;
}
Packet& Packet:: operator >> (string data)
{
	GetData(data.c_str(), strlen(m_offset) + 1);
	return *this;
}
Packet::Packet()
{
	ZeroMemory(&m_upacket, sizeof(UPACKET));
	m_upacket.p_header.len = PACKET_HEADER_SIZE;
	m_offset = m_upacket.msg;
}
Packet::Packet(WORD type)
{
	ZeroMemory(&m_upacket, sizeof(UPACKET));
	m_upacket.p_header.len = PACKET_HEADER_SIZE;
	m_offset = m_upacket.msg;
	m_upacket.p_header.type = type;
}
Packet:: ~Packet()
{

}