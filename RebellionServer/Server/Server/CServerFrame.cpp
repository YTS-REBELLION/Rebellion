#include "pch.h"
#include "Error.h"
#include "Protocol.h"
#include "Shared.h"
#include "CServerFrame.h"

constexpr int WORKER_THREAD_NUM{ 4 };
CServerFrame::CServerFrame()
{

}
CServerFrame::~CServerFrame()
{

}

void CServerFrame::Progress()
{
	InitServer();
}

void CServerFrame::InitServer()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == _listenSocket) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			_error->error_display("Listen Socket Error : ", err_no);

	}


	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int ret = ::bind(_listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
	
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			_error->error_display("bind Error : ", err_no);
	}

	listen(_listenSocket, SOMAXCONN);

	_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);


	// init objcet

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(_listenSocket), _iocp, 999999, 0);
	SOCKET c_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	OVER_EX acceptOver;
	
	ZeroMemory(&acceptOver.over, sizeof(acceptOver.over));
	acceptOver.event_type = EV_ACCEPT;
	acceptOver.c_sock = c_sock;
	AcceptEx(_listenSocket, c_sock, acceptOver.net_buf, NULL, sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16, NULL, &acceptOver.over);

	//Create worker thread
	_workerThread.reserve(WORKER_THREAD_NUM);
	for (int i = 0; i < WORKER_THREAD_NUM;++i)
		_workerThread.emplace_back(CreateWorkerThread());

	_timerThread = CreateTimerThread();

	EVENT ev{ MASTER, std::chrono::high_resolution_clock::now() + 200ms,EV_BROADCAST,0 };
	AddTimer(ev);


}

std::thread CServerFrame::CreateWorkerThread()
{
	return std::thread([this] {this->DoWorker(); });
}

std::thread CServerFrame::CreateTimerThread()
{
	return std::thread([this] {this->DoTimer(); });
}

void CServerFrame::DoWorker()
{
	printf("Start worker_thread\n");

	while (true) {
		DWORD ioBytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over;

		GetQueuedCompletionStatus(_iocp, &ioBytes, &key, &over, INFINITE);

		OVER_EX* over_ex = reinterpret_cast<OVER_EX*>(over);
		int id = static_cast<int>(key);
		//CObject& cl = m_objects[id];

		//std::cout << over_ex->event_type << std::endl;
		//std::cout << over_ex->c_sock << std::endl;

		switch (over_ex->event_type) {
		case EV_ACCEPT: {
			printf("Accept Player\n");
			//int user_id = -1;
			//for (int i = 0; i < NPC_ID_START; ++i) {
			//	//m_objects[i].ClientLock();
			//	if (ST_FREE == m_objects[i].m_status) {
			//		m_objects[i].m_status = ST_ALLOC;
			//		m_objects[i].ClientUnLock();
			//		user_id = i;
			//		break;
			//	}
			//	m_objects[i].ClientUnLock();
			//}

			//printf("%d", user_id);
			SOCKET c_sock = over_ex->c_sock;
			if (-1 == user_id) {
				closesocket(c_sock);
			}
			else {
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_sock), _iocp, user_id, 0);
				//CObject& newPlayer = m_objects[user_id];
				newPlayer.SetSocket(c_sock);
				newPlayer.SetID(user_id);
				newPlayer.SetPrevSize(0);
				newPlayer.ClearViewList();
				newPlayer.m_recvOver.wsabuf.buf = newPlayer.m_recvOver.net_buf;
				newPlayer.m_recvOver.wsabuf.len = MAX_BUFFER;
				newPlayer.m_recvOver.event_type = EV_RECV;
				DWORD flags = 0;
				WSARecv(c_sock, &newPlayer.m_recvOver.wsabuf, 1, NULL, &flags, &newPlayer.m_recvOver.over, NULL);
			}

			c_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			over_ex->c_sock = c_sock;
			ZeroMemory(&over_ex->over, sizeof(over_ex->over));
			AcceptEx(_listenSocket, c_sock, over_ex->net_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &over_ex->over);

			break;
		}

		//case EV_RECV: {
		//	if (0 == ioBytes) {
		//		//Disconnect(id);
		//	}
		//	else {
		//		//RecvPacketConstruct(id, ioBytes);
		//		//ZeroMemory(&m_objects[id].m_recvOver.over, sizeof(m_objects[id].m_recvOver.over));
		//		DWORD flags = 0;
		//		WSARecv(m_objects[id].GetSocket(), &m_objects[id].m_recvOver.wsabuf, 1, NULL, &flags, &m_objects[id].m_recvOver.over, NULL);
		//	}
		//	break;
		//}

		case EV_SEND:
			if (0 == ioBytes) {
				//Disconnect(id);
			}
			delete over_ex;
			break;

		
		default:
			std::cout << "Unknown EVENT\n";
			while (true);
		}
	}
}

void CServerFrame::DoTimer()
{
}

void CServerFrame::AddTimer(EVENT ev)
{
	_timerLock.lock();
	_timerQueue.push(ev);
	_timerLock.unlock();
}


