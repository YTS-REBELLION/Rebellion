#include "stdafx.h"
#include "Network.h"
#include "GameObject.h"
#include "Scene.h"

#include "Layer.h"
#include "Camera.h"

#include "Transform.h"
#include "MeshRender.h"
#include "Collider.h"     

#include "PlayerScript.h"
#include "ToolCamScript.h"
#include "MonsterScript.h"



OBJECT_TYPE CheckType(const short& id)
{
	if (id >= 0 && id < MAX_USER) return OBJECT_TYPE::CLIENT;
	else if (id >= NPC_ID_START && NPC_ID_START + 100) return OBJECT_TYPE::MONSTER;
}


CNetwork g_net;
const char* SERVER_IP = "127.0.0.1";

SOCKET g_socket;
int packetTest;
int g_myid = -1;


void CNetwork::err_quit(const char* msg)
{
	LPVOID lpmsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpmsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgBuf);
	exit(1);
}


CNetwork::CNetwork()
{
	ZeroMemory(&_overlapped, sizeof(_overlapped));

}

CNetwork::~CNetwork()
{
}

void CNetwork::Connect()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return;

	g_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ULONG l = 1;

	int ret = ioctlsocket(g_socket, FIONBIO, (unsigned long*)&l);

	if (g_socket == INVALID_SOCKET)
		err_quit("WSASocket");
	SOCKADDR_IN recvAddr;
	ZeroMemory(&recvAddr, 0, sizeof(recvAddr));

	recvAddr.sin_family = AF_INET;
	//recvAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	inet_pton(AF_INET, SERVER_IP, &(recvAddr.sin_addr));
	
	recvAddr.sin_port = htons(SERVER_PORT);

	if (connect(g_socket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			int i = 0;
		}
		else
		{
			int i = 0;
		}
	}

	std::cout << "connect" << std::endl;

	//if (ret == SOCKET_ERROR)
	//	err_quit("connect error");

	WSAEVENT event = WSACreateEvent();
	ZeroMemory(&_overlapped, sizeof(_overlapped));


	_overlapped.hEvent = event;


}

void CNetwork::Receive()
{
	
	EXOVER*	 dataBuf = new EXOVER{};
	DWORD	 recvByte = 0;
	DWORD	 flags = 0;
	dataBuf->over = _overlapped;
	char recvbuf[100] = "";
	int fg = 0;

	int ret = recv(g_socket, recvbuf, sizeof(recvbuf), fg);
	if (ret <= 0)
		return;

	size_t retbytesize = ret;


	if (WSAGetLastError() == WSAEWOULDBLOCK)
	{
		//pass
	}
	
	Process_Data(recvbuf, retbytesize);
	


}

void CNetwork::ProcessPacket(char* ptr)
{

	std::cout << "Process Packet" << std::endl;
	switch (ptr[1]) {
	//case 로그인 패킷:
	
	case SC_PACKET_LOGIN_OK:
	{
		
		sc_packet_login_ok* p = reinterpret_cast<sc_packet_login_ok*>(ptr);
		std::cout << "플레이어 아이디 : "<<p->id << std::endl;
		std::cout << "플레이어 데미지 : "<<p->damage << std::endl;
		std::cout << "플레이어 체력 "<<p->c_hp<< " / " <<p->m_hp << std::endl;
		std::cout << "플레이어 레벨 : "<<p->level << std::endl;
		std::cout << "플레이어 경험치 : " << p->c_exp << " / " << p->m_exp << std::endl;
		

	}
	break;

	case SC_PACKET_LOGIN_FAIL: {
		std::cout << "로그인 페일" << std::endl;

		exit(0);
		break;
	}
	case SC_PACKET_PUT_OBJECT: {
		sc_packet_put_object* packet = reinterpret_cast<sc_packet_put_object*>(ptr);
		int id = packet->id;

		if (id == g_myid) {
			//내꺼 만들기
		}
		else {
			if (CheckType(id) == OBJECT_TYPE::CLIENT) {
				// 다른 사람꺼
			}
			else if (CheckType(id) == OBJECT_TYPE::MONSTER) {
				// 몬스터

			}
		}

	}
							 break;
	//case 입장 패킷:
	case SC_PACKET_LEAVE_OBJECT: {

	}
	}

}

void CNetwork::Process_Data(char* net_buf, size_t& io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[MAX_BUFFER];


	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;


		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}


}

void CNetwork::Send_Packet(void* _packet)
{
	EXOVER dataBuf;
	char* packet = reinterpret_cast<char*>(_packet);
	size_t sent;
	DWORD flag;
	dataBuf.wsabuf.len = packet[0];
	dataBuf.wsabuf.buf = (char*)packet;
	dataBuf.over = _overlapped;

	packetTest = dataBuf.wsabuf.len;

	std::cout << "Packet Send : " << packetTest << std::endl;


	//if (WSASend(g_Socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) == SOCKET_ERROR)
	if (WSASend(g_socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			WSAWaitForMultipleEvents(1, &_overlapped.hEvent, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(g_socket, &_overlapped, (LPDWORD)&sent, FALSE, &flag);
		}
		else
			err_quit("WSASend");
	}

}

void CNetwork::Send_LogIn_Packet()
{

	cs_packet_login packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_LOGIN;
	
	char name[MAX_ID_LEN] = "Tester";
	string namestring;
	
	//cout << name << endl;
	//strcpy_s(name, packet.name);



	std::cout << "name : ";
	std::cin >> name;



	sprintf_s(packet.name, name);
	strcpy_s(name, packet.name);
	g_myid++;

	std::cout << "CS_LOGIN PACKET : " << g_myid << std::endl;

	Send_Packet(&packet);


}
void CNetwork::Send_Move_Packet(unsigned const char& dir, const Vec3& localPos, const Vec3& dirVec,
	const float& rotate, const system_clock::time_point& startTime,
	const float& delta, const bool& movings)
{
	cs_packet_move packet;

	packet.type = CS_PACKET_MOVE;
	packet.size = sizeof(packet);
	packet.direction = dir;

	packet.rotate = rotate;

	packet.start = startTime;
	packet.deltaTime = delta;
	packet.movings = movings;

	Send_Packet(&packet);



}

void CNetwork::Send_Move_Packet(const Vec3& localPos, const Vec3& dirVec, const float& rotate, const system_clock::time_point& startTime, const float& delta)
{

	cs_packet_move packet;

	packet.type = CS_PACKET_MOVE;
	packet.size = sizeof(packet);

	packet.localPos = localPos;
	packet.dirVec = dirVec;
	
	packet.rotate = rotate;


	packet.start = startTime;
	packet.deltaTime = delta;

	Send_Packet(&packet);
}
