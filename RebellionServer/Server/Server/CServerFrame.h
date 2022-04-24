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
	CObject		_objects[10000];



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
	void DoWorker();
	void DoTimer();
	void RecvPacketProcess(int, int);
	void AddTimer(EVENT ev);
	void ProcessPacket(int , char*);

	void Disconnect(int);

	void ActivateNPC(int id);
	void MoveUpdate();
	
	void DoRandomMove(int id);
	void DoTargetMove(int id);
	void UpdatePlayerPos(int id);

	bool IsPlayer(int id);
	bool IsNearNPC(int player, int npc);
	bool IsNear(int a, int b);
};

