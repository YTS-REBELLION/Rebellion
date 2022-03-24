#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>


#include "CObject.h"

class CError;
class CSender;


class CServerFrame {
	// socket 관련 구조체
	SOCKET												_listenSocket;
	SOCKET												_clientSocket;
	SOCKADDR_IN											_clientAddr;
	DWORD												_flags;
	int													_addrLen;

	//thread
	std::thread											_timerThread;
	std::vector<std::thread>							_workerThread;
	HANDLE												_iocp;
	
	//class
	CError* _error;
	CSender* _sender;


	CObject												_objects[10000];



	// timer
	std::mutex											_timerLock;
	std::priority_queue<EVENT>							_timerQueue;
	std::chrono::time_point <std::chrono::system_clock> _prevTime;
	std::chrono::duration<float>						_elapsedTime;

	bool _useDB;

	int _dummpyIndex;
	

	

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

	void AddTimer(EVENT ev);





};

