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
	
	
	
}

void CServerFrame::Progress()
{
	InitServer();
}

void CServerFrame::InitServer()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
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
	CreateMonster();
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(_listenSocket), _iocp, 10000, 0);
	SOCKET c_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	EXP_OVER acceptOver;
	
	ZeroMemory(&acceptOver.over, sizeof(acceptOver.over));
	acceptOver.event_type = OP_ACCEPT;
	acceptOver.c_sock = c_sock;
	acceptOver.wsabuf.len = static_cast<int>(c_sock);

	AcceptEx(_listenSocket, c_sock, acceptOver.net_buf, NULL, sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16, NULL, &acceptOver.over);

	//Create worker thread
	_workerThread.reserve(WORKER_THREAD_NUM);
	for (int i = 0; i < WORKER_THREAD_NUM;++i)
		_workerThread.emplace_back(CreateWorkerThread());


	_timerThread = CreateTimerThread();

	EVENT ev{ MASTER, std::chrono::high_resolution_clock::now() + 200ms,EV_BROADCAST,0 };
	AddTimer(ev);


	_timerThread.join();
	for (std::thread& t : _workerThread)
		t.join();

	/*closesocket(_listenSocket);
	WSACleanup();*/
}

std::thread CServerFrame::CreateWorkerThread()
{
	std::cout << "CreateWorkerThread()" << std::endl;
	return std::thread([this] {this->DoWorker(); });
}

std::thread CServerFrame::CreateTimerThread()
{
	std::cout << "CreateTimerThread()" << std::endl;

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
		idx = (idx + 1) % 3;

		switch (idx) {
		case 0:
			x = 100.f;
			z = 100.f;
			break;
		case 1:
			x = -100.f;
			z = 100.f;
			break;
		case 2:
			x = 200.f;
			z = -200.f;
			break;
		case 3:
			x = -140.f;
			z = 400.f;
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
		Vec3 pos;
		pos.x = x;
		pos.y = 0.f;
		pos.z = z;
		
		_objects[i].SetCurrentExp(0);
		_objects[i].SetMaxExp(100);
		_objects[i].SetCurrentHp(200);
		_objects[i].SetMaxHp(200);
		_objects[i].SetPos(pos);
		_objects[i].SetDamage(50);
		_objects[i].SetLevel(1);
		_objects[i].SetIsAttack(false);
		
	}
}
void CServerFrame::RecvPacketProcess(int id, int iobytes)
{
	EXP_OVER& recvOver = _objects[id]._recvOver;
	
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
	case CS_PACKET_LOGIN: {
		std::cout <<"ID : " << id << "플레이어 등장" << std::endl;
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);

		char name[MAX_ID_LEN];
		strcpy_s(name, packet->name);

		cout << "플레이어 이름 " << name << endl;

		// DB 구현 예정

		// DB에 정보가 있을 때
		EnterGame(id, packet->name);

		// 없을 때
		//_sender->SendLoginFailPacket(_objects[id].GetSocket());
		break;

	}
	case CS_PACKET_MOVE: {
		std::cout << "ID : " << id << "이동" << std::endl;
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
		
		

		Vec3 pos;
		pos.x = packet->localPos.x;
		pos.y = packet->localPos.y;
		pos.z = packet->localPos.z;
		_objects[id].SetPos(pos);

		Do_move(id, 0 , pos, 0.f);

		break;

	}
	case CS_PACKET_RUN: {
		cout << "CS_PACKET_RUN" << endl;
		cs_packet_run* packet = reinterpret_cast<cs_packet_run*>(buf);

		unordered_set<int> old_viewList = _objects[id].GetViewList();

		Vec3 pos;
		pos.x = packet->pos.x;
		pos.y = packet->pos.y;
		pos.z = packet->pos.z;
		_objects[id].SetPos(pos);
		
		for (auto& ob : old_viewList)
		{
			if (ob == id)continue;
			if (true == IsPlayer(ob))
				_sender->SendRunPacket(_objects[ob].GetSocket(), id, pos ,packet->isRun);
		}


		break;
	}
	/*case CS_PACKET_WALK: {
		SetMoveDirection(id, packet->direction, true);
	}*/

	case CS_PACKET_TELEPORT: {

		break;

	}
	case CS_PACKET_STOP: {
		cout << "CS_PACKET_STOP" << endl;
		cs_packet_stop* packet = reinterpret_cast<cs_packet_stop*>(buf);
		short id = packet->id;
		
		Do_stop(id, false);



		break;
	}
	case CS_PACKET_ROTATE: {
		cs_packet_rotate* packet = reinterpret_cast<cs_packet_rotate*>(buf);
		
		unordered_set<int> old_viewList = _objects[id].GetViewList();


		for (auto& ob : old_viewList)
		{
			if (ob == id)continue;
			if (true == IsPlayer(ob))
				_sender->SendRotatePacket(_objects[ob].GetSocket(), id, packet->rotate);
		}

		break;
	}
	case CS_PACKET_ATTACK: {
		cout << "CS_PACKET_ATTACK" << endl;
		cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(buf);

		unordered_set<int> new_viewlist = _objects[id].GetViewList();

		for (auto& user : new_viewlist) {
			if (true == IsPlayer(user))
				_sender->SendPlayerAttackPacket(_objects[user].GetSocket(), id, packet->isAttack);
		}

		break;
	}
	}


}
void CServerFrame::Disconnect(int id)
{
	_sender->SendLeaveObjectPacket(_objects[id].GetSocket(), id, _objects[id].GetMyType());
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
			_sender->SendLeaveObjectPacket(cl.GetSocket(), id, _objects[id].GetMyType());
		}


	}
	_objects[id]._status = ST_FREE;
	_objects[id].ClientUnLock();

}
void CServerFrame::DoWorker()
{
	printf("Start worker_thread\n");

	while (true) {
		DWORD ioBytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over;

		int ret = GetQueuedCompletionStatus(_iocp, &ioBytes, &key, &over, INFINITE);

		EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(over);
		int id = static_cast<int>(key);
		//CObject& cl = m_objects[id];
		/*if (FALSE == ret) {
			int err_no = WSAGetLastError();
			_error->error_display("GQCS Error : ", err_no);
			std::cout << std::endl;
			Disconnect(id);
			if (exp_over->event_type == EV_SEND)
				delete exp_over;
			continue;
		}*/


		//std::cout << over_ex->event_type << std::endl;
		//std::cout << over_ex->c_sock << std::endl;
		//std::cout << exp_over->c_sock << std::endl;


		switch (exp_over->event_type) {
		case OP_ACCEPT: {
			printf("Accept Player\n");
			int user_id = -1;
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
				cout << "ACCEPT c_sock " << c_sock << endl;
				CObject& newPlayer = _objects[user_id];
				newPlayer.SetSocket(c_sock);
				newPlayer.SetID(user_id);
				newPlayer.SetPrevSize(0);
				newPlayer.ClearViewList();
				


				newPlayer._recvOver.wsabuf.buf = newPlayer._recvOver.net_buf;
				newPlayer._recvOver.wsabuf.len = MAX_BUFFER;
				newPlayer._recvOver.event_type = OP_RECV;
				

				DWORD flags = 0;



				int ret = WSARecv(c_sock, &newPlayer._recvOver.wsabuf, 1,
					NULL, &flags, &newPlayer._recvOver.over, NULL);
				
				if (SOCKET_ERROR == ret) {
					int err_no = WSAGetLastError();
					if (ERROR_IO_PENDING != err_no) {
						std::cout << err_no << " - \n";
						_error->error_display("ACCEPT RECV", err_no);
					}
				
				}
				if (SOCKET_ERROR == ret) {
					int err_no = WSAGetLastError();
					if (WSAEWOULDBLOCK == err_no) {
						_error->error_display("ACCEPT RECV", err_no);

					}
				}
				
			}

			ZeroMemory(&exp_over->over, sizeof(exp_over));
			c_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exp_over->c_sock = c_sock;
			AcceptEx(_listenSocket, c_sock, exp_over->net_buf, NULL, sizeof(sockaddr_in) + 16, 
				sizeof(sockaddr_in) + 16, NULL, &exp_over->over);
			break;

		}

		case OP_RECV: {
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

		case OP_SEND:
			if (0 == ioBytes) {
				Disconnect(id);
			}
			delete exp_over;
			break;
		case EV_BROADCAST:
			//cout << "ev_broadcast" << endl;
			//MoveUpdate();
			break;
		default:
			std::cout << "Unknown EVENT\n";
			while (true);
		}
	}
}

void CServerFrame::DoTimer()
{
	printf("Start timer thread\n");

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		while (true) {
			_timerLock.lock();
			if (true == _timerQueue.empty()) {
				_timerLock.unlock();
				break;
			}

			if (_timerQueue.top().wakeup_time > std::chrono::high_resolution_clock::now()) {
				_timerLock.unlock();
				break;
			}

			EVENT ev = _timerQueue.top();
			_timerQueue.pop();
			_timerLock.unlock();

			EXP_OVER* exp_over = new EXP_OVER;
			exp_over->event_type = ev.event_type;
			*(reinterpret_cast<int*>(exp_over->net_buf)) = ev.target_obj;
			PostQueuedCompletionStatus(_iocp, 1, ev.obj_id, &exp_over->over);
		}
	}


}

void CServerFrame::AddTimer(EVENT ev)
{
	_timerLock.lock();
	_timerQueue.push(ev);
	_timerLock.unlock();
}



void CServerFrame::ActivateNPC(int id)
{
	STATUS oldState = ST_SLEEP;
	std::atomic_compare_exchange_strong(&_objects[id]._status, &oldState, ST_ACTIVE);

}

void CServerFrame::MoveUpdate()
{
	//cout << "moveupdate" << endl;
	time_point<system_clock> curTime = system_clock::now();

	for (auto& obj : _objects) {
		if (ST_ACTIVE != obj._status) continue;
		if (false == IsPlayer(obj.GetID())) {
			if (false == obj.GetIsAttack()) {
				if (obj.GetMoveType() == RANDOM)
					DoRandomMove(obj.GetID());
				else DoTargetMove(obj.GetID());
			}
		}
		else UpdatePlayerPos(obj.GetID());
	}
	_elapsedTime = curTime - _prevTime;

	_prevTime = curTime;

	EVENT ev{ MASTER, high_resolution_clock::now() + 200ms, EV_BROADCAST,0 };
	AddTimer(ev);

}

bool CServerFrame::IsPlayer(int id)
{
	return id < NPC_ID_START;
}

bool CServerFrame::IsNearNPC(int player, int npc)
{
	Vec3 A_vPos;
	A_vPos = _objects[player].GetPos();

	Vec3 B_vPos;
	B_vPos = _objects[npc].GetPos();

	float distance = Vec3::Distance(A_vPos, B_vPos);

	/*if (O_DRAKKEN == m_objects[player].GetMyType()) {
		if (distance > 70.f) return false;
	}
	else if (O_PLAYER == m_objects[player].GetMyType()) {
		if (distance > 500.f) return false;
	}*/

	return true;
}

void CServerFrame::DoRandomMove(int id)
{
	cout << "dorandommove" << endl;
	Vec3 Pos;
	Pos.x = _objects[id].GetPos().x;
	Pos.y = _objects[id].GetPos().y;
	Pos.z = _objects[id].GetPos().z;

	float speed = _objects[id].GetSpeed();

	int index = _objects[id].GetNextPosIndex();
	Vec3 nextPos = _objects[id].GetNextPos(index);

	Vec3 dir;
	dir.x = nextPos.x - Pos.x;
	dir.y = nextPos.y - Pos.y;
	dir.z = nextPos.z - Pos.z;

	Vec3 nor = dir.Normalize();

	Pos.x += _elapsedTime.count() * speed * nor.x;
	Pos.y += _elapsedTime.count() * speed * nor.y;
	Pos.z += _elapsedTime.count() * speed * nor.z;


	_objects[id].SetPos(Pos);
	_objects[id].SetLook(nor);

	float distance = Vec3::Distance(nextPos, _objects[id].GetPos());

	if (distance < 50.f) {
		index = index + 1;
		if (index >= 3) index = 0;
		_objects[id].SetNextPosIndex(index);
	}


	for (int i = 0; i < NPC_ID_START; ++i) {
		if (ST_ACTIVE != _objects[i]._status) continue;
		if (true == IsNear(i, id)) {
			_objects[i].ClientLock();
			if (0 != _objects[i].GetViewListCount(id)) {
				_objects[i].ClientUnLock();
				/*_sender->SendMovePacket(_objects[i].GetSocket(), id, _objects[id].GetPos().x, _objects[id].GetPos().y,
					_objects[id].GetPos().z, _objects[id].GetLook().x, 
					_objects[id].GetLook().y, _objects[id].GetLook().z, WALK,
					std::chrono::system_clock::now());*/
			}
			else {
				_objects[i].InsertViewList(id);
				_objects[i].ClientUnLock();
				//cout << "sendputobject1" << endl;
				_sender->SendPutObjectPacket(_objects[i].GetSocket(), id, _objects[id].GetPos().x,
					_objects[id].GetPos().y, _objects[id].GetPos().z,
					_objects[id].GetMyType());
			}
		}
		else {
			_objects[i].ClientLock();
			if (0 != _objects[i].GetViewListCount(id)) {
				_objects[i].EraseViewList(id);
				_objects[i].ClientUnLock();
				_sender->SendLeaveObjectPacket(_objects[i].GetSocket(), id, _objects[id].GetMyType());
			}
			else {
				_objects[i].ClientUnLock();
			}
		}
	}

	for (int i = 0; i < NPC_ID_START; ++i) {
		if (true == IsNear(id, i)) {
			if (ST_ACTIVE == _objects[i]._status) {
				return;
			}
		}
	}
	_objects[id]._status = ST_SLEEP;

}

bool CServerFrame::IsNear(int a, int b)
{

	if (abs(_objects[a].GetPos().x - _objects[b].GetPos().x) > VIEW_RADIUS) return false;
	if (abs(_objects[a].GetPos().y - _objects[b].GetPos().y) > VIEW_RADIUS) return false;
	if (abs(_objects[a].GetPos().z - _objects[b].GetPos().z) > VIEW_RADIUS) return false;
	return true;
}

void CServerFrame::DoTargetMove(int npc_id)
{
	cout << "dotargetmove" << endl;
	Vec3 Pos;
	int player_id = _objects[npc_id].GetTargetID();

	if (ST_ACTIVE != _objects[npc_id]._status) return;
	if (ST_ACTIVE != _objects[player_id]._status) return;
	if (RANDOM == _objects[npc_id].GetMoveType()) return;

	if (true == IsPlayer(npc_id)) {
		std::cout << "ID :" << npc_id << " is not NPC!!\n";
		while (true);
	}

	//if (false == IsNearNPC(player_id, npc_id)) {
	//	_objects[npc_id].SetMoveType(RANDOM);
	//	_objects[npc_id].SetTargetID(-1);
	//	char type = _objects[npc_id].GetMyType();

	//	/*switch (type) {
	//	case O_BARGHEST: m_objects[npc_id].SetSpeed(BARGHEST_WALK_SPEED); break;
	//	case O_GRIFFON: m_objects[npc_id].SetSpeed(GRIFFON_WALK_SPEED); break;
	//	case O_DRAGON:m_objects[npc_id].SetSpeed(DRAGON_WALK_SPEED); break;
	//	}*/
	//	return;
	//}

	float speed = _objects[npc_id].GetSpeed();
	Pos.x = _objects[npc_id].GetPos().x;
	Pos.y = _objects[npc_id].GetPos().y;
	Pos.z = _objects[npc_id].GetPos().z;
	Vec3 dir;
	dir.x = _objects[player_id].GetPos().x - Pos.x;
	dir.y = _objects[player_id].GetPos().y - Pos.y;
	dir.z = _objects[player_id].GetPos().z - Pos.z;
	Vec3 nor = dir.Normalize();

	//std::cout << nor.x << ", " << nor.y << ", " << nor.z << std::endl;
	//std::cout << m_elapsedTime.count() << std::endl;

	Vec3 A_vPos;
	A_vPos = _objects[player_id].GetPos();

	Vec3 B_vPos;
	B_vPos = _objects[npc_id].GetPos();

	float distance = Vec3::Distance(A_vPos, B_vPos);

	bool closed = false;
	/*switch (m_objects[npc_id].GetMyType()) {
	case 몬스터 이름:
		if (O_DRAKKEN == m_objects[player_id].GetMyType()) {
			if (distance < 490.f) closed = true;
		}
		else if (O_PLAYER == m_objects[player_id].GetMyType()) {
			if (distance < 310.f) closed = true;
		}
		break;
	case 몬스터 이름:
	case 몬스터 이름:
		if (몬스터 이름 == m_objects[player_id].GetMyType()) {
			if (distance < 310.f) closed = true;
		}
		else if (O_PLAYER == m_objects[player_id].GetMyType()) {
			if (distance < 130.f) closed = true;
		}
		break;
	}*/

	if (true == closed) {
		if (false == _objects[npc_id].GetIsAttack()) {
			_objects[npc_id].SetIsAttack(true);

			EVENT new_ev{ npc_id, std::chrono::high_resolution_clock::now() + 2s, EV_ATTACK, 0 };
			AddTimer(new_ev);

			// 방어 넣으면 여기
			if (false == _objects[player_id].GetIsDefence()) {
				_objects[player_id].SetCurrentHp(_objects[player_id].GetCurrentHp() - _objects[npc_id].GetDamage());
				_sender->SendHpPacket(_objects[player_id].GetSocket(), _objects[player_id].GetCurrentHp());
				//printf("Hp: %d\n", m_objects[player_id].GetCurrentHp());

				if (0 >= _objects[player_id].GetCurrentHp()) {
					// 죽음
					short level = _objects[player_id].GetLevel();
					short hp = _objects[player_id].GetCurrentHp();
					short changeHp = hp - (50 * level);
					if (0 > changeHp) changeHp = 0;
					_objects[player_id].SetCurrentExp(changeHp);
					_objects[player_id].SetCurrentHp(50);
					_objects[player_id].SetPos(VEC3_TOWN_ENTRANCE_POS);
					_sender->SendPlayerDiePacket(_objects[player_id].GetSocket(), player_id);
					std::unordered_set<int> vl = _objects[player_id].GetViewList();
					for (const auto& id : vl) {
						if (false == IsPlayer(id)) continue;
						if (ST_ACTIVE != _objects[id]._status) continue;
						_sender->SendPlayerDiePacket(_objects[id].GetSocket(), player_id);
					}
				}
			}
			//////////////////////

			for (auto& cl : _objects) {
				if (false == IsPlayer(cl.GetID())) continue;
				if (false == IsNear(cl.GetID(), npc_id)) continue;
				cl.ClientLock();
				if (ST_ACTIVE == cl._status) {
					_sender->SendNPCAttackPacket(cl.GetSocket(), npc_id, Pos.x, Pos.z);
				}
				cl.ClientUnLock();
			}
		}
		return;
	}

	Pos.x += _elapsedTime.count() * speed * nor.x;
	Pos.y += _elapsedTime.count() * speed * nor.y;
	Pos.z += _elapsedTime.count() * speed * nor.z;

	bool check = false;
	for (int i = 0; i < NUM_OBSTACLES; ++i) {
		if (-999 == _obstacles[i].xScale) break;

		// 플레이어
		Vec3 pPos;
		pPos.x = Pos.x;
		pPos.y = Pos.y;
		pPos.z = Pos.z;
		// obstacles
		Vec3 oPos;
		oPos.x = _obstacles[i].xPos;
		oPos.y = _obstacles[i].yPos;
		oPos.z = _obstacles[i].zPos;
		float r = _obstacles[i].zScale;

		float distance = Vec3::Distance(pPos, oPos);

		if (distance <= r) {
			check = true;
			break;
		}
	}

	if (false == check) {
		_objects[npc_id].SetPos(Pos);
	}

	_objects[npc_id].SetLook(nor);

	char status{}; // 애니메이션 상태
	//if (O_BARGHEST == m_objects[npc_id].GetMyType()) status = RUN;
	//else status = WALK;

	for (int i = 0; i < NPC_ID_START; ++i) {
		if (ST_ACTIVE != _objects[i]._status) continue;
		if (true == IsNear(i, npc_id)) {
			_objects[i].ClientLock();
			if (0 != _objects[i].GetViewListCount(npc_id)) {
				_objects[i].ClientUnLock();
				/*_sender->SendMovePacket(_objects[i].GetSocket(), npc_id, _objects[npc_id].GetPos().x,
					_objects[npc_id].GetPos().y,  _objects[npc_id].GetPos().z, 
					_objects[npc_id].GetLook().x, _objects[npc_id].GetLook().y,
					_objects[npc_id].GetLook().z, status, std::chrono::system_clock::now());*/
			}
			else {
				_objects[i].InsertViewList(npc_id);
				_objects[i].ClientUnLock();
				//cout << "sendputobject2" << endl;

				_sender->SendPutObjectPacket(_objects[i].GetSocket(), npc_id, _objects[npc_id].GetPos().x,
					_objects[npc_id].GetPos().y, _objects[npc_id].GetPos().z, 
					_objects[npc_id].GetMyType());
			}
		}
		else {
			_objects[i].ClientLock();
			if (0 != _objects[i].GetViewListCount(npc_id)) {
				_objects[i].EraseViewList(npc_id);
				_objects[i].ClientUnLock();
				_sender->SendLeaveObjectPacket(_objects[i].GetSocket(), npc_id, _objects[npc_id].GetMyType());
			}
			else {
				_objects[i].ClientUnLock();
			}
		}
	}
}


void CServerFrame::UpdatePlayerPos(int id)
{
	cout << "updateplayerpos" << endl;
	unordered_set<int> vl = _objects[id].GetViewList();
	for (const int& npc : vl) {
		if (true == IsPlayer(npc)) continue;
		if (ST_ACTIVE != _objects[npc]._status) continue;
		if (true == IsNearNPC(id, npc)) {
			_objects[npc].SetMoveType(TARGET);
			_objects[npc].SetTargetID(id);
			char type = _objects[npc].GetMyType();
			
		}
	}


	Vec3 pos = _objects[id].GetPos();
	Vec3 look = _objects[id].GetLook();

	_objects[id].ClientLock();

	std::unordered_set<int> oldViewList = _objects[id].GetViewList();
	_objects[id].ClientUnLock();
	std::unordered_set<int> newViewList;

	/*for (auto& cl : _objects) {
		if (false == IsNear(cl.GetID(), id)) continue;
		if (ST_SLEEP == cl._status) ActivateNPC(cl.GetID());
		if (ST_ACTIVE != cl._status) continue;
		if (cl.GetID() == id) continue;
		newViewList.insert(cl.GetID());
	}*/

	for (auto& np : newViewList) {
		if (0 == oldViewList.count(np)) {	// Object가 시야에 새로 들어왔을 때.
			_objects[id].ClientLock();
			_objects[id].InsertViewList(np);
			_objects[id].ClientUnLock();

			_sender->SendPutObjectPacket(_objects[id].GetSocket(), np, _objects[np].GetPos().x, 
				_objects[np].GetPos().y, _objects[np].GetPos().z,
				_objects[np].GetMyType());

			if (false == IsPlayer(np)) continue;
			_objects[np].ClientLock();
			if (0 == _objects[np].GetViewListCount(id)) {
				_objects[np].InsertViewList(id);
				_objects[np].ClientUnLock();

				_sender->SendPutObjectPacket(_objects[np].GetSocket(), id, _objects[id].GetPos().x, 
					_objects[id].GetPos().y, _objects[id].GetPos().z,
					_objects[id].GetMyType());

			}
			else {
				_objects[np].ClientUnLock();
				
			}
		}
		else {							// Object가 계속 시야에 존재하고 있을 떄.
			if (false == IsPlayer(np)) continue;
			_objects[np].ClientLock();
			if (0 != _objects[np].GetViewListCount(id)) {
				_objects[np].ClientUnLock();
			}
			else {
				_objects[np].ClientUnLock();
				_objects[np].InsertViewList(id);
				//cout << "sendputobject5" << endl;

				_sender->SendPutObjectPacket(_objects[np].GetSocket(), id, _objects[id].GetPos().x, _objects[id].GetPos().y, 
					_objects[id].GetPos().z, _objects[id].GetMyType());
			}
		}
	}

	for (auto& op : oldViewList) {		// Object가 시야에서 벗어났을 때.
		if (0 == newViewList.count(op)) {
			_objects[id].ClientLock();
			_objects[id].EraseViewList(op);
			_objects[id].ClientUnLock();
			_sender->SendLeaveObjectPacket(_objects[id].GetSocket(), op, _objects[op].GetMyType());
			if (false == IsPlayer(op)) continue;
			_objects[op].ClientLock();
			if (0 != _objects[op].GetViewListCount(id)) {
				_objects[op].EraseViewList(id);
				_objects[op].ClientUnLock();
				_sender->SendLeaveObjectPacket(_objects[op].GetSocket(), id, _objects[id].GetMyType());
			}
			else {
				_objects[op].ClientUnLock();
			}
		}
	}
}


void CServerFrame::Do_move(const short& id, const char& dir, Vec3& localPos, const float& rotate)
{

	_objects[id].ClientLock();

	std::unordered_set<int> oldViewList = _objects[id].GetViewList();
	_objects[id].ClientUnLock();
	std::unordered_set<int> newViewList;

	for (auto& cl : _objects) {
		if (false == IsNear(cl.GetID(), id)) continue;
		if (ST_SLEEP == cl._status) ActivateNPC(cl.GetID());
		if (ST_ACTIVE != cl._status) continue;
		if (cl.GetID() == id) continue;
		newViewList.insert(cl.GetID());
	}

	for (auto& np : newViewList) {
		if (0 == oldViewList.count(np)) {	// Object가 시야에 새로 들어왔을 때.
			_objects[id].ClientLock();
			_objects[id].InsertViewList(np);
			_objects[id].ClientUnLock();

			_sender->SendPutObjectPacket(_objects[id].GetSocket(), np,
				_objects[np].GetPos().x, _objects[np].GetPos().y, _objects[np].GetPos().z, 
				_objects[np].GetMyType());

			if (false == IsPlayer(np)) continue;

			_objects[np].ClientLock();
			if (0 == _objects[np].GetViewListCount(id)) {
				_objects[np].InsertViewList(id);
				_objects[np].ClientUnLock();
				_sender->SendPutObjectPacket(_objects[np].GetSocket(), id, 
					_objects[id].GetPos().x, _objects[id].GetPos().y, _objects[id].GetPos().z, 
					_objects[id].GetMyType());
			}
			else {
				_objects[np].ClientUnLock();
				_sender->SendMovePacket(_objects[np].GetSocket(), id, _objects[id].GetPos(),
					_objects[id].GetLook().x, _objects[id].GetLook().y, _objects[id].GetLook().z, true,
					std::chrono::system_clock::now());
			}
		}
		else {							// Object가 계속 시야에 존재하고 있을 떄.
			if (false == IsPlayer(np)) continue;
			_objects[np].ClientLock();
			if (0 != _objects[np].GetViewListCount(id)) {
				_objects[np].ClientUnLock();
				_sender->SendMovePacket(_objects[np].GetSocket(), id, _objects[id].GetPos(),
					_objects[id].GetLook().x, _objects[id].GetLook().y, _objects[id].GetLook().z, true,
					std::chrono::system_clock::now());
			}
			else {
				_objects[np].ClientUnLock();
				_objects[np].InsertViewList(id);
				_sender->SendPutObjectPacket(_objects[np].GetSocket(), id, 
					_objects[id].GetPos().x, _objects[id].GetPos().y, _objects[id].GetPos().z, 
					_objects[id].GetMyType());
			}
		}
	}

	
}

void CServerFrame::Do_stop(const short& id, const bool& isMoving)
{
	

    unordered_set<int> old_viewList =_objects[id].GetViewList();
    

    _objects[id].SetIsMove(false);

    
    for (auto& ob : old_viewList)
    {
		if (ob == id)continue;
		if (false == IsPlayer(ob)) continue;
		//if (true == IsPlayer(ob))
		_sender->Send_Stop_Packet(_objects[ob].GetSocket(), id);
    }

}

void CServerFrame::EnterGame(int id, const char* name)
{
	cout << "entergame" << endl;
	_objects[id].ClientLock();
	_objects[id].SetMyName(name);
	

	_sender->SendLoginOkPacket(_objects[id].GetSocket(), _objects[id].GetID(),
		200.f, 0.f, 200.f,
		_objects[id].GetDamage(), _objects[id].GetCurrentHp(),
		_objects[id].GetMaxHp(), _objects[id].GetLevel(),
		_objects[id].GetCurrentExp(), _objects[id].GetMaxExp());

	_objects[id]._status = ST_ACTIVE;
	_objects[id].ClientUnLock();


	for (auto& cl : _objects) {
		int i = cl.GetID();
		if(id == i)continue;
		if (true == IsNear(id, i)) {


			if (ST_SLEEP == _objects[i]._status) {
				ActivateNPC(i);
			}

			if (ST_ACTIVE == _objects[i]._status) {
				cout << "STATUS : ST_ACTIVE" << endl;
				_objects[id].ClientLock();
				_objects[id].InsertViewList(i);
				_objects[id].ClientUnLock();
				//cout << "sendputobject6" << endl;

				_sender->SendPutObjectPacket(_objects[id].GetSocket(), i, _objects[i].GetPos().x,
					_objects[i].GetPos().y, _objects[i].GetPos().z, _objects[i].GetMyType());
				if (true == IsPlayer(i)) {
					_objects[i].ClientLock();
					_objects[i].InsertViewList(id);
					_objects[i].ClientUnLock();
					//cout << "sendputobject7" << endl;

					_sender->SendPutObjectPacket(_objects[i].GetSocket(), id, _objects[id].GetPos().x,
						_objects[id].GetPos().y, _objects[id].GetPos().z, _objects[id].GetMyType());
				}
			}
		}


	}

}

void CServerFrame::CreateMonster()
{
	cout << "Initializing Monster" << endl;

	for (int monsterId = NPC_ID_START; monsterId < NPC_ID_START + 5; ++monsterId) {
		_objects[monsterId].SetID(monsterId);
		_objects[monsterId]._status = ST_SLEEP;
		_objects[monsterId].SetSpeed(80.f);

		_objects[monsterId].SetCurrentHp(150);
		_objects[monsterId].SetMaxHp(150);

		_objects[monsterId].SetLevel(1);

		//_objects[monsterId].SetMoveType(RANDOM);

		//_objects[monsterId].SetIsAttack(false);

		
		//_objects[monsterId].SetNextPosIndex(0);

		_objects[monsterId].SetDamage(_objects[monsterId].GetLevel() * 10);



	}


	_objects[NPC_ID_START].SetPos(Vec3(0.f, 5000.f, 3200.f));/*
	_objects[NPC_ID_START].SetNextPos(0, 900.f, 0.f, 1200.f);
	_objects[NPC_ID_START].SetNextPos(1, 1300.f, 0.f, 900.f);
	_objects[NPC_ID_START].SetNextPos(2, 400.f, 0.f, 1500.f);*/


	_objects[NPC_ID_START + 1].SetPos(Vec3(200.f, 5000.f, 3400.f));/*
	_objects[NPC_ID_START + 1].SetNextPos(0, 70.f, 0.f, 450.f);
	_objects[NPC_ID_START + 1].SetNextPos(1, 370.f, 0.f, 750.f);
	_objects[NPC_ID_START + 1].SetNextPos(2, 770.f, 0.f, 750.f);*/

	_objects[NPC_ID_START + 2].SetPos(Vec3(400.f, 5000.f, 3400.f));/*
	_objects[NPC_ID_START + 2].SetNextPos(0, 70.f, 0.f, 450.f);
	_objects[NPC_ID_START + 2].SetNextPos(1, 370.f, 0.f, 750.f);
	_objects[NPC_ID_START + 2].SetNextPos(2, 770.f, 0.f, 750.f);*/

	_objects[NPC_ID_START + 3].SetPos(Vec3(-200.f, 5000.f, 3400.f));/*
	_objects[NPC_ID_START + 3].SetNextPos(0, 70.f, 0.f, 450.f);
	_objects[NPC_ID_START + 3].SetNextPos(1, 370.f, 0.f, 750.f);
	_objects[NPC_ID_START + 3].SetNextPos(2, 770.f, 0.f, 750.f);*/

	_objects[NPC_ID_START + 4].SetPos(Vec3(-400.f, 5000.f, 3400.f));/*
	_objects[NPC_ID_START + 4].SetNextPos(0, 70.f, 0.f, 450.f);
	_objects[NPC_ID_START + 4].SetNextPos(1, 370.f, 0.f, 750.f);
	_objects[NPC_ID_START + 4].SetNextPos(2, 770.f, 0.f, 750.f);*/

	printf("Monster Initialization finished.\n");

}