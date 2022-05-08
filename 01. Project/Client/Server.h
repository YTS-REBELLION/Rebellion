#pragma once

#include <windows.h>

#include <WS2tcpip.h>
#include <MSWSock.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <stdio.h>

#include <unordered_map>


#pragma comment(lib,"WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

using namespace std;

using namespace chrono;

class Vector3;
class Matrix;

#include "../../RebellionServer/Server/Server/Protocol.h"
