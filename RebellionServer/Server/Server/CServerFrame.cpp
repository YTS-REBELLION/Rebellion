#include "pch.h"
#include "Error.h"
#include "Protocol.h"
#include "Shared.h"
#include "Sender.h"
#include "CServerFrame.h"


constexpr int WORKER_THREAD_NUM{ 4 };
CServerFrame::CServerFrame()
{
	_useDB = false;
	_dummpyIndex = 0;
	_prevTime = std::chrono::system_clock::now();
	_error = new CError;
	_sender = new CSender;


}
CServerFrame::~CServerFrame()
{
	if (nullptr != _error) {
		delete _error;
		_error = nullptr;

	}
	if (nullptr != _sender) {
		delete _sender;
		_sender = nullptr;
	}
	_timerThread.join();
	for (std::thread& t : _workerThread)
		t.join();

	closesocket(_listenSocket);
	WSACleanup();
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
	InitClients();

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
	std::cout << "CreateWorkerThread()" << std::endl;
	return std::thread([this] {this->DoWorker(); });
}

std::thread CServerFrame::CreateTimerThread()
{
	return std::thread([this] {this->DoTimer(); });
}

void CServerFrame::InitClients()
{
	std::cout << "InitClients" << endl;
	for (int i = 0; i < NPC_ID_START; ++i) {
		_objects[i].SetIsDummy(false);
		_objects[i]._status = ST_FREE;
		_objects[i].SetID(i);
		_objects[i].SetSpeed(i);
		_objects[i].SetMyType(PLAYER);

		int idx = 0;
		float x, z;
		switch (idx) {
		case 0:
			x = -1700.f;
			z = 3800.f;
			break;
		case 1:
			x = -1900.f;
			z = 4600.f;
			break;
		case 2:
			x = -2100.f;
			z = 5300.f;
			break;
		case 3:
			x = -2300.f;
			z = 4300.f;
			break;
		case 4:
			x = -2500.f;
			z = 5000.f;
			break;
		case 5:
			x = -2800.f;
			z = 4000.f;
			break;
		}
		idx = (idx + 1) % 6;
		//Vec3 pos;
		//pos.x = x;
		//pos.y = 0.f;
		//pos.z = z;
		_objects[i].SetCurrentExp(0);
		_objects[i].SetMaxExp(100);
		_objects[i].SetCurrentHp(200);
		_objects[i].SetMaxHp(200);
		//_objects[i].SetPos(pos);
		_objects[i].SetDamage(50);
		_objects[i].SetLevel(1);
		_objects[i].SetIsAttack(false);
	}
}
void CServerFrame::RecvPacketProcess(int id, int iobytes)
{
	OVER_EX& recvOver = _objects[id]._recvOver;
	
	int restBytes = iobytes;
	char* p = recvOver.net_buf;
	int packetSize = 0;

	if (0 != _objects[id].GetPrevSize()) {
		packetSize = _objects[id]._packetBuf[0];

	}
	while (restBytes > 0) {
		if (0 == packetSize) {
			packetSize = *p; // p[0]
		}

		if (packetSize <= restBytes + _objects[id].GetPrevSize()) {
			memcpy(_objects[id]._packetBuf + _objects[id].GetPrevSize(),
				p, packetSize - _objects[id].GetPrevSize());

			p += packetSize - _objects[id].GetPrevSize();
			restBytes -= packetSize - _objects[id].GetPrevSize();
			packetSize = 0;
			ProcessPacket(id, _objects[id]._packetBuf);
			_objects[id].SetPrevSize(0);
		}
		else {
			memcpy(_objects[id]._packetBuf + _objects[id].GetPrevSize(), p, restBytes);
			_objects[id].SetPrevSize(_objects[id].GetPrevSize() + restBytes);
			restBytes = 0;
			p += restBytes;
		}
	}


}
void CServerFrame::ProcessPacket(int id, char* buf)
{
	switch (buf[1]) {
	case CS_LOGIN: {
		std::cout <<"ID : " << id << "플레이어 등장" << std::endl;
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);

		char name[MAX_ID_LEN];
		strcpy_s(name, packet->name);

		cout << "플레이어 이름 " << name << endl;
		
		_sender->SendLoginOkPacket(_objects[id].GetSocket(), id, 
			0.f, 0.f, 0.f, 10, 20, 30, 40, 50, 60);



		//_sender->SendLoginFailPacket(_objects[id].GetSocket());



		// DB 구현 예정

		// DB에 정보가 있을 때





	}
	break;
	}


}
void CServerFrame::Disconnect(int id)
{
	_sender->SendLeavePacket(_objects[id].GetSocket(), id, _objects[id].GetMyType());
	_objects[id].ClientLock();
	_objects[id]._status = ST_INGAME;
	closesocket(_objects[id].GetSocket());
	for (int i = 0; i < NPC_ID_START; ++i) {
		CObject& cl = _objects[id];
		if (id == cl.GetID())
			continue;

		if (ST_ACTIVE == cl._status) {
			cl.ClientLock();
			cl.EraseViewList(id);
			cl.ClientUnLock();
			_sender->SendLeavePacket(cl.GetSocket(), id, _objects[id].GetMyType());
		}


	}
	_objects[id]._status = ST_FREE;
	_objects[id].ClientLock();

}
void CServerFrame::DoWorker()
{
	printf("Start worker_thread\n");

	while (true) {
		DWORD ioBytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over;

		int ret = GetQueuedCompletionStatus(_iocp, &ioBytes, &key, &over, INFINITE);

		

		OVER_EX* exp_over = reinterpret_cast<OVER_EX*>(over);
		int id = static_cast<int>(key);
		//CObject& cl = m_objects[id];
		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			_error->error_display("GQCS Error : ", err_no);
			std::cout << std::endl;
			Disconnect(id);
			if (exp_over->event_type == EV_SEND)
				delete exp_over;
			continue;
		}


		//std::cout << over_ex->event_type << std::endl;
		//std::cout << over_ex->c_sock << std::endl;

		switch (exp_over->event_type) {
		case EV_ACCEPT: {
			printf("Accept Player\n");
			int user_id = 0;
			for (int i = 0; i < NPC_ID_START; ++i) {
				_objects[i].ClientLock();
				if (ST_FREE == _objects[i]._status) {
					_objects[i]._status = ST_INGAME;
					_objects[i].ClientUnLock();
					user_id = i;
					break;
				}
				_objects[i].ClientUnLock();
			}

			printf("지금 들어온 플레이어 %d\n", user_id);
			SOCKET c_sock = exp_over->c_sock;
			if (-1 == user_id) {
				closesocket(c_sock);
			}
			else {
				cout << user_id << " 에게 보낼 선물 준비중\n";
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_sock), _iocp, user_id, 0);

				CObject& newPlayer = _objects[user_id];
				newPlayer.SetSocket(c_sock);
				newPlayer.SetID(user_id);
				newPlayer.SetPrevSize(0);
				newPlayer.ClearViewList();
				


				ZeroMemory(&newPlayer._recvOver, sizeof(newPlayer._recvOver));
				newPlayer._recvOver.wsabuf.buf = newPlayer._recvOver.net_buf;
				newPlayer._recvOver.wsabuf.len = MAX_BUFFER;
				newPlayer._recvOver.event_type = EV_RECV;
				

				DWORD flags = 0;



				int ret = WSARecv(c_sock, &newPlayer._recvOver.wsabuf, 1,
					NULL, &flags, &newPlayer._recvOver.over, NULL);
				
				if (SOCKET_ERROR == ret) {
					int err_no = WSAGetLastError();
					if (ERROR_IO_PENDING != err_no)
						_error->error_display("ACCEPT RECV", err_no);
				}
				
			}

			ZeroMemory(&exp_over->over, sizeof(exp_over));
			c_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exp_over->c_sock = c_sock;
			AcceptEx(_listenSocket, c_sock, exp_over->net_buf, NULL, sizeof(sockaddr_in) + 16, 
				sizeof(sockaddr_in) + 16, NULL, &exp_over->over);
			break;

		}

		case EV_RECV: {
			cout << "RECV PACKET" << endl;
			if (0 == ioBytes) {
				Disconnect(id);
			}
			else {
				RecvPacketProcess(id, ioBytes);
				ZeroMemory(&_objects[id]._recvOver.over, sizeof(_objects[id]._recvOver.over));
				DWORD flags = 0;
				WSARecv(_objects[id].GetSocket(), &_objects[id]._recvOver.wsabuf, 1, 
					NULL, &flags, &_objects[id]._recvOver.over, NULL);
			}
			break;
		}

		case EV_SEND:
			if (0 == ioBytes) {
				Disconnect(id);
			}
			delete exp_over;
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


