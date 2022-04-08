#include "pch.h"
#include "DataBase.h"

constexpr auto LEN = 10;

CDataBase::CDataBase() {

}
CDataBase::~CDataBase() {

}

void ShowError() {
	printf("error\n");
}

void CDataBase::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode) 
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];

	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
		// Hide data truncated.. 
		if (wcsncmp(wszState, L"01004", 5)) {
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
	//while (true);
}

int CDataBase::CheckID(int id, int pw)
{
	setlocale(LC_ALL, "korean");
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;

	SQLINTEGER nCheck;
	SQLLEN cbCheck;

	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"drakken_master", SQL_NTS, (SQLWCHAR*)NULL, SQL_NTS, NULL, SQL_NTS);

				// Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);		// 명령어 받을 핸들 할당. hstmt.

					SQLWCHAR query[1024];
					//wsprintf(query, L"UPDATE player_table SET c_px = %d, c_py = %d WHERE c_key = %d", x, y, keyid);
					wsprintf(query, L"EXEC check_id %d", id);
					retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

						retcode = SQLBindCol(hstmt, 1, SQL_C_LONG, &nCheck, LEN, &cbCheck);	// SQL_C_LONG == SQL_C_INTEGER

					}
					for (int i = 0; ; ++i) {
						retcode = SQLFetch(hstmt);
						if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) ShowError();
						if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
							//printf("DB 성공: %d\n", nLEVEL);
						}
						else {
							HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
							break;
						}
					}

					// Process data  
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						SQLCancel(hstmt);
						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}

					SQLDisconnect(hdbc);
				}

				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}

	//std::cout << nCheck << std::endl;
	if (0 != nCheck) {
		int tmp = CheckPW(id, pw);

		if (tmp != pw) nCheck = 0;
	}

	return nCheck;
}


int CDataBase::CheckPW(int id, int pw)
{
	setlocale(LC_ALL, "korean");
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;

	SQLINTEGER nCheck;
	SQLLEN cbCheck;

	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"drakken_master", SQL_NTS, (SQLWCHAR*)NULL, SQL_NTS, NULL, SQL_NTS);

				// Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);		// 명령어 받을 핸들 할당. hstmt.

					SQLWCHAR query[1024];
					//wsprintf(query, L"UPDATE player_table SET c_px = %d, c_py = %d WHERE c_key = %d", x, y, keyid);
					wsprintf(query, L"EXEC Check_pw %d", id);
					retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

						retcode = SQLBindCol(hstmt, 1, SQL_C_LONG, &nCheck, LEN, &cbCheck);	// SQL_C_LONG == SQL_C_INTEGER

					}
					for (int i = 0; ; ++i) {
						retcode = SQLFetch(hstmt);
						if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) ShowError();
						if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
							//printf("DB 성공: %d\n", nLEVEL);
						}
						else {
							HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
							break;
						}
					}

					// Process data  
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						SQLCancel(hstmt);
						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}

					SQLDisconnect(hdbc);
				}

				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}

	return nCheck;
}


void CDataBase::SavePlayerData(char* name, short exp, short hp)
{
	setlocale(LC_ALL, "korean");
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;

	int id = atoi(name);

	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"drakken_master", SQL_NTS, (SQLWCHAR*)NULL, SQL_NTS, NULL, SQL_NTS);

				// Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);		// 명령어 받을 핸들 할당. hstmt.

					SQLWCHAR query[1024];
					wsprintf(query, L"UPDATE User_Table SET user_level = %d, user_exp = %d, user_hp = %d WHERE c_id = %d", level, hp, exp, id);
					retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						printf("DB 저장 성공, ID : %d\n", id);
					}
					else {
						HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
					}

					// Process data  
					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						SQLCancel(hstmt);
						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					}

					SQLDisconnect(hdbc);
				}

				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
}