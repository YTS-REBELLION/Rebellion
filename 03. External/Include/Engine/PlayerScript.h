#pragma once
#include "Script.h"
#include "Mesh.h"
#include "SwordStrike.h"
#include "DeadReckoning.h"

enum class COL_DIR
{
	RIGHT,
	UP,
	LEFT,
	DOWN,
	END,
};

class DeadReckoning;
class CPlayerScript :
	public CScript
{
public:
	

private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;
	Vec3				m_vecPlayerDir;
	float				m_fSpeed = PLAYER_SPEED;
	bool				m_bAttack;
	bool				m_bSkill;
	bool				m_bCol;
	sc_packet_move* m_movePacketTemp = nullptr;
	
	int					m_targetId;
	bool				m_isTarget;

	float				m_vecAniClipTime[5]{};
	int					m_iCulidx;
	bool				FirstPacket = false;
	COL_DIR				m_eDir = COL_DIR::UP;

	bool				m_isMain;

	bool				m_bColCheck = false;

	DeadReckoning*		m_DeadReckoner;

	vector<tMTAnimClip>			m_pVecAnimClip;

	int					m_iID;
public:
	virtual void awake();	
	virtual void update();

	void init();

	void SetPlayerAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame);
	
	void SetPlayerAnimation(const int& i);
	void SetPlayerAnimation(int other_id, int i);

	void AnimationPlay(const PLAYER_ANI_TYPE& type);
	void AnimationPlay(int other_id, const PLAYER_ANI_TYPE& type);

	Ptr<CMesh> GetAniData(const int& type) { return m_pAniData[(int)type]; }

	void SetMain() { m_isMain = true; }
	bool GetMain() { return m_isMain; }
	void SetOtherMovePacket(sc_packet_move* p, const float& rtt);
	void SetBisFrist(const bool& bis) { FirstPacket = bis; }
	void SetOtherMovePacket__IsMoving(const bool& isMoving) {
		if (m_movePacketTemp != nullptr)m_movePacketTemp->isMoving = isMoving;
	}
	void SwordStrike();
	void SetTargetID(const int& targetID) { m_targetId = targetID; }
	void SetTarget(const bool& isTarget) { m_isTarget = isTarget; }
	int GetTargetId() const { return m_targetId; }
	bool GetTarget() const {return m_isTarget;}
	void SetAttack() { 
		if (m_bAttack) {
			m_bAttack = false;
		}
		else m_bAttack = true;
	}
	void SetSkill() {
		if (m_bSkill) {
			m_bSkill = false;
		}
		else m_bSkill = true;
	}
	void SetCol() {
		if (m_bCol) {
			m_bCol = false;
		}
		else m_bCol = true;
	}
	bool GetAttack() { return m_bAttack; }
	bool GetSkill() { return m_bSkill; }
	bool GetCol() { return m_bCol; }
	float GetSpeed() { return m_fSpeed; }

	Vec3 Get_PlayerPos() { return this->Transform()->GetLocalPos(); }
	CPlayerScript* GetPlayer() { return this; }

	int GetID() { return m_iID; }
	void SetID(const int& id) { m_iID = id; }

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();

public:
	bool				m_bIsActive = false;
	bool GetIsHide() { return m_bIsActive; }
	Vec3 GetPlayerDir() { return m_vecPlayerDir; }
public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
};

