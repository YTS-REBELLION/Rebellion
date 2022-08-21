#pragma once
#include "../Client/Server.h"
#include "Mesh.h"
#include "Ptr.h"
#include "GameObject.h"

class CGameObject;
class CScene;
class CMesh;
class CToolCamScript;

struct EXOVER
{
	WSAOVERLAPPED	over;
	char			io_buf[MAX_BUFFER];
	union {
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
	int id;
};

enum class OBJECT_TYPE { PLAYER, M_MONSTER, FM_MONSTER, BOSS, NPC };


class CNetwork
{
private:

public:
	CNetwork();
	~CNetwork() {};
public:
	void SetObj(CGameObject* obj) { m_pObj = new CGameObject; m_pObj = obj; }
	CGameObject* GetObj() { return m_pObj; }
	void D_SetObj(CGameObject* obj) { m_pDObj = new CGameObject; m_pObj = obj; }
	//void SetCamObj(CGameObject* obj) { m_pCamObj = new CGameObject; m_pCamObj = obj; }
	void SetAniData(Ptr<CMesh> _aniData) { m_aniData.push_back(_aniData); }


public:
	void Connect();

public:
	void Receive();
	void ProcessPacket(char* ptr);
	void Process_Data(char* net_buf, size_t& io_byte);
	//void Enter_Player(const int& id);

public:
	void Send_Packet(void* packet);
	void Send_LogIn_Packet();
	void Send_Move_Packet(unsigned const char& dir, const Vec3& localPos, const Vec3& dirVec,
		const float& rotate, const system_clock::time_point& startTime,
		const float& delta, const bool& movings);
	void Send_Move_Packet(const Vec3& localPos, const Vec3& dirVec,
		const float& rotate, const system_clock::time_point& startTime,
		const float& delta);
	void Send_Stop_Packet(const bool& isMoving, const short& id);
	void Send_Rotate_Packet(const int& id, const Vec3& rotate);
	void Send_Attack_Animation_Packet(const int& id, const bool& isAttack);
	void Send_Run_Packet(const int& id, Vec3 pos, const bool& isRun);
	void Send_MonsterDie_Packet(const int& id, const bool& isDead);
	void Send_Player2MonsterCol_Packet(const int& id, const int& playerid, const bool& iscol, int attackType);
	void SetAnimation(int id, int i);
	void Send_MonsterRotate_Packet(const int& id, const int& other_id, Vec3 vRot);
	void Send_Dungeon_Packet(bool isEnter);
	void Send_bossmap_Packet(bool isEnter);
	void Send_PlayerDieTest_Packet(const int& id);
	void Send_Mon2Player_Packet(const int& playerId, bool isCol);


	void Send_MobToMobCol_Packet(const int& monsterId, const int& other_monsterId, bool isCol, MONSTER_MOVE moveType);
	void Send_Teleport_Packet(const int& playerId, Vec3 localPos);
	void Send_Skill_Packet(const int& playerId, PLAYER_ANI_TYPE anitype, bool isSkill);
public:
	void err_quit(const char* msg);
private:
	OVERLAPPED		_overlapped;
	CGameObject* m_pObj;
	CGameObject* m_pDObj;

	vector<Ptr<CMesh>>	m_aniData;
	int d_myid;
	int monsterdie = 0;
	friend class CLayer;
	bool isBossCreate = false;
};


