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
#include "M_MonsterScript.h"




OBJECT_TYPE CheckType(const short& id)
{
	if (id >= 0 && id < MAX_USER) return OBJECT_TYPE::PLAYER;
	else if (id >= NPC_ID_START && NPC_ID_START + 40) return OBJECT_TYPE::FM_MONSTER;
	else if(id >= NPC_ID_START + 40 && NPC_ID_START + 50) return OBJECT_TYPE::M_MONSTER;
}


CNetwork g_net;
//const char* SERVER_IP = "127.0.0.1";
//const char* SERVER_IP = "192.168.63.11";
//const char* SERVER_IP = "121.190.132.161";
const char* SERVER_IP = "172.20.10.2";
OBJ GameObject;

SOCKET g_socket;

int packetTest;
int g_myid = -1;
int targetId = -1;

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
		GameObject.find(g_myid)->second->GetScript<CPlayerScript>()->SetID(g_myid);

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


			Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Idle.mdat", L"MeshData\\Player_FM_Idle.mdat");

			CGameObject* pObject = new CGameObject;
			GameObject.emplace(id, pObject);
			GameObject.find(id)->second->SetID(id);

			GameObject.find(id)->second = pMeshData->Instantiate();
			GameObject.find(id)->second->SetName(L"Player1");
			GameObject.find(id)->second->FrustumCheck(false);
			GameObject.find(id)->second->Transform()->SetLocalPos(Vec3(packet->x, packet->y, packet->z));
			GameObject.find(id)->second->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			GameObject.find(id)->second->Transform()->SetLocalRot(Vec3(XMConvertToRadians(180.f), 0.f, 0.f));
			
			GameObject.find(id)->second->AddComponent(new CCollider2D);
			GameObject.find(id)->second->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
			GameObject.find(id)->second->Collider2D()->SetOffsetPos(Vec3(0.f, -100.f, 0.f));
			GameObject.find(id)->second->Collider2D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));

			// 플레이어 스크립트 붙여주기.
			GameObject.find(id)->second->AddComponent(new CPlayerScript);
			CPlayerScript* PlayerScript = GameObject.find(id)->second->GetScript<CPlayerScript>();

			GameObject.find(id)->second->GetScript<CPlayerScript>()->init();
			GameObject.find(id)->second->GetScript<CPlayerScript>()->SetID(id);
			//GameObject.find(id)->second->GetScript<CPlayerScript>()->SetTarget(false);

			PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 0, 0, 55);									// AniData Index 0

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Walk.mdat", L"MeshData\\Player_FM_Walk.mdat");
			PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 1, 0, 30);									// AniData Index 1

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Run.mdat", L"MeshData\\Player_FM_Run.mdat");
			PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 2, 0, 22);									// AniData Index 2

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Attack.mdat", L"MeshData\\Player_FM_Attack.mdat");
			PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 3, 0, 45);									// AniData Index 3

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Skill_1.mdat", L"MeshData\\Player_FM_Skill_1.mdat");
			PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 4, 0, 75);									// AniData Index 3


			CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", GameObject.find(id)->second, false);
		}

		else if (CheckType(id) == OBJECT_TYPE::FM_MONSTER) {
			//// 몬스터

			CGameObject* pMonster = new CGameObject;
			Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Idle.mdat", L"MeshData\\Monster_FM_Idle.mdat");
			GameObject.emplace(id, pMonster);
			GameObject.find(id)->second->SetID(id);
			GameObject.find(id)->second = pMeshData->Instantiate();
			GameObject.find(id)->second->SetName(L"FM_Monster");
			GameObject.find(id)->second->FrustumCheck(false);

			GameObject.find(id)->second->Transform()->SetLocalPos(Vec3(packet->x, packet->y, packet->z));
			GameObject.find(id)->second->Transform()->SetLocalScale(Vec3(4.5f, 4.5f, 4.5f));
			GameObject.find(id)->second->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
			
			GameObject.find(id)->second->AddComponent(new CCollider2D);
			GameObject.find(id)->second->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
			GameObject.find(id)->second->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
			GameObject.find(id)->second->Collider2D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));
			GameObject.find(id)->second->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));

			// 몬스터 스크립트 붙여주기.
			GameObject.find(id)->second->AddComponent(new CMonsterScript);
			GameObject.find(id)->second->GetScript<CMonsterScript>()->SetID(id);

			CMonsterScript* MonsterScript = GameObject.find(id)->second->GetScript<CMonsterScript>();
			
			//몬스터 애니메이션
			MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 0, 0, 100);				
			
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Walk.mdat", L"MeshData\\Monster_FM_Walk.mdat");
			MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 1, 0, 29);								

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Hit.mdat", L"MeshData\\Monster_FM_Hit.mdat");
			MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 1, 0, 40);

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Attack.mdat", L"MeshData\\Monster_FM_Attack.mdat");
			MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 3, 0, 36);

			GameObject.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
			GameObject.find(id)->second->GetScript<CMonsterScript>()->SetHP(100);
			GameObject.find(id)->second->GetScript<CMonsterScript>()->SetLerpPos(Vec3(packet->x, packet->y, packet->z));

			CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", GameObject.find(id)->second, false);
		}

		else if (CheckType(id) == OBJECT_TYPE::M_MONSTER) {

			CGameObject* pM_Monster = new CGameObject;
			Ptr<CMeshData>pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Idle.mdat", L"MeshData\\Monster_M_Idle.mdat");
			GameObject.emplace(id, pM_Monster);
			GameObject.find(id)->second->SetID(id);
			GameObject.find(id)->second = pMeshData->Instantiate();
			GameObject.find(id)->second->SetName(L"M_Monster");
			GameObject.find(id)->second->FrustumCheck(false);

			GameObject.find(id)->second->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
			GameObject.find(id)->second->Transform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
			GameObject.find(id)->second->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, XMConvertToRadians(-90.f)));
			
			GameObject.find(id)->second->AddComponent(new CCollider2D);
			GameObject.find(id)->second->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
			GameObject.find(id)->second->Collider2D()->SetOffsetPos(Vec3(-120.f, 15.f, 0.f));
			GameObject.find(id)->second->Collider2D()->SetOffsetScale(Vec3(45.f, 35.f, 25.f));

			// 몬스터 스크립트 붙여주기.
			pM_Monster->AddComponent(new CM_MonsterScript);

			CM_MonsterScript* M_MonsterScript = pM_Monster->GetScript<CM_MonsterScript>();
			pM_Monster->GetScript<CM_MonsterScript>()->init();

			////몬스터 애니메이션
			M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 0, 0, 55);								// AniData Index 0

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Walk.mdat", L"MeshData\\Monster_M_Walk.mdat");
			M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 1, 0, 41);								// AniData Index 1

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Hit.mdat", L"MeshData\\Monster_M_Hit.mdat");
			M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 2, 0, 53);								// AniData Index 2

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Attack.mdat", L"MeshData\\Monster_M_Attack.mdat");
			M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 3, 0, 53);								// AniData Index 3

			CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", pM_Monster, false);
		}

		break;

	}
	case SC_PACKET_LEAVE_OBJECT: {
		sc_packet_leave* packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int id = packet->id;
		cout << "leave id : " << id << endl;
		if (CheckType(id) == OBJECT_TYPE::FM_MONSTER) {
			GameObject.find(id)->second->GetScript<CMonsterScript>()->GetObj()->SetDead();
			GameObject.erase(id);

		}
		else if (CheckType(id) == OBJECT_TYPE::M_MONSTER) {
			GameObject.find(id)->second->GetScript<CM_MonsterScript>()->GetObj()->SetDead();
			GameObject.erase(id);
		}
		else if (CheckType(id) == OBJECT_TYPE::PLAYER) {
			GameObject.find(id)->second->GetScript<CPlayerScript>()->GetObj()->SetDead();
			GameObject.erase(id);
		}
		break;


	}
	case SC_PACKET_MONSTERDIE: {
		sc_packet_monsterdie* packet = reinterpret_cast<sc_packet_monsterdie*>(ptr);
		int id = packet->id;
		if (CheckType(id) == OBJECT_TYPE::FM_MONSTER) {
			GameObject.find(id)->second->GetScript<CMonsterScript>()->GetObj()->SetDead();
		}
		else if (CheckType(id) == OBJECT_TYPE::M_MONSTER) {
			GameObject.find(id)->second->GetScript<CMonsterScript>()->GetObj()->SetDead();
		}
		break;
	}
	case SC_PACKET_MOVE: {
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = packet->id;

		if (other_id == g_myid)
		{
			if (packet->status)
				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->AnimationPlay(other_id, PLAYER_ANI_TYPE::WALK);
			cout << "현재 위치 x : " << packet->localPos.x << ", z : " << packet->localPos.z << endl;
			GameObject.find(other_id)->second->Transform()->SetLocalPos(packet->localPos);
		}
		else
		{ 
			if (CheckType(other_id) == OBJECT_TYPE::PLAYER)
			{
				cout << "현재 위치 x : " << packet->localPos.x << ", z : " << packet->localPos.z << endl;
				GameObject.find(other_id)->second->Transform()->SetLocalPos(packet->localPos);
				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->SetBisFrist(true);

				if (packet->status)
					GameObject.find(other_id)->second->GetScript<CPlayerScript>()->AnimationPlay(other_id, PLAYER_ANI_TYPE::WALK);			
			}
			else if (CheckType(other_id) == OBJECT_TYPE::FM_MONSTER)
			{

				//GameObject.find(other_id)->second->GetScript<CMonsterScript>()->SetDirChange(true);
				GameObject.find(other_id)->second->GetScript<CMonsterScript>()->SetLerpPos(packet->localPos);
				GameObject.find(other_id)->second->GetScript<CMonsterScript>()->SetMove(packet->status);
			}
			else if (CheckType(other_id) == OBJECT_TYPE::M_MONSTER)
			{
				GameObject.find(other_id)->second->GetScript<CM_MonsterScript>()->SetLerpPos(packet->localPos);
				GameObject.find(other_id)->second->GetScript<CM_MonsterScript>()->SetMove(packet->status);
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
				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->AnimationPlay(other_id, PLAYER_ANI_TYPE::IDLE);
			}
			else
			{

				GameObject.find(other_id)->second->GetScript<CPlayerScript>()->AnimationPlay(other_id, PLAYER_ANI_TYPE::IDLE);
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
				GameObject.find(id)->second->GetScript<CPlayerScript>()->AnimationPlay(id, PLAYER_ANI_TYPE::ATTACK);
			}
			else {
				GameObject.find(id)->second->GetScript<CPlayerScript>()->AnimationPlay(id, PLAYER_ANI_TYPE::IDLE);
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

				GameObject.find(id)->second->GetScript<CPlayerScript>()->AnimationPlay(id, PLAYER_ANI_TYPE::RUN);
			}
			else {
				GameObject.find(id)->second->GetScript<CPlayerScript>()->AnimationPlay(id, PLAYER_ANI_TYPE::IDLE);
			}
		}
		break;
	}
	case SC_PACKET_NPC_ATTACK: {
		cout << "SC_PACKET_NPC_ATTACK" << endl;
		sc_packet_npc_attack* packet = reinterpret_cast<sc_packet_npc_attack*>(ptr);
		int monsterId = packet->id;

		CMonsterScript* monsterScr = GameObject.find(monsterId)->second->GetScript<CMonsterScript>();
	
		//GameObject.find(monsterId)->second->GetScript<CMonsterScript>()->SetDirChange(false);
	
		GameObject.find(monsterId)->second->GetScript<CMonsterScript>()->SetMove(false);
		GameObject.find(monsterId)->second->GetScript<CMonsterScript>()->SetAttack(packet->isAttack);
		GameObject.find(monsterId)->second->GetScript<CMonsterScript>()->AnimationPlay(MONSTER_ANI_TYPE::ATTACK);

		break;
	}
	case SC_PACKET_TARGET: {
		sc_packet_targetplayer* packet = reinterpret_cast<sc_packet_targetplayer*>(ptr);
		// packet -> int targetId, bool isTarget


		GameObject.find(packet->monster_id)->second->GetScript<CMonsterScript>()->SetTarget(packet->isTarget);
		GameObject.find(packet->monster_id)->second->GetScript<CMonsterScript>()->SetTargetID(packet->id);
		
		break;
	}
	case SC_PACKET_QUESTDONE: {
		sc_packet_questdone* packet = reinterpret_cast<sc_packet_questdone*>(ptr);
		if (packet->isDone) {
			cout << "퀘스트 완료!" << endl;
			cout << "다음 퀘스트 고고 !" << endl;
		}
		//GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->QuestDone(QUEST::FIRST);
		switch (packet->nextQuest) {
		case QUEST::SECOND: {
			GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->SetQuestCnt(QUEST::SECOND);
			//GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->SetQuestView(packet->isDone);
			GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->QuestInit(QUEST::SECOND);
			
			break;
		}
		case QUEST::THIRD: {
			cout << "세번째 퀘스트 시작!" << endl;
			GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->SetQuestCnt(QUEST::THIRD);
			GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->QuestInit(QUEST::THIRD);
			break;
		}
		default: {
			cout << "하위" << endl;
			break;
		}
		}

		break;

	}
	case SC_PACKET_QUESTSTART: {
		sc_packet_queststart* packet = reinterpret_cast<sc_packet_queststart*>(ptr);
			
		cout << "클라이언트 퀘스트 시작하세요" << endl;
		GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->SetQuestCnt(QUEST::FIRST);
		//GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->SetQuestView(packet->isStart);
		GameObject.find(packet->id)->second->GetScript<CPlayerScript>()->QuestInit(QUEST::FIRST);
		
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

void CNetwork::Send_MonsterDie_Packet(const int& id, const bool& isDead)
{
	cs_packet_monsterdie packet;
	packet.type = CS_PACKET_MD;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.isDead = isDead;

	Send_Packet(&packet);

}

void CNetwork::Send_Player2MonsterCol_Packet(const int& id, const int& playerid, const bool& iscol)
{

	cs_packet_player2monstercol packet;
	packet.type = CS_PACKET_P2MCOL;
	packet.id = id;
	packet.playerId = playerid;
	packet.size = sizeof(packet);
	packet.iscol = iscol;

	Send_Packet(&packet);

}

void CNetwork::Send_MonsterRotate_Packet(const int& id, const int& other_id, Vec3 vRot)
{
	cs_packet_monsterdir packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.vRot = vRot;
	packet.playerId = other_id;
	packet.type = CS_PACKET_MONSTERDIR;

	Send_Packet(&packet);

}

