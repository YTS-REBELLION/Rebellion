#include "stdafx.h"
#include "Network.h"
#include "GameObject.h"
#include "Scene.h"
//#include "SceneMgr.h"

#include "Layer.h"
#include "Camera.h"

#include "Transform.h"
#include "MeshRender.h"
//#include "Collider.h"     

#include "PlayerScript.h"
#include "SwordScript.h"
#include "ToolCamScript.h"
#include "MonsterScript.h"




OBJECT_TYPE CheckType(const short& id)
{
	if (id >= 0 && id < MAX_USER) return OBJECT_TYPE::PLAYER;
	else if (id >= NPC_ID_START && NPC_ID_START + 100) return OBJECT_TYPE::MONSTER;
}


CNetwork g_net;
const char* SERVER_IP = "127.0.0.1";
//const char* SERVER_IP = "121.190.132.161";
OBJ GameObject;

SOCKET g_socket;
int packetTest;
int g_myid = -1;


void CNetwork::err_quit(const char* msg)
{
	LPVOID lpmsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpmsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgBuf);
	exit(1);
}


CNetwork::CNetwork()
{
	m_pObj = nullptr;
}


void CNetwork::Connect()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return;

	g_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ULONG l = 1;

	int ret = ioctlsocket(g_socket, FIONBIO, (unsigned long*)&l);

	if (g_socket == INVALID_SOCKET)
		err_quit("WSASocket");
	SOCKADDR_IN recvAddr;
	ZeroMemory(&recvAddr, 0, sizeof(recvAddr));

	recvAddr.sin_family = AF_INET;
	//recvAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	inet_pton(AF_INET, SERVER_IP, &(recvAddr.sin_addr));
	
	recvAddr.sin_port = htons(SERVER_PORT);

	if (connect(g_socket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			int i = 0;
		}
		else
		{
			int i = 0;
		}
	}

	std::cout << "connect" << std::endl;

	//if (ret == SOCKET_ERROR)
	//	err_quit("connect error");

	WSAEVENT event = WSACreateEvent();
	ZeroMemory(&_overlapped, sizeof(_overlapped));


	_overlapped.hEvent = event;


}

void CNetwork::Receive()
{
	
	EXOVER*	 dataBuf = new EXOVER{};
	DWORD	 recvByte = 0;
	DWORD	 flags = 0;
	dataBuf->over = _overlapped;
	char recvbuf[100] = "";
	int fg = 0;

	int ret = recv(g_socket, recvbuf, sizeof(recvbuf), fg);
	if (ret <= 0)
		return;

	size_t retbytesize = ret;


	if (WSAGetLastError() == WSAEWOULDBLOCK)
	{
		//pass
	}
	
	Process_Data(recvbuf, retbytesize);
	


}

void CNetwork::ProcessPacket(char* ptr)
{

	//std::cout << "Process Packet" << std::endl;
	switch (ptr[1]) {
		//case 로그인 패킷:

	case SC_PACKET_LOGIN_OK:
	{

		sc_packet_login_ok* p = reinterpret_cast<sc_packet_login_ok*>(ptr);
		std::cout << "플레이어 아이디 : " << p->id << std::endl;
		std::cout << "플레이어 데미지 : " << p->damage << std::endl;
		std::cout << "플레이어 체력 " << p->c_hp << " / " << p->m_hp << std::endl;
		std::cout << "플레이어 레벨 : " << p->level << std::endl;
		std::cout << "플레이어 경험치 : " << p->c_exp << " / " << p->m_exp << std::endl;
		std::cout << "플레이어의 위치 x : " << p->x << ", z : " << p->z << endl;

		g_myid = p->id;
		m_pObj->Transform()->SetLocalPos(Vec3(p->x, p->y, p->z));

		GameObject.emplace(g_myid, m_pObj);
		GameObject.find(g_myid)->second->SetID(g_myid);

		GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetMain();
		//GameObject.find(g_myid)->second->//GetScript<CPlayerScript>()->SetMain();
		/*GameObject.emplace(g_myid, m_pObj);

		GameObject.find(g_myid)->second->SetID(g_myid);*/


		break;

	}

	case SC_PACKET_LOGIN_FAIL: {
		std::cout << "로그인 페일" << std::endl;

		exit(0);
		break;

	}
	case SC_PACKET_PUT_OBJECT: {
		sc_packet_put_object* packet = reinterpret_cast<sc_packet_put_object*>(ptr);
		int id = packet->id;

		//if (id == g_myid) {
		//	//내꺼 만들기
		//}
		//else {
		if (CheckType(id) == OBJECT_TYPE::PLAYER) {
			// 다른 사람꺼


			Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Idle.mdat", L"MeshData\\Player_Idle.mdat");

			CGameObject* pObject = new CGameObject;
			GameObject.emplace(id, pObject);
			GameObject.find(id)->second->SetID(id);


			GameObject.find(id)->second = pMeshData->Instantiate();
			GameObject.find(id)->second->SetName(L"Player1");
			GameObject.find(id)->second->FrustumCheck(false);
			GameObject.find(id)->second->Transform()->SetLocalPos(Vec3(packet->x, packet->y, packet->z));
			GameObject.find(id)->second->Transform()->SetLocalScale(Vec3(0.08f, 0.08f, 0.08f));
			GameObject.find(id)->second->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
			GameObject.find(id)->second->AddComponent(new CCollider2D);
			//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH);

			GameObject.find(id)->second->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
			GameObject.find(id)->second->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
			GameObject.find(id)->second->Collider2D()->SetOffsetScale(Vec3(800.f, 850.f, 1700.f));
			
			// 플레이어 스크립트 붙여주기.
			GameObject.find(id)->second->AddComponent(new CPlayerScript);

			//CPlayerScript* PlayerScript = GameObject.find(id)->second->GetScript<CPlayerScript>();
			//// 플레이어 애니메이션
			//PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 0
			//SetAniData(pMeshData->GetMesh());

			////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Walk.fbx");
			////pMeshData->Save(pMeshData->GetPath());
			//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Walk.mdat", L"MeshData\\Player_Walk.mdat");
			//PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 1
			//SetAniData(pMeshData->GetMesh());

			////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Run.fbx");
			////pMeshData->Save(pMeshData->GetPath());
			//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Run.mdat", L"MeshData\\Player_Run.mdat");
			//PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 2
			//SetAniData(pMeshData->GetMesh());

			////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Attack.fbx");
			////pMeshData->Save(pMeshData->GetPath());
			//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Attack.mdat", L"MeshData\\Player_Attack.mdat");
			//PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 3
			//SetAniData(pMeshData->GetMesh());


			CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", GameObject.find(id)->second, false);
			

			for (auto& data : m_aniData)
			{
				GameObject.find(id)->second->GetScript<CPlayerScript>()->GetPlayerAnimation(data);
			}
			// 
			// 플레이어 스크립트 붙여주기.
			//pObject->AddComponent(new CPlayerScript);
			//CPlayerScript* PlayerScript = pObject->GetScript<CPlayerScript>();
			/*GameObject.find(id)->second->AddComponent(new CPlayerScript);
			GameObject.find(id)->second->GetScript<CPlayerScript>();*/

			//CGameObject* pSwordObject = nullptr;
			//Ptr<CMeshData> pSwordMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\asdq.mdat", L"MeshData\\asdq.mdat");
			//Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");

			//pSwordObject = pSwordMeshData->Instantiate();
			//pSwordObject->SetName(L"Player_Sword");
			//pSwordObject->FrustumCheck(false);
			//pSwordObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
			//pSwordObject->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
			//pSwordObject->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(-90.f), XMConvertToRadians(-90.f)));
			//pSwordObject->AddComponent(new CCollider2D);
			//pSwordObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
			//pSwordObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
			//pSwordObject->Collider2D()->SetOffsetScale(Vec3(10.f, 140.f, 5.f));

			//Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");
			//pSwordObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, SwordObject.GetPointer());

			//pSwordObject->AddComponent(new CSwordScript);
			//CSwordScript* SwordScript = GameObject.find(id)->second->GetScript<CSwordScript>();
			//pSwordObject->GetScript<CSwordScript>()->SetTarget(GameObject.find(id)->second);
			//pSwordObject->GetScript<CSwordScript>()->SetBoneIdx(36);
			////pSwordObject->GetScript<CSwordScript>()->SetBoneIdx2(48);

			//CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", pSwordObject, false);
			//GameObject.find(id)->second->AddChild(pSwordObject);
		}

		else if (CheckType(id) == OBJECT_TYPE::MONSTER) {
			// 몬스터

					// Monster 객체 생성
			CGameObject* pMonster = new CGameObject;
			Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Sword And Shield Idle (2).mdat", L"MeshData\\Sword And Shield Idle (2).mdat");
			//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Sword And Shield Idle (2).fbx");
			//pMeshData->Save(pMeshData->GetPath());
			GameObject.emplace(id, pMonster);
			GameObject.find(id)->second->SetID(id);

			GameObject.find(id)->second = pMeshData->Instantiate();
			GameObject.find(id)->second->SetName(L"Monster1");
			GameObject.find(id)->second->FrustumCheck(false);
			GameObject.find(id)->second->Transform()->SetLocalPos(Vec3(packet->x, packet->y, packet->z));
			GameObject.find(id)->second->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
			GameObject.find(id)->second->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
			GameObject.find(id)->second->AddComponent(new CCollider2D);
			GameObject.find(id)->second->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
			GameObject.find(id)->second->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
			GameObject.find(id)->second->Collider2D()->SetOffsetScale(Vec3(20.f, 20.f, 40.f));

			// 플레이어 스크립트 붙여주기.
			GameObject.find(id)->second->AddComponent(new CMonsterScript);

			CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", GameObject.find(id)->second, false);

			/*for (auto& data : m_aniData)
			{
				GameObject.find(id)->second->GetScript<CMonsterScript>()->GetPlayerAnimation(data);
			}*/

			CMonsterScript* MonsterScript = GameObject.find(id)->second->GetScript<CMonsterScript>();
			// 플레이어 애니메이션
			MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 0
			SetAniData(pMeshData->GetMesh());

			//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Walk.fbx");
			//pMeshData->Save(pMeshData->GetPath());
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Big Hit To Head", L"MeshData\\Big Hit To Head.mdat");
			MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 1
			SetAniData(pMeshData->GetMesh());

			//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Shoulder Hit And Fall.fbx");
			//pMeshData->Save(pMeshData->GetPath());
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Shoulder Hit And Fall", L"MeshData\\Shoulder Hit And Fall.mdat");
			MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 2
			SetAniData(pMeshData->GetMesh());

			
			GameObject.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
			GameObject.find(id)->second->GetScript<CMonsterScript>()->SetHP(100);
		}

		break;

	}
	case SC_PACKET_LEAVE_OBJECT: {
		sc_packet_leave* packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int id = packet->id;
		if (CheckType(id) == OBJECT_TYPE::MONSTER) {
			GameObject.find(id)->second->GetScript<CMonsterScript>()->GetObj()->SetDead();
			GameObject.erase(id);

		}
		else if (CheckType(id) == OBJECT_TYPE::PLAYER) {
			GameObject.find(id)->second->GetScript<CPlayerScript>()->GetObj()->SetDead();
			GameObject.erase(id);
		}
		break;


	}
	case SC_PACKET_MOVE: {
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = packet->id;


		if (other_id == g_myid)
		{
			if (packet->status)
				GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(other_id, 1);
			GameObject.find(other_id)->second->Transform()->SetLocalPos(packet->localPos);
		}
		else
		{ 

			if (CheckType(other_id) == OBJECT_TYPE::PLAYER)
			{
				GameObject.find(other_id)->second->Transform()->SetLocalPos(packet->localPos);
				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->SetBisFrist(true);
				GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(true);

				if (packet->status)
					GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(other_id, 1);	//SetPlayerAnimation(other_id, 1);
				/*else
					GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(other_id, 0);*/

				GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetOtherMovePacket(packet, 1 * 0.00000001);
			
			}
			else if (CheckType(other_id) == OBJECT_TYPE::MONSTER)
			{
				cout << "클라이언트 몬스터 무브" << endl;
				cout << "서버에서 보낸 몬스터 packet move 좌표" << endl;
				cout << packet->localPos.x << ", " << packet->localPos.z << endl;
				GameObject.find(other_id)->second->Transform()->SetLocalPos(packet->localPos);

				GameObject.find(other_id)->second->GetScript<CMonsterScript>()->SetBisFrist(true);
				GameObject.find(other_id)->second->GetScript<CMonsterScript>()->SetOtherMovePacket__IsMoving(true);
				/*if (packet->status)
					GameObject.find(other_id)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(other_id, 1);
				*/
				GameObject.find(other_id)->second->GetScript<CMonsterScript>()->SetOtherMovePacket(packet, 1 * 0.00000001);

			}
		}
		//break;
		break;
	}
	case SC_PACKET_STOP: {

		sc_packet_stop* packet = reinterpret_cast<sc_packet_stop*>(ptr);
		int other_id = packet->id;
		if (CheckType(other_id) == OBJECT_TYPE::PLAYER) {
			if (other_id == g_myid)
			{
				// 혹시나 해서 하는 SetPlayerAnimation g_myid가 맞음
				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(g_myid, 0);


				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(packet->isMoving);


			}
			else
			{
				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(other_id, 0);



				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(packet->isMoving);

			}
		}
	}
	break;
	case SC_PACKET_ROTATE: {
		sc_packet_rotate* packet = reinterpret_cast<sc_packet_rotate*>(ptr);



		GameObject.find(packet->id)->second->Transform()->SetLocalRot(packet->rotate);
		break;
	}
	case SC_PACKET_PLAYER_ATTACK: {
		cout << "SC_PACKET_PLAYER_ATTACK" << endl;
		sc_packet_player_attack* packet = reinterpret_cast<sc_packet_player_attack*>(ptr);
		int id = packet->id;
		if (id == g_myid) {

		}
		else {
			if (packet->isAttack) {
				GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(id, 3);
			}
			else {
				GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(id, 0);

			}
		}

		break;
	}
	case SC_PACKET_RUN: {
		cout << "SC_PACKET_RUN" << endl;
		sc_packet_run* packet = reinterpret_cast<sc_packet_run*>(ptr);
		int id = packet->id;
		if (id == g_myid) {

		}
		else {
			if (packet->isRun) {
				GameObject.find(id)->second->Transform()->SetLocalPos(packet->pos);

				GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(id, 2);
			}
			else {

				GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetPlayerAnimation(id, 0);
			}
		}
		break;
	}
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void CNetwork::Process_Data(char* net_buf, size_t& io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[1024];


	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;


		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}


}

void CNetwork::Send_Packet(void* _packet)
{
	EXOVER dataBuf;
	char* packet = reinterpret_cast<char*>(_packet);
	size_t sent;
	DWORD flag;
	dataBuf.wsabuf.len = packet[0];
	dataBuf.wsabuf.buf = (char*)packet;
	dataBuf.over = _overlapped;

	packetTest = dataBuf.wsabuf.len;

	//std::cout << "Packet Send : " << packetTest << std::endl;


	//if (WSASend(g_Socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) == SOCKET_ERROR)
	if (WSASend(g_socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			WSAWaitForMultipleEvents(1, &_overlapped.hEvent, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(g_socket, &_overlapped, (LPDWORD)&sent, FALSE, &flag);
		}
		else
			err_quit("WSASend");
	}

}

void CNetwork::Send_LogIn_Packet()
{

	cs_packet_login packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_LOGIN;
	
	char name[MAX_ID_LEN] = "Tester";
	string namestring;
	
	//cout << name << endl;
	//strcpy_s(name, packet.name);



	std::cout << "name : ";
	std::cin >> name;

	

	sprintf_s(packet.name, name);
	strcpy_s(name, packet.name);
	g_myid++;

	std::cout << "CS_LOGIN PACKET : " << g_myid << std::endl;

	Send_Packet(&packet);


}
void CNetwork::Send_Move_Packet(unsigned const char& dir, const Vec3& localPos, const Vec3& dirVec,
	const float& rotate, const system_clock::time_point& startTime,
	const float& delta, const bool& movings)
{
	cs_packet_move packet;

	packet.type = CS_PACKET_MOVE;
	packet.size = sizeof(packet);
	packet.direction = dir;

	packet.rotate = rotate;

	packet.start = startTime;
	packet.deltaTime = delta;
	packet.movings = movings;

	Send_Packet(&packet);



}

void CNetwork::Send_Move_Packet(const Vec3& localPos, const Vec3& dirVec, const float& rotate, const system_clock::time_point& startTime, const float& delta)
{

	cs_packet_move packet;

	packet.type = CS_PACKET_MOVE;
	packet.size = sizeof(packet);

	packet.localPos = localPos;
	packet.dirVec = dirVec;
	
	packet.rotate = rotate;

	packet.start = startTime;
	packet.deltaTime = delta;

	Send_Packet(&packet);
}

void CNetwork::Send_Stop_Packet(const bool& isMoving, const short& id)
{
	cout << "Send_Stop_Packet" << endl;
	cs_packet_stop packet;
	packet.type = CS_PACKET_STOP;
	packet.size = sizeof(packet);
	packet.id = id;
	packet.isMoving = isMoving;

	Send_Packet(&packet);


}

void CNetwork::Send_Rotate_Packet(const int& id, const Vec3& rotate)
{
	cs_packet_rotate packet;
	packet.type = CS_PACKET_ROTATE;
	packet.size = sizeof(packet);
	packet.id = id;
	packet.rotate = rotate;
	Send_Packet(&packet);
}

void CNetwork::Send_Attack_Animation_Packet(const int& id, const bool& isAttack)
{
	cs_packet_attack packet;
	packet.type = CS_PACKET_ATTACK;
	packet.size = sizeof(packet);
	packet.id = id;
	packet.isAttack = isAttack;

	Send_Packet(&packet);

}

void CNetwork::Send_Run_Packet(const int& id, Vec3 pos ,const bool& isRun)
{
	cs_packet_run packet;
	packet.type = CS_PACKET_RUN;
	packet.size = sizeof(packet);
	packet.id = id;
	packet.isRun = isRun;
	packet.pos = pos;
	Send_Packet(&packet);


}

