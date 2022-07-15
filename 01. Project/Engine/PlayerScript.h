#pragma once
#include "Script.h"
#include "Mesh.h"
#include "SwordStrike.h"

enum class COL_DIR
{
	RIGHT,
	UP,
	LEFT,
	DOWN,
	END,
};


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
	sc_packet_move* m_movePacketTemp = nullptr;

	float				m_vecAniClipTime[5]{};
	int					m_iCulidx;

	bool				FirstPacket = false;
	COL_DIR				m_eDir = COL_DIR::UP;

	int					m_id;
	bool				m_isMain;

	bool				m_bColCheck = false;
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

	
public:
	virtual void awake();	
	virtual void update();

	void GetPlayerAnimation(Ptr<CMesh> AniDate) { m_pAniData.push_back(AniDate); };
	void SetPlayerAnimation(const int& i);
	Ptr<CMesh> GetAniData(const int& type) { return m_pAniData[(int)type]; }

	void SetPlayerAnimation(int other_id, int i);
	void SetMain() { m_isMain = true; };

	void SetOtherMovePacket(sc_packet_move* p, const float& rtt);
	void SetBisFrist(const bool& bis) { FirstPacket = bis; }
	void SetOtherMovePacket__IsMoving(const bool& isMoving) {
		if (m_movePacketTemp != nullptr)m_movePacketTemp->isMoving = isMoving;
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
	void SetAttack() { 
		if (m_bAttack) {
			m_bAttack = false;
		}
		else m_bAttack = true;
	}
	bool GetAttack() { return m_bAttack; }
	Vec3 Get_PlayerPos() { return this->Transform()->GetLocalPos(); }
	CPlayerScript* GetPlayer() { return this; }


	
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

