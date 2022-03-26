#pragma once
#include "../Client/Server.h"


struct EXOVER
{
	WSAOVERLAPPED	over;
	char			io_buf[MAX_BUFFER];
	union {
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
	int id;
};




class CNetwork 
{
	OVERLAPPED		_overlapped;



public:
	CNetwork();
	~CNetwork();
public:
	void Connect();

public:
	void Receive();
	void ProcessPacket(char* ptr);
	void Process_Data(char* net_buf, size_t& io_byte);
	void Enter_Player(const int& id);

public:
	void Send_Packet(void* packet);
	void Send_LogIn_Packet();
	void Send_Move_Packet(unsigned const char& dir);


public:
	void err_quit(const char* msg);

};


