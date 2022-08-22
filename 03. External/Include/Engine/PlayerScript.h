#pragma once
#include "Script.h"
#include "Mesh.h"
#include "SwordStrike.h"
#include "DeadReckoning.h"

class DeadReckoning;
class CPlayerScript :
	public CScript
{
public:


private:

	CGameObject* m_pQuestComplete = nullptr;
	CGameObject* m_pQuestBoxExplane1 = nullptr;
	CGameObject* m_pQuestBoxExplane2 = nullptr;
	CGameObject* m_pQuestBoxExplane3 = nullptr;
	CGameObject* m_pQuestBoxExplane4 = nullptr;
	CGameObject* m_pQuestBoxExplane5 = nullptr;
	CGameObject* m_pQuestBoxExplane6 = nullptr;
	CGameObject* m_pQuestBoxExplane7 = nullptr;
	CGameObject* m_pQuestBoxExplane8 = nullptr;

	CGameObject* m_pQuestBox1 = nullptr;
	CGameObject* m_pQuestBox2 = nullptr;
	CGameObject* m_pQuestBox3 = nullptr;

	CGameObject* m_pQuestBox2_1 = nullptr;
	CGameObject* m_pQuestBox2_2 = nullptr;
	CGameObject* m_pQuestBox2_3 = nullptr;

	CGameObject* m_pQuestBox3_1 = nullptr;
	CGameObject* m_pQuestBox3_2 = nullptr;
	CGameObject* m_pQuestBox3_3 = nullptr;

	CGameObject* m_pQuestBox4_1 = nullptr;
	CGameObject* m_pQuestBox4_2 = nullptr;
	CGameObject* m_pQuestBox4_3 = nullptr;

	int m_iKillMonCnt = 0;


	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;
	vector<Ptr<CMesh>>	m_pOhter_AniData;
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
	COL_DIR				m_eDir;

	bool				m_isMain;

	bool				m_bMapCol = false;
	bool				m_bNextMapCol = false;
	DeadReckoning* m_DeadReckoner;

	vector<tMTAnimClip>			m_pVecAnimClip;
	vector<tMTAnimClip>			m_pOhter_VecAnimClip;

	bool				m_bMeteor = false;
	bool				m_bMeteor2 = false;
	CGameObject* pMagicJinpp = nullptr;
	float m_fcreate_time = 0.f;
	QUEST m_iClearCnt = QUEST::FIRST;
	int m_Q_Cnt = 0;
	bool m_questView = false;

	bool m_bQuest_01_clear = false;
	bool m_bQuest_02_clear = false;
	bool m_bQuest_03_clear = false;
	bool m_bQuest_04_clear = false;
	bool m_bQuest_05_clear = false;

	CGameObject* pManaobj;

	Vec3	CharacterBoxScale = Vec3(100.f, 100.f, 1.f);
	Vec3	FrameUiScale = Vec3(500.f, 100.f, 1.f);
	Vec3	HpcoverUiScale = Vec3(500.f, 40.f, 1.f);
	
	bool m_bSkillCool01 = false;
	float m_fSkillCool01 = 0.f;
	bool m_bSkillCool02 = false;
	float m_fSkillCool02 = 0.f;
	bool m_bSkillCool03 = false;
	float m_fSkillCool03 = 0.f;
	bool m_bSkillCool04 = false;
	float m_fSkillCool04 = 0.f;
	

	Ptr<CMeshData> pPMeshData;

	int					m_iID;
	bool				m_bSkill_Start[4] = { false, };
	//Äù½ºÆ®1 png
	Ptr<CTexture> pQuestexplane1 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane", L"Texture\\Quest\\quest01.png");

	Ptr<CTexture> pQuest0 = CResMgr::GetInst()->Load<CTexture>(L"Quest", L"Texture\\Quest\\npc1_quest00.png");
	Ptr<CTexture> pQuest1 = CResMgr::GetInst()->Load<CTexture>(L"Quest2", L"Texture\\Quest\\npc1_quest01.png");
	Ptr<CTexture> pQuest2 = CResMgr::GetInst()->Load<CTexture>(L"Quest3", L"Texture\\Quest\\npc1_quest02.png");
	//Äù½ºÆ®2 png
	Ptr<CTexture> pQuestexplane2_0 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane2_0", L"Texture\\Quest\\quest02_0.png");
	Ptr<CTexture> pQuestexplane2_1 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane2_1", L"Texture\\Quest\\quest02_1.png");
	Ptr<CTexture> pQuestexplane2_2 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane2_2", L"Texture\\Quest\\quest02_2.png");
	Ptr<CTexture> pQuestexplane2_3 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane2_3", L"Texture\\Quest\\quest02_3.png");

	Ptr<CTexture> pQuest2_1 = CResMgr::GetInst()->Load<CTexture>(L"Quest2_2", L"Texture\\Quest\\npc2_quest01.png");
	Ptr<CTexture> pQuest2_2 = CResMgr::GetInst()->Load<CTexture>(L"Quest2_3", L"Texture\\Quest\\npc2_quest02.png");


	//Äù½ºÆ®3 png

	Ptr<CTexture> pQuestexplane3_0 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_0", L"Texture\\Quest\\quest03_0.png");
	Ptr<CTexture> pQuestexplane3_1 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_1", L"Texture\\Quest\\quest03_1.png");
	Ptr<CTexture> pQuestexplane3_2 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_2", L"Texture\\Quest\\quest03_2.png");
	Ptr<CTexture> pQuestexplane3_3 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_3.png");
	Ptr<CTexture> pQuestexplane3_4 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_4.png");
	Ptr<CTexture> pQuestexplane3_5 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_5.png");
	Ptr<CTexture> pQuestexplane3_6 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_6.png");
	Ptr<CTexture> pQuestexplane3_7 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_7.png");
	Ptr<CTexture> pQuestexplane3_8 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_8.png");
	Ptr<CTexture> pQuestexplane3_9 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_9.png");
	Ptr<CTexture> pQuestexplane3_10 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_10.png");
	Ptr<CTexture> pQuestexplane3_11 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_11.png");
	Ptr<CTexture> pQuestexplane3_12 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_12.png");
	Ptr<CTexture> pQuestexplane3_13 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_13.png");
	Ptr<CTexture> pQuestexplane3_14 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_14.png");
	Ptr<CTexture> pQuestexplane3_15 = CResMgr::GetInst()->Load<CTexture>(L"QuestExplane3_3", L"Texture\\Quest\\quest03_15.png");



	Ptr<CTexture> pQuest3_0 = CResMgr::GetInst()->Load<CTexture>(L"Quest3_1", L"Texture\\Quest\\npc3_quest00.png");

	//
	Ptr<CTexture> pQuest4_1 = CResMgr::GetInst()->Load<CTexture>(L"Quest2_3", L"Texture\\Quest\\npc4_quest01.png");
	Ptr<CTexture> pQuest4_2 = CResMgr::GetInst()->Load<CTexture>(L"Quest2_3", L"Texture\\Quest\\quest04_0.png");


	Ptr<CTexture> pQuestComplete = CResMgr::GetInst()->Load<CTexture>(L"QuestComplete", L"Texture\\Quest\\QuestComplete.png");
	bool	m_bDash = false;
	bool	m_bHit = false;
	bool	m_bBossSkillHit = false;
	float fdamage = 0.f;
	float ftempmp = 0.f;

	bool questStart = false;

	CGameObject* pPlayerColObject = nullptr;
	CGameObject* pSwordColObject = nullptr;


	// ui
	CGameObject* m_pUi;
	CGameObject* m_pUnderUi;
	Vec3 HpUiScale;
	Vec3 HpUiPos;
	Vec3 MpUiScale;
	Vec3 MpUiPos;
	int m_pSkillMana = 0;


	PLAYER_ANI_TYPE m_sAniType;
	COL_PLANE m_eCol_Plane;

	CCollider2D* m_pColObj;

public:
	virtual void awake();
	virtual void update();

	void init();

	void SetPlayerAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame);
	void SetPlayerAnimationData(Ptr<CMesh> AniDate, int other_id, const int& i, const UINT& _StartFrame, const UINT& _EndFrame);

	void SetPlayerAnimation(const int& i);
	void SetPlayerAnimation(int other_id, const int& i);

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
	void QuestInit(QUEST questNum);
	void SetTargetID(const int& targetID) { m_targetId = targetID; }
	void SetTarget(const bool& isTarget) { m_isTarget = isTarget; }
	int GetTargetId() const { return m_targetId; }
	bool GetTarget() const { return m_isTarget; }
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
	void SetHit(bool _type) { m_bHit = _type; }
	void SetHitBossSkill(bool _type) { m_bBossSkillHit = _type; }
	void SetMapCol(bool _type) { m_bMapCol = _type; }
	void SetNextMapCol(bool _type) { m_bNextMapCol = _type; }

	void SetQuestStart(bool isStart) { questStart = isStart; }
	void SetQuestCnt(QUEST questId) { m_iClearCnt = questId; }
	QUEST GetQuestCnt() { return m_iClearCnt; }
	void SetQuestView(bool isQuest) { m_questView = isQuest; }
	void QuestDone(QUEST questNum);
	void QuestExplane(QUEST questNum, int count);

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
	bool GetHit() { return m_bHit; }
	bool GetHitBossSkill() { return m_bBossSkillHit; }
	bool GetMapCol() { return m_bMapCol; }
	void GetNextMapCol(bool _type) { m_bNextMapCol = _type; }
	float GetSpeed() { return m_fSpeed; }

	Vec3 Get_PlayerPos() { return this->Transform()->GetLocalPos(); }
	CPlayerScript* GetPlayer() { return this; }

	int GetID() { return m_iID; }
	void SetID(const int& id) { m_iID = id; }

	void SetColPlayer(CGameObject* _obj) { pPlayerColObject = _obj; }
	CGameObject* GetColPlayer() { return pPlayerColObject; }

	void SetColSSA(CGameObject* _obj) { pSwordColObject = _obj; }
	CGameObject* GetColSSA() { return pSwordColObject; }

	bool GetSkillStrat(int _index) { return m_bSkill_Start[_index]; }

	void isDash(bool _type) { m_bDash = _type; };
	// sound
	void PlaySound_(const Sound_Type& sound);

	void SetOtherPlayerAniType(const PLAYER_ANI_TYPE& type) { m_sAniType = type; }

	void SetColPlane(COL_PLANE _eType) { m_eCol_Plane = _eType; }
	COL_PLANE GetColPlane() { return m_eCol_Plane; }

	void SetColObj(CCollider2D* _obj) { m_pColObj = _obj; }
	CCollider2D* GetColObj() { return m_pColObj; }

	bool NextPosColCheak(Vec3 _objPos, CCollider2D* _colobj);
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

