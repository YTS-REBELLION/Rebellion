#pragma once
#include <sqlext.h>

class CDataBase
{
	SQLRETURN retcode;
	SQLINTEGER nHP, nEXP;
	SQLLEN cbHP, cbEXP;


public:
	CDataBase();
	~CDataBase();

public:
	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
	int CheckID(int id, int pw);
	int CheckPW(int id, int pw);
	void SavePlayerData(char* name, short exp, short hp);
	
};

