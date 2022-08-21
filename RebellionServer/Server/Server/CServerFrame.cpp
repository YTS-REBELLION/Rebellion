#include "pch.h"
#include "Error.h"
#include "Protocol.h"
#include "Shared.h"
#include "Sender.h"
#include "CServerFrame.h"
#include <map>
#include "func.h"
#include <algorithm>



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

	
	_timerThread.join();
	for (std::thread& t : _workerThread)
		t.join();

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
		//_objects[i].SetSpeed(i);
		_objects[i].SetMyType(PLAYER);
		int idx = 0;
		float x, z;
		idx = (idx + 1) % 3;

		switch (i) {
		case 0:
			x = 300.f;
			z = 300.f;
			break;
		case 1:
			x = -300.f;
			z = -300.f;
			break;
		case 2:
			x = 0.f;
			z = 300.f;
			break;
		}
		Vec3 pos;
		pos.x = x;//x;
		pos.y = 0.f;
		pos.z = z;// z;
		
		_objects[i].SetCurrentExp(0);
		_objects[i].SetMaxExp(100);
		_objects[i].SetCurrentHp(1500);
		_objects[i].SetMaxHp(2000);
		_objects[i].SetPos(pos);
		_objects[i].SetDamage(25);
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
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
		
		

		Vec3 pos;
		pos.x = packet->localPos.x;
		pos.y = packet->localPos.y;
		pos.z = packet->localPos.z;
		_objects[id].SetPos(pos);


		if (_objects[id].GetDunGeonEnter()) {
			Do_move_Dungeon(id, 0, pos, 0.f);
		}
		else {
			Do_move(id, 0, pos, 0.f);
		}

		break;

	}
	case CS_PACKET_RUN: {
		cs_packet_run* packet = reinterpret_cast<cs_packet_run*>(buf);
		unordered_set<int> temp_vl;
		if (_objects[id].GetDunGeonEnter()) temp_vl = _objects[id].DungeonGetViewList();
		else temp_vl = _objects[id].GetViewList();
		
		unordered_set<int> old_viewList = temp_vl;

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
	case CS_PACKET_TELEPORT: {

		break;

	}
	case CS_PACKET_STOP: {
		cs_packet_stop* packet = reinterpret_cast<cs_packet_stop*>(buf);
		short id = packet->id;
		
		Do_stop(id, false);



		break;
	}
	case CS_PACKET_ROTATE: {
		cs_packet_rotate* packet = reinterpret_cast<cs_packet_rotate*>(buf);
		
		unordered_set<int> temp_vl;
		if (_objects[id].GetDunGeonEnter()) {
			temp_vl = _objects[id].DungeonGetViewList();
		}
		else {
			temp_vl = _objects[id].GetViewList();
		}
		unordered_set<int> old_viewList = temp_vl;


		for (auto& ob : old_viewList)
		{
			if (ob == id)continue;
			if (true == IsPlayer(ob))
				_sender->SendRotatePacket(_objects[ob].GetSocket(), id, packet->rotate);
		}

		break;
	}
	case CS_PACKET_ATTACK: {
		cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(buf);

		unordered_set<int> temp_vl;
		if (_objects[id].GetDunGeonEnter()) {
			temp_vl = _objects[id].DungeonGetViewList();
		}
		else {
			temp_vl = _objects[id].GetViewList();
		}

		unordered_set<int> old_viewList = temp_vl;

		// 애니메이션 보이기.
		for (auto& user : old_viewList) {
			if (true == IsPlayer(user))
				_sender->SendPlayerAttackPacket(_objects[user].GetSocket(), id, packet->isAttack);
		}

		break;
	}
	case CS_PACKET_P2MCOL: {
		cs_packet_player2monstercol* packet = reinterpret_cast<cs_packet_player2monstercol*>(buf);
		int monsterId = packet->playerId;
		int pid = packet->id;
		cout << monsterId << ", " << pid << endl;
		switch (packet->attackType) {
		case 0: {
			
			_objects[monsterId].SetCurrentHp(_objects[monsterId].GetCurrentHp() - _objects[pid].GetDamage());


			break;
		}
		case 1: {
			cout << "스킬 맞음" << endl;

			_objects[monsterId].SetCurrentHp(_objects[monsterId].GetCurrentHp() - _objects[pid].GetDamage());


			break;
		}
		default:
			cout << "패킷 오류" << endl;
			break;
		}
		
		
		_objects[monsterId].SetCurrentHp(_objects[monsterId].GetCurrentHp() - _objects[pid].GetDamage());

		unordered_set<int> old_viewList = _objects[id].GetViewList();

		cout<<"아이디 " << monsterId << " 체력 : " << _objects[monsterId].GetCurrentHp() << endl;

		if (_objects[monsterId].GetCurrentHp() <= 0) {
			_objects[monsterId]._status = ST_FREE;
			++monsterdieCnt;
			//_sender->SendMonsterDiePacket(_objects[id].GetSocket(), monsterId);
			_sender->SendLeaveObjectPacket(_objects[id].GetSocket(), monsterId, _objects[monsterId].GetMyType());
			cout << "몬스터 잡기 : " << monsterdieCnt << endl;
		}


		if (monsterdieCnt == 3 && !isSecondQuestDone)
		{
			for(int i = 0; i<_acceptNumber;++i){
				cout << "두번째 퀘스트 완료 패킷 전송 " << endl;
				_sender->SendQuestDonePacket(_objects[i].GetSocket(), i, QUEST::SECOND, true);
				isSecondQuestDone = true;
			}
		}

		if (monsterdieCnt == 14 && !isSecondQuestDone) {
			for (int i = 0; i < _acceptNumber; ++i) {
				cout << "두번째 퀘스트 완료 패킷 전송 " << endl;
				_sender->SendQuestDonePacket(_objects[i].GetSocket(), i, QUEST::THIRD, true);
				isSecondQuestDone = true;
				
			}
		}


		break;
	}
	case CS_PACKET_MONSTERDIR: {
		cs_packet_monsterdir* packet = reinterpret_cast<cs_packet_monsterdir*>(buf);
		
		int monsterId = packet->id;
		int pId = packet->playerId;
		unordered_set<int> old_viewList = _objects[id].GetViewList();

		for (auto& users : old_viewList) {
			if (_objects[users]._status != ST_ACTIVE) continue;
			if (true == IsPlayer(users)) {
				_sender->SendMonsterDirPacket(_objects[id].GetSocket(), pId, monsterId, packet->vRot);
			}

		}
		break;
	}
	case CS_PACKET_DUNGEON: {
		cs_packet_dungeon* packet = reinterpret_cast<cs_packet_dungeon*>(buf);
		unordered_set<int> dun_vl;
		unordered_set<int> old_viewList = _objects[id].GetViewList();
		//unordered_set<int> temp_vl = _objects[id].GetViewList();
		_enterRoom.insert(id);
		
		for (int i = 0; i < _enterRoom.size(); ++i) {
			if (id == i) break;
		}

		if (_enterRoom.size() != _acceptNumber) {
			for (auto& users : _enterRoom)
				_sender->Send_WaitRoom_Packet(_objects[users].GetSocket(),_enterRoom.size());
		}
		else {
			fullEnter = true;

			_objects[id].ClearViewList();
			_objects[id].SetDunGeonEnter(true);

			

			for (auto& users : old_viewList) {
				_objects[users].SetDunGeonEnter(true);
				if (users == id) continue;
				if (false == IsPlayer(users)) continue;
				_objects[users].ClientLock();
				_objects[users].EraseViewList(id);
				_objects[users].ClientUnLock();
				_sender->SendLeaveObjectPacket(_objects[users].GetSocket(), id, _objects[id].GetMyType());
			}

			DungeonEnter(id);
		}

		break;
	}
	case CS_PACKET_DIETEST: {
		cs_packet_dietest* packet = reinterpret_cast<cs_packet_dietest*>(buf);
		int playerId = packet->id;
		_objects[playerId].SetCurrentHp(0);

		break;
	}
	case CS_PACKET_SKILL: {
		cs_packet_skill* packet = reinterpret_cast<cs_packet_skill*>(buf);

		int playerId = packet->id;

		switch (packet->anitype) {
		case PLAYER_ANI_TYPE::SKILL_1: {
			for (int i = 0; i < _acceptNumber; ++i) {
				if (_objects[i]._status != ST_ACTIVE) continue;
				if (playerId == i) continue;
				_sender->SendSkillPacket(_objects[i].GetSocket(), playerId, packet->anitype,packet->isSkill);
			}

			break;
		}
		case PLAYER_ANI_TYPE::SKILL_2: {
			for (int i = 0; i < _acceptNumber; ++i) {
				if (_objects[i]._status != ST_ACTIVE) continue;
				if (playerId == i) continue;
				_sender->SendSkillPacket(_objects[i].GetSocket(), playerId, packet->anitype, packet->isSkill);
			}

			break;
		}
		default:
			cout << "스킬 없슴" << endl;
		}



		break;
	}
	case CS_PACKET_M2PCOL:
	{
		cs_packet_m2p* packet = reinterpret_cast<cs_packet_m2p*>(buf);
		int player_id = packet->playerId;
		_objects[player_id].SetCurrentHp(_objects[player_id].GetCurrentHp() - 100.f);

		cout << "플레이어 체력 : " << _objects[player_id].GetCurrentHp() << endl;

		break;
	}
	default:
		printf("Unknown PACKET type [%d]\n", buf[1]);
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

	_acceptNumber -= 1;
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
					_acceptNumber++;
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

		case EV_MONSTER_MOVE:
			AggroMove(id);
			delete exp_over;
			break;
		case EV_ATTACK:
			_objects[id].SetIsAttack(false);
			break;
		case EV_DUNGEON_ENTER: {




			break;
		}
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

			if (_timerQueue.top().wakeup_time > std::chrono::system_clock::now()) {
				_timerLock.unlock();
				break;
			}

			EVENT ev = _timerQueue.top();
			_timerQueue.pop();
			_timerLock.unlock();

			switch (ev.event_type) {
			case EV_MONSTER_MOVE: {
				EXP_OVER* exp_over = new EXP_OVER;
				exp_over->event_type = ev.event_type;
				PostQueuedCompletionStatus(_iocp, 1, ev.obj_id, &exp_over->over);

				break;
			}
			case EV_ATTACK: {
				EXP_OVER* exp_over = new EXP_OVER;
				exp_over->event_type = ev.event_type;
				PostQueuedCompletionStatus(_iocp, 1, ev.obj_id, &exp_over->over);
				break;
			}
			case EV_DUNGEON_ENTER:{
				EXP_OVER* exp_over = new EXP_OVER;
				exp_over->event_type = ev.event_type;
				PostQueuedCompletionStatus(_iocp, 1, ev.obj_id, &exp_over->over);
				break;
			}
			}

		}
	}


}

void CServerFrame::AddTimer(int obj_id ,EV_TYPE ev_type, system_clock::time_point t)
{
	EVENT ev{ obj_id, t, ev_type };
	_timerLock.lock();
	_timerQueue.push(ev);
	_timerLock.unlock();
}

void CServerFrame::ActivateNPC(int id)
{
	_objects[id].SetMoveType(TARGET);
	if (ST_SLEEP == _objects[id]._status) {
		if (true == CAS(&_objects[id]._status, ST_SLEEP, ST_ACTIVE))
			AddTimer(id, EV_MONSTER_MOVE, system_clock::now() + 1s);
	}

}

void CServerFrame::AggroMove(int npc_id)
{
	int player_id = _objects[npc_id].GetTargetID();
	//cout << "TargetID : " << player_id << endl;
	if (ST_ACTIVE != _objects[npc_id]._status) return;
	if (ST_ACTIVE != _objects[player_id]._status) return;
	if (RANDOM == _objects[npc_id].GetMoveType()) return;

	if (true == IsPlayer(npc_id)) {
		std::cout << "ID :" << npc_id << " is not NPC!!\n";
		while (true);
	}

	Vec3 A_vPos = _objects[player_id].GetPos();
	Vec3 B_vPos = _objects[npc_id].GetPos();

	float distance = Vec3::Distance(A_vPos, B_vPos);
	bool closed;

	if (distance < 200.f) _objects[npc_id]._closed = true;			// 사거리에 들어옴
	else { _objects[npc_id]._closed = false; _objects[npc_id]._move = true; }							// 사거리에 들어오지 않음 -> 몬스터가 움직여야겠지?

	float speed = _objects[npc_id].GetSpeed();

	Vec3 Pos = _objects[npc_id].GetPos();
	Vec3 prev_pos = Pos;
	Vec3 dir = _objects[player_id].GetPos() - Pos;
	Vec3 up = Vec3(0.f, 1.f, 0.f);

	Vec3 Forward = dir.Normalize();
	prev_pos = Pos + (Forward * speed * _elapsedTime.count());
	bool col = false;
	vector<int>	M_A_B;

	if (_objects[npc_id]._closed == false) {
		for (int i = NPC_ID_START; i < NPC_ID_END; ++i)
		{
			if (_objects[i]._status == ST_SLEEP) continue;
			if (npc_id != i) {
				if (Vec3::Distance(prev_pos, _objects[i].GetPos()) < 150.f) {
					col = true;
				}
			}
		}
		if (!col)
		{
			_objects[npc_id].SetPos(prev_pos);
		}
		else {
			Vec3 Right = cross(Forward, up);
			Vec3 Left = -Right;
			Vec3 Back = -Forward;

			Vec3 Forward_Right = Forward + Right;	Forward_Right.Normalize();
			Vec3 Forward_Left = Forward + Left;		Forward_Left.Normalize();
			Vec3 Back_Right = Back + Right;			Back_Right.Normalize();
			Vec3 Back_Left = Back + Left;			Back_Left.Normalize();

			vector<Vec3> Monster_dir{ Forward, Back, Right, Left, Forward_Right, Forward_Left, Back_Right, Back_Left };
			map<float, Vec3> nearlist_map;

			for (int i = NPC_ID_START; i < NPC_ID_END; ++i)
			{
				if (_objects[i]._status == ST_SLEEP) continue;
				for (auto dir : Monster_dir)
				{
					prev_pos = Pos + (dir * speed * _elapsedTime.count());

					if (Vec3::Distance(prev_pos, _objects[i].GetPos()) < 150.f) continue;
					else {
						nearlist_map.try_emplace(Vec3::Distance(prev_pos, _objects[i].GetPos()), prev_pos);
					}
				}
			}
			_objects[npc_id].SetPos(nearlist_map.begin()->second);
		}
	}
	//_objects[npc_id].SetLook(Forward);

	if (_objects[npc_id]._closed == true) {						// 몬스터 공격범위
		if (false == _objects[npc_id].GetIsAttack()) {
			_objects[npc_id].SetIsAttack(true);
			_objects[npc_id]._move = false;
			
			AddTimer(npc_id, EV_ATTACK, system_clock::now() + 2s);
			
			/*_objects[player_id].SetCurrentHp(_objects[player_id].GetCurrentHp() - _objects[npc_id].GetDamage());
			_sender->SendHpPacket(_objects[player_id].GetSocket(), _objects[player_id].GetCurrentHp());*/

			if (0 >= _objects[player_id].GetCurrentHp() && !_objects[player_id]._objectsDie) {
				cout<<"아이디 : " << player_id << " 님 사망" << endl;
				
				ComeBackScene(player_id);
			}

			for (auto& cl : _objects) {
				if (false == IsPlayer(cl.GetID())) continue;
				if (false == IsNear(cl.GetID(), npc_id)) continue;
				if (cl._objectsDie == true) continue;
				cl.ClientLock();
				if (ST_ACTIVE == cl._status) {
					_sender->SendNPCAttackPacket(cl.GetSocket(), npc_id, Pos.x, Pos.z, true);
				}
				cl.ClientUnLock();
			}
		}
		
	}

	for (int i = 0; i < NPC_ID_START; ++i) {
		if (ST_ACTIVE != _objects[i]._status) continue;
		if (_objects[npc_id]._move == false) continue;
		if (_objects[i]._objectsDie == true) continue;
		if (true == IsNear(i, npc_id)) {
			_objects[i].ClientLock();
			if (0 != _objects[i].DungeonGetViewListCount(npc_id)) {
				_objects[i].ClientUnLock();
				if (_objects[npc_id]._closed == false) {					// 사거리에 들어오지 않음 
					_objects[npc_id]._move = true;

					_sender->SendMovePacket(_objects[i].GetSocket(), npc_id, _objects[npc_id].GetPos(),
						_objects[npc_id].GetLook().x, _objects[npc_id].GetLook().y, _objects[npc_id].GetLook().z, true // true
						,std::chrono::system_clock::now());
				}
				else {
					_objects[npc_id]._move = false;

					_sender->SendMovePacket(_objects[i].GetSocket(), npc_id, _objects[npc_id].GetPos(),
						_objects[npc_id].GetLook().x, _objects[npc_id].GetLook().y, _objects[npc_id].GetLook().z, false // false
						,std::chrono::system_clock::now());
				}
			}
			else {
				_objects[i].DungeonInsertViewList(npc_id);
				_objects[i].ClientUnLock();
				cout << "혹시 이곳 어그로무브? 1 " << endl;
				_sender->SendPutObjectPacket(_objects[i].GetSocket(), npc_id, _objects[npc_id].GetPos().x,
					_objects[npc_id].GetPos().y, _objects[npc_id].GetPos().z,
					_objects[npc_id].GetMyType());
			}
		}
		//else {
		//	_objects[i].ClientLock();
		//	if (0 != _objects[i].DungeonGetViewListCount(npc_id)) {
		//		_objects[i].DungeonEraseViewList(npc_id);
		//		//CAS(&_objects[npc_id]._status, ST_ACTIVE, ST_SLEEP);
		//		_objects[i].ClientUnLock();
		//		cout << "혹시 어그로무브 리브? 2" << endl;
		//		_sender->SendLeaveObjectPacket(_objects[i].GetSocket(), npc_id, _objects[npc_id].GetMyType());
		//	}
		//	else {
		//		_objects[i].ClientUnLock();
		//	}
		//}

	}
	
	for (int i = 0; i < NPC_ID_START; ++i) {
		if (ST_ACTIVE != _objects[i]._status) continue;
		if (_objects[i]._objectsDie) continue;
		if (true == IsNear(npc_id, i)) {
			AddTimer(npc_id, EV_MONSTER_MOVE, system_clock::now() + 200ms);
			return;
			
		}
	}
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

	if (distance > 300.f) {
		return false;

	}
	return true;
}

bool CServerFrame::IsNear(int a, int b)
{

	if (abs(_objects[a].GetPos().x - _objects[b].GetPos().x) > VIEW_RADIUS) return false;
	if (abs(_objects[a].GetPos().y - _objects[b].GetPos().y) > VIEW_RADIUS) return false;
	if (abs(_objects[a].GetPos().z - _objects[b].GetPos().z) > VIEW_RADIUS) return false;

	return true;
}

void CServerFrame::Do_move(const short& id, const char& dir, Vec3& localPos, const float& rotate)
{
	
	unordered_set<int> vl = _objects[id].GetViewList();
	time_point<system_clock> curTime = system_clock::now();

	_objects[id].ClientLock();

	std::unordered_set<int> oldViewList = _objects[id].GetViewList();
	_objects[id].ClientUnLock();
	std::unordered_set<int> newViewList;

	/*if (_objects[id].GetPos().z >= 310.f && !_objects[id]._questStart) {
		cout << "퀘스트 시작 패킷" << endl;
		_objects[id]._questStart = true;
		_sender->SendQuestStartPacket(_objects[id].GetSocket(), id, true);
	}*/

	if (_objects[id].GetPos().z >= 100.f && !_objects[id]._questStart) {
		cout << "퀘스트 시작 패킷" << endl;
		_objects[id]._questStart = true;
		_sender->SendQuestStartPacket(_objects[id].GetSocket(), id, true);
	}


	for (auto& cl : _objects) {
		if (false == IsNear(cl.GetID(), id)) continue;
		if (ST_ACTIVE != cl._status) continue;
		if (cl.GetID() == id) continue;
		if (cl.GetDunGeonEnter()) continue;
		if (true == IsPlayer(cl.GetID()))
			newViewList.insert(cl.GetID());

	}

	for (auto& np : newViewList) {
		if (0 == oldViewList.count(np)) {	// Object가 시야에 새로 들어왔을 때.
			//if (_objects[id]._objectsDie == true) continue;
			_objects[id].ClientLock();
			_objects[id].InsertViewList(np);
			_objects[id].ClientUnLock();
			cout << "do_move 풋오브 1" << endl;
			_sender->SendPutObjectPacket(_objects[id].GetSocket(), np,
				_objects[np].GetPos().x, _objects[np].GetPos().y, _objects[np].GetPos().z, 
				_objects[np].GetMyType());

			if (false == IsPlayer(np)) continue;
			_objects[np].ClientLock();
			if (0 == _objects[np].GetViewListCount(id)) {
				_objects[np].InsertViewList(id);
				_objects[np].ClientUnLock();
				//cout << "do_move 2" << endl;
				cout << "do_move 풋오브 2" << endl;

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
				//cout << "do_move 3" << endl;
				cout << "do_move 풋오브 3" << endl;

				_sender->SendPutObjectPacket(_objects[np].GetSocket(), id, 
					_objects[id].GetPos().x, _objects[id].GetPos().y, _objects[id].GetPos().z, 
					_objects[id].GetMyType());
			}
		}
	}


	for (auto& op : oldViewList) {		// Object가 시야에서 벗어났을 때.
		if (0 == newViewList.count(op)) {
			_objects[id].ClientLock();
			_objects[id].EraseViewList(op);
			_objects[id].ClientUnLock();
			//if (_objects[id]._objectsDie) continue;
			cout << "혹시 두 무브 리브? 1" << endl;
			_sender->SendLeaveObjectPacket(_objects[id].GetSocket(), op, _objects[op].GetMyType());
			//std::atomic_compare_exchange_strong(&_objects[id]._status, &oldState, ST_ACTIVE);
			if (false == IsPlayer(op)) continue;
			_objects[op].ClientLock();
			if (0 != _objects[op].GetViewListCount(id)) {
				_objects[op].EraseViewList(id);
				_objects[op].ClientUnLock();
				cout << "혹시 두 무브 리브? 2" << endl;
				_sender->SendLeaveObjectPacket(_objects[op].GetSocket(), id, _objects[id].GetMyType());
			}
			else {
				_objects[op].ClientUnLock();
			}
		}
	}

	_elapsedTime = curTime - _prevTime;
	_prevTime = curTime;
}
void CServerFrame::Do_move_Dungeon(const short& id, const char& dir, Vec3& localPos, const float& rotate)
{
	unordered_set<int> vl = _objects[id].DungeonGetViewList();
	time_point<system_clock> curTime = system_clock::now();

	// 몬스터 관련 패킷
	for (const int& npc : vl){
		if (true == IsPlayer(npc)) continue;
		if (ST_ACTIVE != _objects[npc]._status) continue;
		if (true == IsNearNPC(id, npc)) {
			_objects[npc].SetTargetID(id);
			_objects[npc].SetTarget(true);
		}
		_sender->SendTargetPlayerPacket(_objects[id].GetSocket(), _objects[npc].GetTargetID(), true, npc);
	}

	_objects[id].ClientLock();

	std::unordered_set<int> oldViewList = _objects[id].DungeonGetViewList();
	_objects[id].ClientUnLock();
	std::unordered_set<int> newViewList;
	/*
	if (_objects[id].GetPos().z >= 100.f && !_objects[id]._questStart && fullEnter) {
		cout << "퀘스트 시작 패킷" << endl;
		_objects[id]._questStart = true;
		_sender->SendQuestStartPacket(_objects[id].GetSocket(), id, true);
	}*/

	for (auto& cl : _objects) {

		if (false == IsNear(cl.GetID(), id)) {
			continue;
		};
		if (ST_SLEEP == cl._status && fullEnter) {
			ActivateNPC(cl.GetID());
		}
		if (!cl.GetDunGeonEnter()) continue;
		if (ST_ACTIVE != cl._status) continue;
		if (cl.GetID() == id) continue;
		newViewList.insert(cl.GetID());
	}

	for (auto& np : newViewList) {
		if (0 == oldViewList.count(np)) {	// Object가 시야에 새로 들어왔을 때.
		//	if (_objects[id]._objectsDie == true) continue;
			_objects[id].ClientLock();
			_objects[id].DungeonInsertViewList(np);
			_objects[id].ClientUnLock();
			cout << "do_move 던전 풋오브 1" << endl;

			_sender->SendPutObjectPacket(_objects[id].GetSocket(), np,
				_objects[np].GetPos().x, _objects[np].GetPos().y, _objects[np].GetPos().z,
				_objects[np].GetMyType());

			if (false == IsPlayer(np)) continue;

			_objects[np].ClientLock();
			if (0 == _objects[np].DungeonGetViewListCount(id)) {
				_objects[np].DungeonInsertViewList(id);
				_objects[np].ClientUnLock();
				cout << "do_move 던전 풋오브 2" << endl;

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
			if (0 != _objects[np].DungeonGetViewListCount(id)) {
				_objects[np].ClientUnLock();
				_sender->SendMovePacket(_objects[np].GetSocket(), id, _objects[id].GetPos(),
					_objects[id].GetLook().x, _objects[id].GetLook().y, _objects[id].GetLook().z, true,
					std::chrono::system_clock::now());
			}
			else {
				_objects[np].ClientUnLock();
				_objects[np].DungeonInsertViewList(id);
				cout << "do_move 던전 풋오브 3" << endl;

				_sender->SendPutObjectPacket(_objects[np].GetSocket(), id,
					_objects[id].GetPos().x, _objects[id].GetPos().y, _objects[id].GetPos().z,
					_objects[id].GetMyType());
			}
		}
	}


	//for (auto& op : oldViewList) {		// Object가 시야에서 벗어났을 때.
	//	if (0 == newViewList.count(op)) {
	//		_objects[id].ClientLock();
	//		_objects[id].DungeonEraseViewList(op);
	//		_objects[id].ClientUnLock();
	//		cout << "혹시 두 무브 던전 리브? 1" << endl;
	//		_sender->SendLeaveObjectPacket(_objects[id].GetSocket(), op, _objects[op].GetMyType());
	//		if (false == IsPlayer(op)) continue;
	//		_objects[op].ClientLock();
	//		if (0 != _objects[op].DungeonGetViewListCount(id)) {
	//			_objects[op].DungeonEraseViewList(id);
	//			_objects[op].ClientUnLock();
	//			cout << "혹시 두 무브 던전 리브? 2" << endl;
	//			_sender->SendLeaveObjectPacket(_objects[op].GetSocket(), id, _objects[id].GetMyType());
	//		}
	//		else {
	//			_objects[op].ClientUnLock();
	//		}
	//	}
	//}

	_elapsedTime = curTime - _prevTime;

	_prevTime = curTime;
}

void CServerFrame::Do_stop(const short& id, const bool& isMoving)
{
	
	unordered_set<int> temp_vl;
	if (_objects[id].GetDunGeonEnter()) {
		temp_vl = _objects[id].DungeonGetViewList();
	}
	else {
		temp_vl = _objects[id].GetViewList();
	}
	unordered_set<int> old_viewList = temp_vl;
    

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
		_objects[id].GetPos().x,_objects[id].GetPos().y,_objects[id].GetPos().z,
		_objects[id].GetDamage(), _objects[id].GetCurrentHp(),
		_objects[id].GetMaxHp(), _objects[id].GetLevel(),
		_objects[id].GetCurrentExp(), _objects[id].GetMaxExp());

	_objects[id]._status = ST_ACTIVE;
	_objects[id].ClientUnLock();

	for (int i = 0; i < NPC_ID_START;++i) {
		if (id == i) continue;
		if (true == IsNear(id, i)) {
			if (ST_ACTIVE == _objects[i]._status) {
				_objects[id].ClientLock();
				_objects[id].InsertViewList(i);
				_objects[id].ClientUnLock();

				_sender->SendPutObjectPacket(_objects[id].GetSocket(), i, _objects[i].GetPos().x,
					_objects[i].GetPos().y, _objects[i].GetPos().z, _objects[i].GetMyType());
				if (true == IsPlayer(i)) {
					_objects[i].ClientLock();
					_objects[i].InsertViewList(id);
					_objects[i].ClientUnLock();

					_sender->SendPutObjectPacket(_objects[i].GetSocket(), id, _objects[id].GetPos().x,
						_objects[id].GetPos().y, _objects[id].GetPos().z, _objects[id].GetMyType());
				}
			}
		}
	}


}
void CServerFrame::DungeonEnter(int id)
{

	_objects[id].ClientLock();
	_objects[id].SetPos(Vec3(0.f, 0.f, 0.f));
	for (auto& users : _enterRoom)
		_sender->SendDungeonEnterPacket(_objects[users].GetSocket(), users, true);

	_objects[id].ClientUnLock();

	for (int i = 0; i < _enterRoom.size(); ++i) {
		if (id == i) continue;
		if (true == IsNear(id, i)) {

			if (ST_ACTIVE == _objects[i]._status) {

				_objects[id].ClientLock();
				_objects[id].DungeonInsertViewList(i);
				_objects[id].ClientUnLock();
				_sender->SendPutObjectPacket(_objects[id].GetSocket(), i, _objects[i].GetPos().x,
					_objects[i].GetPos().y, _objects[i].GetPos().z, _objects[i].GetMyType());

				if (true == IsPlayer(i)) {
					_objects[i].ClientLock();
					_objects[i].DungeonInsertViewList(id);
					_objects[i].ClientUnLock();

					_sender->SendPutObjectPacket(_objects[i].GetSocket(), id, _objects[id].GetPos().x,
						_objects[id].GetPos().y, _objects[id].GetPos().z, _objects[id].GetMyType());

				}
			}
		}
	}


	for (int i = 0; i < _enterRoom.size(); ++i) {
		_sender->SendQuestDonePacket(_objects[i].GetSocket(), i, QUEST::SECOND, true);
	}

}

void CServerFrame::ComeBackScene(int player_id)
{
	cout << "집결지로 복귀합니다." << endl;
	_objects[player_id].ClientLock();
	_objects[player_id].SetCurrentHp(2000.f);
	
	
	/*short level = _objects[player_id].GetLevel();
	short hp = _objects[player_id].GetCurrentHp();
	short changeHp = hp - (50 * level);*/
	//if (0 > changeHp) changeHp = 0;
	/*_objects[player_id].SetCurrentExp(changeHp);
	_objects[player_id].SetCurrentHp(50);*/

	_objects[player_id].SetPos(Vec3(0.f, 0.f, 300.f));
	_objects[player_id].SetDunGeonEnter(false);
	_objects[player_id].ClearViewList();

	std::unordered_set<int> vl = _objects[player_id].DungeonGetViewList();

	for (const auto& id : vl) {
		if (false == IsPlayer(id)) continue;
		
		_sender->SendPlayerDiePacket(_objects[id].GetSocket(), player_id);
		
		if (id != player_id)
		{
			_objects[id].DungeonEraseViewList(player_id);
			_sender->SendLeaveObjectPacket(_objects[id].GetSocket(), player_id, _objects[player_id].GetMyType());
		}
		//_sender->SendLeaveObjectPacket(_objects[player_id].GetSocket(), player_id, _objects[id].GetMyType());



	}

	_objects[player_id].DungeonClearViewList();
	_sender->SendPlayerDiePacket(_objects[player_id].GetSocket(), player_id);

	_objects[player_id]._objectsDie = true;
	_objects[player_id].ClientUnLock();

	for (int i = 0; i < _acceptNumber; ++i) {
		if (false == IsPlayer(i)) continue;
		if (player_id == i) continue;
		if (true == IsNear(player_id, i))
		{
			if (true == _objects[i]._objectsDie)
			{
				_objects[player_id].ClientLock();
				_objects[player_id].InsertViewList(i);
				_objects[player_id].ClientUnLock();
				cout << "컴백홈" << endl;

				_sender->SendPutObjectPacket(_objects[player_id].GetSocket(), i, _objects[i].GetPos().x,
					_objects[i].GetPos().y, _objects[i].GetPos().z, _objects[i].GetMyType());

				if (true == IsPlayer(i))
				{
					_objects[i].ClientLock();
					_objects[i].InsertViewList(player_id);
					_objects[i].ClientUnLock();

					_sender->SendPutObjectPacket(_objects[i].GetSocket(), player_id, _objects[player_id].GetPos().x,
						_objects[player_id].GetPos().y, _objects[player_id].GetPos().z, _objects[player_id].GetMyType());
				}
			}

		}
	}
}

void CServerFrame::QuestDone(const short& id)
{

	
}

bool CServerFrame::CAS(volatile atomic<STATUS>* addr, STATUS expected, STATUS new_val)
{
	return atomic_compare_exchange_strong(reinterpret_cast<volatile atomic<STATUS>*>(addr), &expected, new_val);
}

void CServerFrame::CreateMonster()
{
	cout << "Initializing Monster" << endl;

	for (int monsterId = NPC_ID_START; monsterId < MONSTER_LV1_ID; ++monsterId) {
		_objects[monsterId].SetID(monsterId);
		_objects[monsterId]._status = ST_SLEEP;
		_objects[monsterId].SetSpeed(MONSTER_SPEED);
		_objects[monsterId].SetMoveType(RANDOM);
		_objects[monsterId].SetIsAttack(false);
		_objects[monsterId].SetDunGeonEnter(true);
		//_objects[monsterId].SetNextPosIndex(0);

	}
	for (int monsterId = NPC_ID_START; monsterId < MONSTER_LV1_ID; ++monsterId) {

		_objects[monsterId].SetCurrentHp(LV1_MONSTER_HP);
		_objects[monsterId].SetMaxHp(LV1_MONSTER_HP);
		_objects[monsterId].SetLevel(1);
		_objects[monsterId].SetDamage(_objects[monsterId].GetLevel() * 10);

	}

	/*for (int monsterId = MONSTER_LV1_ID; monsterId < MONSTER_LV2_ID; ++monsterId) {

		_objects[monsterId].SetCurrentHp(LV2_MONSTER_HP);
		_objects[monsterId].SetMaxHp(LV2_MONSTER_HP);
		_objects[monsterId].SetLevel(2);
		_objects[monsterId].SetDamage(_objects[monsterId].GetLevel() * 10);

	}
	for (int monsterId = MONSTER_LV2_ID; monsterId < MONSTER_LV3_ID; ++monsterId) {
		_objects[monsterId].SetCurrentHp(LV3_MONSTER_HP);
		_objects[monsterId].SetMaxHp(LV3_MONSTER_HP);
		_objects[monsterId].SetLevel(3);
		_objects[monsterId].SetDamage(_objects[monsterId].GetLevel() * 10);
	}
	for (int monsterId = MONSTER_LV3_ID; monsterId < MONSTER_LV4_ID; ++monsterId) {
		_objects[monsterId].SetCurrentHp(LV4_MONSTER_HP);
		_objects[monsterId].SetMaxHp(LV4_MONSTER_HP);
		_objects[monsterId].SetLevel(4);
		_objects[monsterId].SetDamage(_objects[monsterId].GetLevel() * 10);
	}
	for (int monsterId = MONSTER_LV4_ID; monsterId < MONSTER_LV5_ID; ++monsterId) {
		_objects[monsterId].SetCurrentHp(LV5_MONSTER_HP);
		_objects[monsterId].SetMaxHp(LV5_MONSTER_HP);
		_objects[monsterId].SetLevel(5);
		_objects[monsterId].SetDamage(_objects[monsterId].GetLevel() * 10);
	}
	for (int monsterId = MONSTER_LV5_ID; monsterId < MONSTER_LV6_ID; ++monsterId) {
		_objects[monsterId].SetCurrentHp(LV6_MONSTER_HP);
		_objects[monsterId].SetMaxHp(LV6_MONSTER_HP);
		_objects[monsterId].SetLevel(6);
		_objects[monsterId].SetDamage(_objects[monsterId].GetLevel() * 10);
	}*/

	// LV1
	_objects[NPC_ID_START].SetPos(Vec3(150.f, 0.f, 1200.f));
	//_objects[NPC_ID_START + 1].SetPos(Vec3(-120.f, 0.f, 1600.f));
	//_objects[NPC_ID_START + 2].SetPos(Vec3(-150.f, 0.f, 2050.f));
	//_objects[NPC_ID_START + 3].SetPos(Vec3(160.f, 0.f, 2300.f));
	//
	//_objects[NPC_ID_START + 4].SetPos(Vec3(-2250.f, 0.f, 3450.f));
	//_objects[NPC_ID_START + 5].SetPos(Vec3(-2800.f, 0.f, 3750.f));
	//_objects[NPC_ID_START + 6].SetPos(Vec3(-3000.f, 0.f, 3450.f));
	//_objects[NPC_ID_START + 7].SetPos(Vec3(-3450.f, 0.f, 3600.f));

	//// 중앙 홀 몬스터

	//// LV2
	//_objects[NPC_ID_START + 5].SetPos(Vec3(-400.f, 0.f, 4500.f));
	//_objects[NPC_ID_START + 6].SetPos(Vec3(-200.f, 0.f, 4500.f));
	//_objects[NPC_ID_START + 7].SetPos(Vec3(200.f, 0.f, 4500.f));
	//_objects[NPC_ID_START + 8].SetPos(Vec3(400.f, 0.f, 4500.f));
	//_objects[NPC_ID_START + 9].SetPos(Vec3(-600.f, 0.f, 4500.f));
	//_objects[NPC_ID_START + 10].SetPos(Vec3(600.f, 0.f, 4500.f));
	//_objects[NPC_ID_START + 11].SetPos(Vec3(800.f, 0.f, 4500.f));
	//_objects[NPC_ID_START + 12].SetPos(Vec3(-200.f, 0.f, 4700.f));
	//_objects[NPC_ID_START + 13].SetPos(Vec3(-400.f, 0.f, 4700.f));
	//_objects[NPC_ID_START + 14].SetPos(Vec3(200.f, 0.f, 4700.f));
	//_objects[NPC_ID_START + 15].SetPos(Vec3(400.f, 0.f, 4700.f));
	//
	////오른쪽 미로 몬스터
	////중앙 몬스터
	//// LV3
	//_objects[NPC_ID_START + 16].SetPos(Vec3(4000.f, 0.f, 10800.f));
	//_objects[NPC_ID_START + 17].SetPos(Vec3(3800.f, 0.f, 11000.f));
	//_objects[NPC_ID_START + 18].SetPos(Vec3(3800.f, 0.f, 11200.f));
	//_objects[NPC_ID_START + 19].SetPos(Vec3(3800.f, 0.f, 10600.f));
	//_objects[NPC_ID_START + 20].SetPos(Vec3(3800.f, 0.f, 10400.f));
	//
	//
	//// 북쪽 몬스터
	//// 중앙 몬스터
	//// LV5
	//_objects[NPC_ID_START + 21].SetPos(Vec3(0.f, 0.f, 16700.f));
	//_objects[NPC_ID_START + 22].SetPos(Vec3(-200.f, 0.f, 16400.f));
	//_objects[NPC_ID_START + 23].SetPos(Vec3(-400.f, 0.f, 16400.f));
	//_objects[NPC_ID_START + 24].SetPos(Vec3(200.f, 0.f, 16400.f));
	//_objects[NPC_ID_START + 25].SetPos(Vec3(400.f, 0.f, 16400.f));
	//_objects[NPC_ID_START + 26].SetPos(Vec3(-600.f, 0.f, 16400.f));
	//_objects[NPC_ID_START + 27].SetPos(Vec3(600.f, 0.f, 16400.f));
	//
	//// 북쪽 왼쪽 통로 몬스터
	//// LV4
	//_objects[NPC_ID_START + 28].SetPos(Vec3(-1700.f, 0.f, 13600.f));
	//_objects[NPC_ID_START + 29].SetPos(Vec3(-1300.f, 0.f, 13400.f));
	//_objects[NPC_ID_START + 30].SetPos(Vec3(-1500.f, 0.f, 13400.f));
	//_objects[NPC_ID_START + 31].SetPos(Vec3(-1300.f, 0.f, 13800.f));
	//_objects[NPC_ID_START + 32].SetPos(Vec3(-1500.f, 0.f, 13800.f));
	//
	//// 좌측 끝 몬스터
	//// LV6
	//_objects[NPC_ID_START + 33].SetPos(Vec3(-6300.f, 0.f, 13500.f));
	//_objects[NPC_ID_START + 34].SetPos(Vec3(-6100.f, 0.f, 13300.f));
	//_objects[NPC_ID_START + 35].SetPos(Vec3(-6100.f, 0.f, 13100.f));
	//_objects[NPC_ID_START + 36].SetPos(Vec3(-6100.f, 0.f, 13700.f));
	//_objects[NPC_ID_START + 37].SetPos(Vec3(-6100.f, 0.f, 13900.f));
	//_objects[NPC_ID_START + 38].SetPos(Vec3(-6100.f, 0.f, 14100.f));
	//_objects[NPC_ID_START + 39].SetPos(Vec3(-6100.f, 0.f, 12900.f));


	// 보스 몬스터 아이디 141
	//_objects[NPC_ID_START + 40].SetPos(Vec3(0, 0.f, 5000.f));
	//_objects[NPC_ID_START + 40].SetID(NPC_ID_START + 40);
	//_objects[NPC_ID_START + 40]._status = ST_SLEEP;
	//_objects[NPC_ID_START + 40].SetSpeed(2000.f);
	//_objects[NPC_ID_START + 40].SetMoveType(RANDOM);
	//_objects[NPC_ID_START + 40].SetIsAttack(false);
	//_objects[NPC_ID_START + 40].SetDunGeonEnter(true);

	printf("Monster Initialization finished.\n");
}