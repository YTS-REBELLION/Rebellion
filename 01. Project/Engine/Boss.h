#pragma once
#include "Script.h"
#include "Mesh.h"
class CPlayerScript;
class CBossScript :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;
	Vec3				m_vecPlayerDir;
	float				m_fSpeed = 100.f;

	bool				m_Is_Move;
	bool				m_Is_Attack;
	bool				m_bHit;

	bool				m_bAniOk;

	sc_packet_move* m_movePacketTemp = nullptr;
	sc_packet_npc_attack* m_attackPacket = nullptr;
	float				m_vecAniClipTime[5]{};
	int					m_iCulidx;

	bool				FirstPacket = false;

	bool				isDir = false;
	int					m_id;
	bool				m_isMain;

	int					m_targetId;
	bool				m_isTarget;

	bool				m_colEnter = false;

	float				m_fMaxHp = 1200.f;
	float				m_fHp = 0.f;

	vector<tMTAnimClip>			m_pVecAnimClip;

	Vec3 Monster_Dir;
	Vec3 Monster_Nor;
	float m_fAngle;

	Vec3 LerpPos;

	CCollider2D* m_pColObj;
	bool		m_bColCheck = false;
	CGameObject* pMonsterColObject = nullptr;
	CGameObject* pSwordColObject = nullptr;

public:
	void init();
	virtual void awake();
	virtual void update();

	void SetBossAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame);
	void SetBossAnimation(const int& i);
	void SetBossAnimation(int other_id, const int& i);
	void AnimationPlay(const Boss_ANI_TYPE& type);
	void AnimationPlay(int other_id, const Boss_ANI_TYPE& type);
	Ptr<CMesh> GetAniData(const int& type) { return m_pAniData[(int)type]; }

	void SetMain() { m_isMain = true; };

	void SetOtherMovePacket(sc_packet_move* p);
	void SetBisFrist(const bool& bis) { FirstPacket = bis; }
	void SetOtherMovePacket__IsMoving(const bool& isMoving) {
		if (m_movePacketTemp != nullptr) m_movePacketTemp->isMoving = isMoving;
	}
	void SetDirChange(const bool& bis) { isDir = bis; }
	void SetHP(const int& hp) { m_fHp = hp; }
	void SetMonsterPacket(sc_packet_npc_attack* p);
	void SetMonsterAttackPacket(const bool& isAttack) {
		if (m_attackPacket != nullptr) m_attackPacket->isAttack = isAttack;
	}

	void SetMove(bool _Is_Move) { m_Is_Move = _Is_Move; }
	void SetAttack(bool _Is_Attack) { m_Is_Attack = _Is_Attack; }

	void SetTargetID(const int& targetID) { m_targetId = targetID; }
	void SetTarget(const bool& isTarget) { m_isTarget = isTarget; }
	void SetNor(Vec3 nor) { Monster_Dir = nor; }

	int GetID() { return m_id; }
	void SetID(const int& id) { m_id = id; }

	CGameObject* m_pPlayer;

	void SetLerpPos(Vec3 Pos) { LerpPos = Pos; }
	Vec3 GetLerpPos() { return LerpPos; }
	void UpdateLerpPos();

	void SetHit() {
		if (m_bHit) m_bHit = false;
		else m_bHit = true;
	}
	bool GetHit() const { return m_bHit; }

	void SetColObj(CCollider2D* _obj) { m_pColObj = _obj; }
	void SetColMonster(CGameObject* _obj) { pMonsterColObject = _obj; }
	CGameObject* GetColMonster() { return pMonsterColObject; }

	void SetColSSA(CGameObject* _obj) { pSwordColObject = _obj; }
	CGameObject* GetColSSA() { return pSwordColObject; }
public:
	CLONE(CBossScript);

public:
	CBossScript();
	virtual ~CBossScript();

public:
	bool				m_bIsActive = false;
	bool GetIsHide() { return m_bIsActive; }
	Vec3 GetPlayerDir() { return m_vecPlayerDir; }
public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
};

