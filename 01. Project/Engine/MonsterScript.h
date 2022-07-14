#pragma once
#include "Script.h"
#include "Mesh.h"
class CPlayerScript;
class CMonsterScript :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;
	Vec3				m_vecPlayerDir;
	float				m_fSpeed = 200.f;
	bool				m_bAttack;
	bool				m_bHit;
	bool				m_bAniOk;

	sc_packet_move* m_movePacketTemp = nullptr;

	float				m_vecAniClipTime[5]{};
	int					m_iCulidx;

	bool				FirstPacket = false;
	

	int					m_id;
	bool				m_isMain;

	float				m_fMaxHp=1200.f;
	float				m_fHp=0.f;

	vector<tMTAnimClip>			m_pVecAnimClip;
public:
	virtual void awake();
	virtual void update();

	void SetMonsterAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame);
	void SetMonsterAnimation(const int& i);
	void SetMonsterAnimation(int other_id, const int& i);
	void AnimationPlay(const MONSTER_ANI_TYPE& type);

	Ptr<CMesh> GetAniData(const int& type) { return m_pAniData[(int)type]; }

	void SetMain() { m_isMain = true; };

	void SetOtherMovePacket(sc_packet_move* p, const float& rtt);
	void SetBisFrist(const bool& bis) { FirstPacket = bis; }
	void SetOtherMovePacket__IsMoving(const bool& isMoving) {
		if (m_movePacketTemp != nullptr) m_movePacketTemp->isMoving = isMoving;
	}
	void SetID(const int& id) { m_id = id; }
	void SetHP(const int& hp) { m_fHp = hp; }

	void SetAttack() {
		if (m_bAttack) {
			m_bAttack = false;
		}
		else m_bAttack = true;
	}
	bool GetAttack() { return m_bAttack; }

	void SetHit() {
		if (m_bAniOk) {
			m_bAniOk = false;
		}
		else m_bAniOk = true;
	}
	bool GetHit() { return m_bAniOk; }
	int GetID() { return m_id; }

	CPlayerScript* m_pPlayer = nullptr;
	

public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();

public:
	bool				m_bIsActive = false;
	bool GetIsHide() { return m_bIsActive; }
	Vec3 GetPlayerDir() { return m_vecPlayerDir; }
public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
};

