#pragma once

#include "CObject.h"

class CError;
class CSender;


class CServerFrame {
	// socket ���� ����ü
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
	CObject		_objects[141-35];



	// timer
	std::mutex											_timerLock;
	std::priority_queue<EVENT>							_timerQueue;
	std::chrono::time_point <std::chrono::system_clock> _prevTime;
	std::chrono::duration<float>						_elapsedTime;

	bool _useDB;

	int _dummpyIndex;
	
	OBSTACLE _obstacles[NUM_OBSTACLES];

	

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


	void Disconnect(int);

	void ActivateNPC(int id);
	void MoveUpdate();
	
	//
	void AggroMove(int id);
	//

	void DoRandomMove(int id);
	void DoTargetMove(int id);
	void UpdatePlayerPos(int id);
	void SetMoveDirection(int id, char direction, bool b);
	void Do_move(const short& id, const char& dir, Vec3& localPos, const float& rotate);
	void Do_stop(const short& id, const bool& isMoving);
	bool IsPlayer(int id);
	bool IsNearNPC(int player, int npc);
	bool IsNear(int a, int b);
};

