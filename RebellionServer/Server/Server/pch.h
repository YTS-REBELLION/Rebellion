// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
// 기본적으로 쓰는 것들
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <thread>
#include <queue>
#include <mutex>

#include <unordered_set>
#include <atomic>

#include <cstring>


using namespace std;
using namespace chrono;

#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma comment (lib, "MSWSock.LIB")
#pragma comment(lib, "lua54.lib")
#pragma comment(lib,"WS2_32.lib")

//extern "C" {
//#include "lua.h"
//#include "lauxlib.h"
//#include "lualib.h"
//}
//

#include <d3d12.h>
#include "SimpleMath.h"
#pragma comment(lib, "d3d12")
typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;
using DirectX::SimpleMath::Matrix;

#include "../../../01. Project/Engine/define.h"
#include "Protocol.h"
#include "Shared.h"


#include "func.h"

#endif //PCH_H
