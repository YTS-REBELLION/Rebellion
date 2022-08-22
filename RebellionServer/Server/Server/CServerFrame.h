#pragma once

#include "CObject.h"

class CError;
class CSender;


class CServerFrame {


	// socket 관련 구조체
	SOCKET			_listenSocket;
	SOCKET			_clientSocket;
	SOCKADDR_IN		_clientAddr;
	DWORD			_flags;
	int				_addrLen;

	//thread
	std::thread					_timerThread;
	std::vector<std::thread>	_workerThread;
	HANDLE						_iocp;
	
	//class
	CError*		_error;
	CSender*	_sender;
	CObject		_objects[150];
	

	// timer
	std::mutex											_timerLock;
	std::priority_queue<EVENT>							_timerQueue;
	std::chrono::time_point <std::chrono::system_clock> _prevTime;
	std::chrono::duration<float>						_elapsedTime;
	unordered_set<int> _enterRoom;


	bool _useDB;

	int _dummpyIndex;
	
	OBSTACLE _obstacles[NUM_OBSTACLES];
	int _acceptNumber = 0;
	bool isQuestDone = false;
	bool isSecondQuestDone = false;
	bool isThirdQuestDone = false;


	vector<pair<int ,int>> col_id;
	//vector<int> col_id;
public:
	CServerFrame();
	~CServerFrame();

	void Progress();

	void InitServer();

	std::thread CreateWorkerThread();
	std::thread CreateTimerThread();

	void InitClients();
	void CreateMonster();
	void DoWorker();
	void DoTimer();
	void AddTimer(int obj_id, EV_TYPE ev_type, system_clock::time_point t);
	void RecvPacketProcess(int, int);
	void ProcessPacket(int , char*);
	void EnterGame(int id, const char* name);
	void DungeonEnter(int id);
	bool CAS(volatile atomic<STATUS>* addr, STATUS expected, STATUS new_val);



	void Disconnect(int);

	void ActivateNPC(int id);
	
	//
	void AggroMove(int id);
	//
	void Do_move(const short& id, const char& dir, Vec3& localPos, const float& rotate);
	void Do_stop(const short& id, const bool& isMoving);
	bool IsPlayer(int id);
	bool IsNearNPC(int player, int npc);
	bool IsNear(int a, int b);
	bool CollisionMonster(Vec3 A_vPos, Vec3 B_vPos);

	void TestCol(const short& monsterid, const short& other_monsterId);
	bool comp(pair<int, int> a, pair<int, int> b);

	void QuestDone(const short& id);

	void ComeBackScene(int id);
	void Do_move_Dungeon(const short& id, const char& dir, Vec3& localPos, const float& rotate);
	void Do_move_BossMap(const short& id, const char& dir, Vec3& localPos, const float& rotate);

	void BossMove(int id);
	int			monsterdieCnt = 0;
	bool		queststart = false;

	int _enterPlayer;

	bool fullEnter = false;
	int gameLevel = 1;
};

