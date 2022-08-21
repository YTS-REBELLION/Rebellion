#pragma once
#pragma once
#include "Script.h"
#include "Mesh.h"
class CPlayerScript;
class CBoss :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;
	Vec3				m_vecPlayerDir;
	float				m_fSpeed = 200.f;

	bool				m_Is_Move;
	bool				m_Is_Attack;
	bool				m_bHit;

	bool				m_bAniOk;

	sc_packet_move* m_movePacketTemp = nullptr;
	sc_packet_npc_attack* m_attackPacket = nullptr;
	float				m_vecAniClipTime[5]{};
	int					m_iCulidx;

	bool				FirstPacket = false;

	int					m_targetId;
	bool				m_isTarget;

	int					m_id;
	bool				m_isMain;

	float				m_fMaxHp = 1200.f;
	float				m_fHp = 0.f;
	float	test = 0.f;

	bool				m_bColCheck = false;
	CCollider2D* m_pColObj;

	vector<tMTAnimClip>			m_pVecAnimClip;

	Vec3 Monster_Dir;
	Vec3 Monster_Nor;
	float m_fAngle;

	Vec3 LerpPos;
public:
	virtual void awake();
	virtual void update();

	void init();

	void SetMonsterAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame);
	void SetMonsterAnimation(const int& i);
	void SetMonsterAnimation(int other_id, const int& i);
	void AnimationPlay(const MONSTER_ANI_TYPE& type);

	Ptr<CMesh> GetAniData(const int& type) { return m_pAniData[(int)type]; }

	void SetMain() { m_isMain = true; };

	void SetOtherMovePacket(sc_packet_move* p);
	void SetBisFrist(const bool& bis) { FirstPacket = bis; }
	void SetOtherMovePacket__IsMoving(const bool& isMoving) {
		if (m_movePacketTemp != nullptr) m_movePacketTemp->isMoving = isMoving;
	}

	void SetHP(const int& hp) { m_fHp = hp; }
	void SetMonsterPacket(sc_packet_npc_attack* p);
	void SetMonsterAttackPacket(const bool& isAttack) {
		if (m_attackPacket != nullptr) m_attackPacket->isAttack = isAttack;
	}

	void SetMove(bool _Is_Move) { m_Is_Move = _Is_Move; }
	void SetAttack(bool _Is_Attack) { m_Is_Attack = _Is_Attack; }

	void SetNor(Vec3 nor) { Monster_Dir = nor; }

	int GetID() { return m_id; }
	void SetID(const int& id) { m_id = id; }

	CGameObject* m_pPlayer;
	void SetTargetID(const int& targetID) { m_targetId = targetID; }
	void SetTarget(const bool& isTarget) { m_isTarget = isTarget; }
	void SetLerpPos(Vec3 Pos) { LerpPos = Pos; }
	void UpdateLerpPos();


	void SetColObj(CCollider2D* _obj) { m_pColObj = _obj; }
public:
	CLONE(CBoss);

public:
	CBoss();
	virtual ~CBoss();

public:
	bool				m_bIsActive = false;
	bool GetIsHide() { return m_bIsActive; }
	Vec3 GetPlayerDir() { return m_vecPlayerDir; }
public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
};

