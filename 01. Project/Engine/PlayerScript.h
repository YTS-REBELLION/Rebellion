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
	CGameObject* m_pQuestBox1;
	CGameObject* m_pQuestBox2;
	CGameObject* m_pQuestBox3;
	CGameObject* m_pQuestBox4;
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;
	Vec3				m_vecPlayerDir;
	float				m_fSpeed = PLAYER_SPEED;
	bool				m_bAttack;
	bool				m_bSkill;
	bool				m_bCol;
	sc_packet_move* m_movePacketTemp = nullptr;

	float				m_vecAniClipTime[5]{};
	int					m_iCulidx;

	bool				FirstPacket = false;
	COL_DIR				m_eDir = COL_DIR::UP;

	bool				m_isMain;

	bool				m_bColCheck = false;

	DeadReckoning*		m_DeadReckoner;

	vector<tMTAnimClip>			m_pVecAnimClip;


	bool				m_bMeteor = false;
	bool				m_bMeteor2 = false;
	CGameObject* pMagicJinpp = nullptr;
	float m_fcreate_time = 0.f;
	int m_iClearCnt = 0;
	int m_Q_Cnt = 0;


	bool m_bQuest_01_clear = false;
	bool m_bQuest_02_clear = false;
	bool m_bQuest_03_clear = false;
	bool m_bQuest_04_clear = false;
	bool m_bQuest_05_clear = false;


	Vec3	CharacterBoxScale = Vec3(100.f, 100.f, 1.f);
	Vec3	FrameUiScale = Vec3(600.f, 100.f, 1.f);
	Vec3	HpcoverUiScale = Vec3(500.f, 40.f, 1.f);
	Vec3	HpUiScale = Vec3(500.f, 40.f, 1.f);
	Vec3	MpUiScale = Vec3(500.f, 40.f, 1.f);


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


	void SwordStrike();
	void MegaSlash();
	void Swing();
	void Sting();
	void FireBall();
	void UnleashedPower();
	void Meteor();
	void Delete_Meteor();
	void Set_MagicJin(CGameObject* p) { pMagicJinpp = p; };

	bool GetAttack() { return m_bAttack; }
	bool GetSkill() { return m_bSkill; }
	bool GetCol() { return m_bCol; }
	float GetSpeed() { return m_fSpeed; }

	Vec3 Get_PlayerPos() { return this->Transform()->GetLocalPos(); }
	CPlayerScript* GetPlayer() { return this; }

	int GetID() { return m_iID; }
	void SetID(const int& id) { m_iID = id; }

	CCollider2D* m_pColObj;
	void SetColObj(CCollider2D* _obj) { m_pColObj = _obj; }

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

