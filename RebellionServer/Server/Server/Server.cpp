// Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "CServerFrame.h"

CServerFrame g_server;


int main()
{
    std::wcout.imbue(locale("Korean"));
    std::cout << "Start Server Progress\n";
    g_server.Progress();
}

