#include "pch.h"
#include "Error.h"


CError::CError()
{}

CError::~CError()
{}

void CError::error_display(const char* msg, int err_no) 
{
	//WCHAR* lpMsgBuf;
	//FormatMessage(
	//	FORMAT_MESSAGE_ALLOCATE_BUFFER |
	//	FORMAT_MESSAGE_FROM_SYSTEM,
	//	NULL, err_no,
	//	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	//	(LPTSTR)&lpMsgBuf, 0, NULL);
	//std::cout << msg;
	//std::wcout << L"¿¡·¯ " << lpMsgBuf << std::endl;
	///*DebugBreak();
	//while (true);*/
	//LocalFree(lpMsgBuf);
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	wcout << lpMsgBuf << endl;
	//while (true);
	LocalFree(lpMsgBuf);
}