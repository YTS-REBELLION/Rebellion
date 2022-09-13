#pragma once
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class CSoundMgr
{
private:
	CSoundMgr() {};
	~CSoundMgr() {};
public:
	static CSoundMgr* GetInst()
	{
		static CSoundMgr mgr;
		return &mgr;
	}
public:
	void Play(Sound_Type type, const bool& isasync = true, const bool& isloop = false);

};

