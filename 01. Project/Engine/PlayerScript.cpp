#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include "SwordScript.h"
#include "MonsterScript.h"
#include"CollisionMgr.h"
#include"SwordStrike.h"
#include"MegaSlash.h"
#include"Sting.h"
#include"Swing2.h"
#include"Swing3.h"
#include"Swing4.h"
#include"Swing.h"
#include"FireBall.h"
#include"Meteor.h"
#include"UnleashedPower.h"
#include "Dungeon.h"
#include "GameObject.h"

#include"ParticleSystem.h"
#include "SwordAttackAreaScript.h"
#include"ParticleScript.h"
#include "SoundMgr.h"


bool isReckoning = false;

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_bAttack(false)
	, m_iCulidx(0)
	, m_isTarget(false)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::init()
{	
	cout << "이닛 " << endl;

	/*CGameObject* pSwordCol = new CGameObject;
	pSwordCol->SetName(L"PlayerSwordCol");
	pSwordCol->AddComponent(new CCollider2D);
	pSwordCol->AddComponent(new CTransform);
	pSwordCol->AddComponent(new CMeshRender);
	pSwordCol->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
	pSwordCol->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pSwordCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pSwordCol->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pSwordCol->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	if (CSceneMgr::GetInst()->GetCurScene()->GetType() == SCENE_TYPE::DUNGEON)
		pSwordCol->Collider2D()->SetOffsetScale(Vec3(50.f, 50.f, 50.f));
	else
		pSwordCol->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	pSwordCol->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pSwordCol->AddComponent(new CSwordAttackAreaScript);
	pSwordCol->GetScript<CSwordAttackAreaScript>()->Set_Object(GetObj());
	pSwordCol->SetActive(false);
	GetObj()->GetScript<CPlayerScript>()->SetColSSA(pSwordCol);
	
	CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"PlayerSword", pSwordCol, false);*/


	Ptr<CTexture> pS_astrostone = CResMgr::GetInst()->Load<CTexture>(L"Astrostone", L"Texture\\Icon\\S_astrostone.png");
	Ptr<CTexture> pS_Blood_splash = CResMgr::GetInst()->Load<CTexture>(L"Bloodsplash", L"Texture\\Icon\\S_Blood_splash.png");
	Ptr<CTexture> pS_Blue_firework = CResMgr::GetInst()->Load<CTexture>(L"Blue_firework", L"Texture\\Icon\\S_Blue_firework.png");
	Ptr<CTexture> pPlayer_Frame = CResMgr::GetInst()->Load<CTexture>(L"Player_Frame", L"Texture\\HpUi\\Player_Frame.png");
	Ptr<CTexture> pCharacter_Box = CResMgr::GetInst()->Load<CTexture>(L"Character_Box", L"Texture\\HpUi\\Character_Box.png");
	Ptr<CTexture> pHealth = CResMgr::GetInst()->Load<CTexture>(L"Health", L"Texture\\HpUi\\Health.png");
	Ptr<CTexture> pHealthMana_Cover = CResMgr::GetInst()->Load<CTexture>(L"HealthMana_Cover", L"Texture\\HpUi\\HealthMana_Cover.png");
	Ptr<CTexture> pMana = CResMgr::GetInst()->Load<CTexture>(L"Mana", L"Texture\\HpUi\\Mana.png");


	tResolution res = CRenderMgr::GetInst()->GetResolution();
	CGameObject* pObject = new CGameObject;


	 Ptr<CMaterial>  pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	
	//체력 UI
	pObject = new CGameObject;
	pObject->SetName(L"HpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.5f) - (res.fWidth / 1.5f), res.fHeight / 2.5f + 10.f, 1.f));
	pObject->Transform()->SetLocalScale(FrameUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pCharacter_Box.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"HpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), res.fHeight / 2.5f + 10.f, 1.f));
	pObject->Transform()->SetLocalScale(CharacterBoxScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayer_Frame.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	//hp,mp bar
	Vec3 vScale(350.f, 40.f, 1.f);


	for (int i = 0; i < 2; ++i) {
		CGameObject* pHpUi = new CGameObject;
		pHpUi->SetName(L"HpUICoverObject");
		pHpUi->FrustumCheck(false);   // 절두체 컬링 사용하지 않음
		pHpUi->AddComponent(new CTransform);
		pHpUi->AddComponent(new CMeshRender);

		pHpUi->Transform()->SetLocalPos(Vec3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 160.f
			, (res.fHeight / 2.f) - (vScale.y / 2.f) - (20.f * (i + 1) + (20.f * i))
			, 1.f));

		pHpUi->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pHpUi->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pHpUi->MeshRender()->SetMaterial(pMtrl->Clone());
		if (i == 0) {
			pHpUi->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pHealthMana_Cover.GetPointer());
		}
		else {
			pHpUi->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pHealthMana_Cover.GetPointer());
		}
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pHpUi);


	}


	for (int i = 0; i < 2; ++i) {
		CGameObject* pHpUi = new CGameObject;
		pHpUi->SetName(L"HpUIObject");
		pHpUi->FrustumCheck(false);   // 절두체 컬링 사용하지 않음
		pHpUi->AddComponent(new CTransform);
		pHpUi->AddComponent(new CMeshRender);

		pHpUi->Transform()->SetLocalPos(Vec3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 160.f
			, (res.fHeight / 2.f) - (vScale.y / 2.f) - (20.f * (i + 1) + (20.f * i))
			, 1.f));

		pHpUi->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pHpUi->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pHpUi->MeshRender()->SetMaterial(pMtrl->Clone());
		if (i == 0) {
			pHpUi->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pHealth.GetPointer());
		}
		else {
			pHpUi->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pMana.GetPointer());
		}
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pHpUi);

		if (i == 0) {
			m_pUi = pHpUi;
		}
		else {
			m_pUnderUi = pHpUi;
		}
	}

	////스킬 UI


	//Vec3	SkillUiScale = Vec3(50.f, 50.f, 1.f);

	//pObject = new CGameObject;
	//pObject->SetName(L"SkillIcon");
	//pObject->FrustumCheck(false);
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f - (res.fHeight / 3.f), 1.f));
	//pObject->Transform()->SetLocalScale(SkillUiScale);

	////MeshRender 설정

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	//pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	//pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pS_astrostone.GetPointer());

	//// AddGameObject
	//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);


	//pObject = new CGameObject;
	//pObject->SetName(L"SkillIcon");
	//pObject->FrustumCheck(false);
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f) + SkillUiScale.x, 0.f - (res.fHeight / 3.f), 1.f));
	//pObject->Transform()->SetLocalScale(SkillUiScale);

	////MeshRender 설정

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	//pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	//pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pS_Blood_splash.GetPointer());

	//// AddGameObject
	//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	//pObject = new CGameObject;
	//pObject->SetName(L"SkillIcon");
	//pObject->FrustumCheck(false);
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f) + SkillUiScale.x * 2, 0.f - (res.fHeight / 3.f), 1.f));
	//pObject->Transform()->SetLocalScale(SkillUiScale);

	////MeshRender 설정

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	//pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	//pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pS_Blue_firework.GetPointer());

	//// AddGameObject
	//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Effect\\SwordStrike_Effect.fbx");
}

void CPlayerScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
	m_isTarget = false;
}

void CPlayerScript::update()
{
	// Z-up To Y-up
	Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vDirRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);
	Transform()->SetLocalDir(DIR_TYPE::FRONT, vDirUp);

	Transform()->SetWorldDir(DIR_TYPE::UP, vDirFront);
	Transform()->SetLocalDir(DIR_TYPE::UP, vDirFront);

	Transform()->SetWorldDir(DIR_TYPE::RIGHT, -vDirRight);
	Transform()->SetLocalDir(DIR_TYPE::RIGHT, -vDirRight);

	Vec3 WorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = GetObj()->Transform()->GetLocalPos();
	Vec3 localRot = GetObj()->Transform()->GetLocalRot();
	CTransform* playerTrans = Transform();
	
	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	CPlayerScript* player = GetObj()->GetScript<CPlayerScript>();
	//cout << "PosX" << localPos.x << ", " << "PosY" << localPos.y << ", " << "PosZ" << localPos.z << endl;

	if (m_isMain) {
		if ((KEY_TAB(KEY_TYPE::KEY_W) || KEY_TAB(KEY_TYPE::KEY_A) || KEY_TAB(KEY_TYPE::KEY_S) || KEY_TAB(KEY_TYPE::KEY_D)))
		{
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
		}

		if (KEY_TAB(KEY_TYPE::KEY_SPACE))
		{
			GetObj()->Animator3D()->SetClipTime(3, 0.f);
			SetAttack();
			PlaySound_(Sound_Type::HIT);
			g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), GetAttack());

			
			CGameObject* pObject = new CGameObject;
		
			pObject->SetName(L"PostEffect");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);
			// Material 값 셋팅
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl");
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
			pObject->MeshRender()->SetMaterial(pMtrl, 0);
			pObject->Transform()->SetLocalScale(Vec3(30.f, 100.f, 30.f));
			pObject->Transform()->SetLocalRot(Vec3(0.0f, 1.f, 0.0f));
			
			pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{0.f,150.f,0.f}+ this->Transform()->GetLocalDir(DIR_TYPE::FRONT)*100 + this->Transform()->GetLocalDir(DIR_TYPE::RIGHT) * 50);
			pObject->AddComponent(new CSting);
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);


		}

		else if (KEY_AWAY(KEY_TYPE::KEY_SPACE))
		{
			SetAttack();
			pSwordColObject->SetActive(false);
			g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), GetAttack());
		}

		//if (KEY_TAB(KEY_TYPE::KEY_2))
		//{
		//	GetObj()->Animator3D()->SetClipTime(4, 0.f);
		//	SetSkill();
		//}

		if (localPos.z >= 1400.f && !questStart) {
			questStart = true;
			g_net.Send_Dungeon_Packet(true);
		}
		if (localPos.x > 3400.f) {
			g_net.Send_bossmap_Packet(true);
		}
		if(KEY_TAB(KEY_TYPE::KEY_LSHIFT)) isDash(true);

		else if (KEY_AWAY(KEY_TYPE::KEY_LSHIFT)) isDash(false);

		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			system_clock::time_point start = system_clock::now();

			if (!m_bDash) {
				if (CSceneMgr::GetInst()->GetCurScene()->GetType() == SCENE_TYPE::DUNGEON)
					localPos += WorldDir * m_fSpeed/2.0f * DT;
				else localPos += WorldDir * m_fSpeed * DT;
				AnimationPlay(PLAYER_ANI_TYPE::WALK);
				g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);
			}
			else {
				if (CSceneMgr::GetInst()->GetCurScene()->GetType() == SCENE_TYPE::DUNGEON)
					localPos += WorldDir * m_fSpeed * 1.3f * DT;
				else localPos += WorldDir * m_fSpeed * 2.0f * DT;
				AnimationPlay(PLAYER_ANI_TYPE::RUN);
				g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);
			}
		}

		else if (KEY_HOLD(KEY_TYPE::KEY_NUM3))
		{
			AnimationPlay(PLAYER_ANI_TYPE::DIE);
		}

		else if (KEY_HOLD(KEY_TYPE::KEY_SPACE))
		{
			AnimationPlay(PLAYER_ANI_TYPE::ATTACK);

			if (GetObj()->Animator3D()->GetCliTime(3) > 0.65f)
				pSwordColObject->SetActive(true);

			if(GetObj()->Animator3D()->GetCliTime(3) > 1.4f)
				pSwordColObject->SetActive(false);
		}

		/*else if (GetSkill())
		{
			if (GetObj()->Animator3D()->GetCliTime(4) < GetObj()->Animator3D()->GetAnimClip(4).dTimeLength - 0.05f)
			{
				AnimationPlay(PLAYER_ANI_TYPE::SKILL_1);
				g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_1, GetSkill());

				if (GetObj()->Animator3D()->GetCliTime(4) >= 0.9f
					&& GetObj()->Animator3D()->GetCliTime(4) <= 0.92f) {
					SwordStrike(); m_bSkill_Start[0] = true;
				}
			}
			else
			{
				m_bSkill_Start[0] = false;
				SetSkill();
				g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_1, GetSkill());
			}
		}*/

		else AnimationPlay(PLAYER_ANI_TYPE::IDLE);
		//if (KEY_AWAY(KEY_TYPE::KEY_3))
		//{
		//	cout << "메가슬레시!" << endl;
		//	MegaSlash();
		//}

		if (KEY_AWAY(KEY_TYPE::KEY_5)&& !m_bSkillCool01)
		{
		
			cout << "표창!" << endl;
			Swing();
			fdamage = 20.f;
			m_pSkillMana = 10;
			m_bSkillCool01 = true;
			g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_5, true);

		}

		if (KEY_AWAY(KEY_TYPE::KEY_6) && !m_bSkillCool02)
		{
			
			cout << "파이어볼!" << endl;
			FireBall();
			g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_6, true);
			//g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_2, false);
			fdamage = 20.f;
			m_pSkillMana = 10;
			m_bSkillCool02 = true;
		}

		if (KEY_AWAY(KEY_TYPE::KEY_7) && !m_bSkillCool03)
		{
			cout << "메테오!" << endl;
			Meteor();
			m_bMeteor2 = true;
			fdamage = 20.f;
			m_pSkillMana = 10;
			m_bSkillCool03 = true;
			g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_7, true);

		}

		if (KEY_AWAY(KEY_TYPE::KEY_8) && !m_bSkillCool04)
		{
			UnleashedPower();
			fdamage = 20.f;
			m_pSkillMana = 10;
			g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_8, true);

		}

		if (m_bSkillCool01)
		{
			m_fSkillCool01 += DT;
			if (m_fSkillCool01 > 5.f)
			{
				m_fSkillCool01 = 0.f;
				m_bSkillCool01 = false;
			}
		}

		if (m_bSkillCool02)
		{
			m_fSkillCool02 += DT;
			if (m_fSkillCool02 > 5.f)
			{
				m_fSkillCool02 = 0.f;
				m_bSkillCool02 = false;
			}
		}

		if (m_bSkillCool03)
		{
			m_fSkillCool03 += DT;
			if (m_fSkillCool03 > 5.f)
			{
				m_fSkillCool03 = 0.f;
				m_bSkillCool03 = false;
			}
		}

		if (m_bSkillCool04)
		{
			m_fSkillCool04 += DT;
			if (m_fSkillCool04 > 5.f)
			{
				m_fSkillCool04 = 0.f;
				m_bSkillCool04 = false;
			}
		}


		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
		{
			vRot.y += vDrag.x * DT * 0.5f;
			g_net.Send_Rotate_Packet(g_myid, vRot);
		}

		if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_D)))
		{
			g_net.Send_Stop_Packet(false, GetObj()->GetID());
		}

		// 플레이어 사망
		if (KEY_TAB(KEY_TYPE::KEY_NUM0)) {
			g_net.Send_PlayerDieTest_Packet(GetObj()->GetID());
		}

		if (KEY_TAB(KEY_TYPE::KEY_Q))
		{
			++m_Q_Cnt;

			// 퀘스트 스타트
			//첫번째 퀘스트고 q 를 한번눌렀을시
			if (!m_bQuest_01_clear) {
				if (m_iClearCnt == QUEST::FIRST && m_Q_Cnt == 1)
				{
					m_pQuestBox1->SetDead();
					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBox");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest1.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox2 = pObject;

				}

				if (m_iClearCnt == QUEST::FIRST && m_Q_Cnt == 2)
				{
					m_pQuestBox2->SetDead();
					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBox");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest2.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox3 = pObject;
				}
				if (m_iClearCnt == QUEST::FIRST && m_Q_Cnt == 3)
				{
					m_pQuestBox3->SetDead();
					m_Q_Cnt = 0;
					m_bQuest_01_clear = true;
					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBoxExplane");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane1.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBoxExplane1 = pObject;



				}
			}

			if (!m_bQuest_02_clear) {
				//두번째 퀘스트고 q 를 한번눌렀을시
				if (m_iClearCnt == QUEST::SECOND && m_Q_Cnt == 1)
				{

					m_pQuestBox2_1->SetDead();
					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBox");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest2_1.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox2_2 = pObject;

				}
				if (m_iClearCnt == QUEST::SECOND && m_Q_Cnt == 2)
				{
					m_pQuestBox2_2->SetDead();
					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBox");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest2_2.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox2_3 = pObject;



				}
				if (m_iClearCnt == QUEST::SECOND && m_Q_Cnt == 3)
				{
					m_pQuestBox2_3->SetDead();
					//m_bQuest_02_clear = true;
					m_Q_Cnt = 0;

					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBoxExplane");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane2_0.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBoxExplane2 = pObject;



				}
			}

			if (!m_bQuest_03_clear) {
				//세번째 퀘스트고 q 를 한번눌렀을시
				if (m_iClearCnt == QUEST::THIRD && m_Q_Cnt == 1)
				{
					m_pQuestBox3_1->SetDead();
					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBox");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest2_1.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox3_2 = pObject;




				}
				if (m_iClearCnt == QUEST::THIRD && m_Q_Cnt == 2)
				{
					m_pQuestBox3_2->SetDead();
					m_Q_Cnt = 0;

					//tResolution res = CRenderMgr::GetInst()->GetResolution();

					//Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);

					//CGameObject* pObject = new CGameObject;

					//pObject = new CGameObject;
					//pObject->SetName(L"QuestBoxExplane");
					//pObject->FrustumCheck(false);
					//pObject->AddComponent(new CTransform);
					//pObject->AddComponent(new CMeshRender);

					//pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
					//pObject->Transform()->SetLocalScale(QuestBoxinScale);

					////MeshRender 설정

					//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					//Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					//pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_0.GetPointer());

					//// AddGameObject
					//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					//m_pQuestBox3_3 = pObject;


				}
			}
			if (!m_bQuest_04_clear) {
				//두번째 퀘스트고 q 를 한번눌렀을시
				if (m_iClearCnt == QUEST::FORTH && m_Q_Cnt == 1)
				{
					m_pQuestBox4_1->SetDead();
					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBox");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest4_1.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox4_2 = pObject;

				}
				if (m_iClearCnt == QUEST::FORTH && m_Q_Cnt == 2)
				{
					m_pQuestBox4_2->SetDead();
					//m_bQuest_02_clear = true;
					m_Q_Cnt = 0;

					tResolution res = CRenderMgr::GetInst()->GetResolution();

					Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
					CGameObject* pObject = new CGameObject;

					pObject = new CGameObject;
					pObject->SetName(L"QuestBoxExplane");
					pObject->FrustumCheck(false);
					pObject->AddComponent(new CTransform);
					pObject->AddComponent(new CMeshRender);

					pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
					pObject->Transform()->SetLocalScale(QuestBoxinScale);

					//MeshRender 설정

					pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

					Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
					pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest4_2.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBoxExplane1 = pObject;


				}
			}
		}

		//체력감소ui
		HpUiScale = m_pUi->Transform()->GetLocalScale();
		HpUiPos = m_pUi->Transform()->GetLocalPos();

		if (GetHit()) {
			HpUiScale.x -= DT;//충돌할시 *데미지
			HpUiPos.x -= DT / 2.f;
		}

		if (GetHitBossSkill()) {
			HpUiScale.x -= 20.f * DT;//충돌할시 *데미지
			HpUiPos.x -= (20.f * DT) / 2.f;
		}
		m_pUi->Transform()->SetLocalScale(Vec3(HpUiScale.x, HpUiScale.y, HpUiScale.z));
		m_pUi->Transform()->SetLocalPos(Vec3(HpUiPos.x, HpUiPos.y, HpUiPos.z));

		//마나감소ui
		MpUiScale = m_pUnderUi->Transform()->GetLocalScale();
		MpUiPos = m_pUnderUi->Transform()->GetLocalPos();

		if (MpUiScale.x < 350.f)
		{
			MpUiScale.x += (DT);//자연마나회복
			MpUiPos.x += (DT) / 2.f;
		}
		if (MpUiScale.x >= 350.f)
		{
			MpUiScale.x = 350.f;
			 
		}
		if (MpUiScale.x <= 0.f)
		{
			MpUiScale.x = 0.f;

		}
		
		MpUiScale.x -= (m_pSkillMana);//스킬쓸시 *마나량
		MpUiPos.x -= (m_pSkillMana) / 2.f;

		m_pUnderUi->Transform()->SetLocalScale(Vec3(MpUiScale.x, MpUiScale.y, MpUiScale.z));
		m_pUnderUi->Transform()->SetLocalPos(Vec3(MpUiPos.x, MpUiPos.y, MpUiPos.z));

		m_pSkillMana = 0;

		if (m_bMeteor2)
		{
			m_fcreate_time += DT;
		}
		if (m_fcreate_time >= 5.f)
		{
			m_bMeteor = true;
		}
		if (m_bMeteor)
		{
			Delete_Meteor();
		}

		Vec3 NextLocalPos = localPos + WorldDir * m_fSpeed * 5.0f * DT;

		if (GetMapCol())
		{
			switch (m_eCol_Plane)
			{
			case COL_PLANE::X_PLANE:
				if (KEY_HOLD(KEY_TYPE::KEY_W))
				{
					localPos.x += WorldDir.x;
					if (!m_bDash) {
						if (NextPosColCheak(NextLocalPos, GetColObj())) localPos.z -= WorldDir.z * m_fSpeed * DT;
						else localPos.z += WorldDir.z * m_fSpeed * DT;
					}
					else {
						if (NextPosColCheak(NextLocalPos, GetColObj())) localPos.z -= WorldDir.z * m_fSpeed * 2.0f * DT;
						else localPos.z += WorldDir.z * m_fSpeed * DT;
					}
				}
				break;
			case COL_PLANE::Z_PLANE:
				if (KEY_HOLD(KEY_TYPE::KEY_W))
				{
					localPos.z += WorldDir.z;
					if (!m_bDash) {
						if (NextPosColCheak(NextLocalPos, GetColObj())) localPos.x -= WorldDir.x * m_fSpeed * DT;
						else localPos.x += WorldDir.x * m_fSpeed * DT;
					}
					else {
						if (NextPosColCheak(NextLocalPos, GetColObj())) localPos.x -= WorldDir.x * m_fSpeed * 2.0f * DT;
						else localPos.x += WorldDir.x * m_fSpeed * DT;
					}
				}
				break;
			default:
				break;
			}
		}

		Transform()->SetLocalPos(localPos);
		Transform()->SetLocalRot(vRot);
	}
	else
	{
		switch (m_sAniType)
		{
		case PLAYER_ANI_TYPE::IDLE:
			AnimationPlay(GetID(), PLAYER_ANI_TYPE::IDLE);
			break;
		case PLAYER_ANI_TYPE::WALK:
			AnimationPlay(GetID(), PLAYER_ANI_TYPE::WALK);
			break;
		case PLAYER_ANI_TYPE::RUN:
			AnimationPlay(GetID(), PLAYER_ANI_TYPE::RUN);
			break;
		case PLAYER_ANI_TYPE::ATTACK:
			if (GetObj()->Animator3D()->GetCliTime(3) > 0.65f)
				GameObject.find(GetID())->second->GetScript<CPlayerScript>()->GetColSSA()->SetActive(true);
			AnimationPlay(GetID(), PLAYER_ANI_TYPE::ATTACK);
			break;
		case PLAYER_ANI_TYPE::SKILL_1:
			break;
		case PLAYER_ANI_TYPE::SKILL_2:
			break;
		case PLAYER_ANI_TYPE::SKILL_3:
			break;
		case PLAYER_ANI_TYPE::DIE:
			break;
		default:
			break;
		}
	}
}


void CPlayerScript::SetPlayerAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame)
{
	m_pAniData.push_back(AniDate);

	tMTAnimClip* tNewAnimClip = new tMTAnimClip;
	tNewAnimClip->iStartFrame = _StartFrame;
	tNewAnimClip->iEndFrame = _EndFrame;
	tNewAnimClip->iFrameLength = _EndFrame - _StartFrame;
	tNewAnimClip->dStartTime = (double)_StartFrame / (double)30;
	tNewAnimClip->dEndTime = (double)_EndFrame / (double)30;
	tNewAnimClip->dTimeLength = tNewAnimClip->dEndTime - tNewAnimClip->dStartTime;

	m_pVecAnimClip.push_back(*tNewAnimClip);

	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);
}

void CPlayerScript::SetPlayerAnimationData(Ptr<CMesh> AniDate, int other_id, const int& i, const UINT& _StartFrame, const UINT& _EndFrame)
{
	m_pOhter_AniData.push_back(AniDate);

	tMTAnimClip* tNewAnimClip = new tMTAnimClip;
	tNewAnimClip->iStartFrame = _StartFrame;
	tNewAnimClip->iEndFrame = _EndFrame;
	tNewAnimClip->iFrameLength = _EndFrame - _StartFrame;
	tNewAnimClip->dStartTime = (double)_StartFrame / (double)30;
	tNewAnimClip->dEndTime = (double)_EndFrame / (double)30;
	tNewAnimClip->dTimeLength = tNewAnimClip->dEndTime - tNewAnimClip->dStartTime;

	m_pOhter_VecAnimClip.push_back(*tNewAnimClip);

	GameObject.find(other_id)->second->Animator3D()->SetAnimClip(&m_pOhter_VecAnimClip);
}

void CPlayerScript::SetPlayerAnimation(const int& i)
{
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);
	GetObj()->MeshRender()->SetMesh(m_pAniData[i]);
}

void CPlayerScript::SetPlayerAnimation(int other_id, const int& i)
{
	//if (m_pAniData.size() == 0)	return;
	GameObject.find(other_id)->second->Animator3D()->SetBones(m_pOhter_AniData[i]->GetBones());
	GameObject.find(other_id)->second->Animator3D()->SetAnimClip(&m_pOhter_VecAnimClip);
	GameObject.find(other_id)->second->MeshRender()->SetMesh(m_pOhter_AniData[i]);
}

void CPlayerScript::SetOtherMovePacket(sc_packet_move* p, const float& rtt)
{
	m_movePacketTemp = new sc_packet_move;

	m_movePacketTemp = p;

}

void CPlayerScript::AnimationPlay(const PLAYER_ANI_TYPE& type)
{
	if (type == PLAYER_ANI_TYPE::IDLE)
	{
		GetObj()->Animator3D()->SetCurClip(0);
		SetPlayerAnimation(0);
	}
	if (type == PLAYER_ANI_TYPE::WALK)
	{
		GetObj()->Animator3D()->SetCurClip(1);
		SetPlayerAnimation(1);
	}
	if (type == PLAYER_ANI_TYPE::RUN)
	{
		GetObj()->Animator3D()->SetCurClip(2);
		SetPlayerAnimation(2);
	}
	if (type == PLAYER_ANI_TYPE::ATTACK)
	{
		GetObj()->Animator3D()->SetCurClip(3);
		SetPlayerAnimation(3);
	}
	if (type == PLAYER_ANI_TYPE::SKILL_1)
	{
		GetObj()->Animator3D()->SetCurClip(4);
		SetPlayerAnimation(4);
	}
	if (type == PLAYER_ANI_TYPE::DIE)
	{
		GetObj()->Animator3D()->SetCurClip(5);
		SetPlayerAnimation(5);
	}
}

void CPlayerScript::AnimationPlay(int other_id, const PLAYER_ANI_TYPE& type)
{
	if (type == PLAYER_ANI_TYPE::IDLE)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(0);
		SetPlayerAnimation(other_id, 0);
	}
	if (type == PLAYER_ANI_TYPE::WALK)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(1);
		SetPlayerAnimation(other_id, 1);
	}
	if (type == PLAYER_ANI_TYPE::RUN)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(2);
		SetPlayerAnimation(other_id, 2);
	}
	if (type == PLAYER_ANI_TYPE::ATTACK)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(3);
		SetPlayerAnimation(other_id, 3);
	}
	if (type == PLAYER_ANI_TYPE::SKILL_1)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(4);
		SetPlayerAnimation(other_id, 4);
	}
	if (type == PLAYER_ANI_TYPE::DIE)
	{
		GetObj()->Animator3D()->SetCurClip(5);
		SetPlayerAnimation(other_id, 5);
	}
}

void CPlayerScript::OnCollisionEnter(CCollider2D* _pOther)
{
	//if (_pOther->GetObj()->GetName() == L"FM_Monster") g_net.Send_Mon2Player_Packet(GetObj()->GetID(), true);

	if (_pOther->GetObj()->GetName() == L"MonsterSwordCol") SetHit(true);

	if (_pOther->GetObj()->GetName() == L"BossSwordCol") SetHit(true);

	if (_pOther->GetObj()->GetName() == L"FireTest" ||
		_pOther->GetObj()->GetName() == L"FireTest2"||
		_pOther->GetObj()->GetName() == L"FireTest3"||
		_pOther->GetObj()->GetName() == L"FireBall"
		) SetHitBossSkill(true);

	if (_pOther->GetObj()->GetName() == L"Map_Wall")
	{
		SetColPlane(_pOther->GetPlane());
		SetColObj(_pOther);
		SetMapCol(true);
	}
}

bool CPlayerScript::NextPosColCheak(Vec3 _objPos, CCollider2D* _colobj)
{
	float CenterX = _objPos.x;
	float CenterZ = _objPos.z;
	float Radius = GetColPlayer()->Transform()->GetLocalScale().x * GetColPlayer()->Collider2D()->GetOffsetScale().x;

	// left		: -x
	// right	: x
	// top		: z
	// bottom	: -z

	float left = _colobj->GetObj()->Transform()->GetLocalPos().x - (_colobj->Transform()->GetLocalScale().x * _colobj->Collider2D()->GetOffsetScale().x) / 2;
	float right = _colobj->GetObj()->Transform()->GetLocalPos().x + (_colobj->Transform()->GetLocalScale().x * _colobj->Collider2D()->GetOffsetScale().x) / 2;
	float top = _colobj->GetObj()->Transform()->GetLocalPos().z - (_colobj->Transform()->GetLocalScale().z * _colobj->Collider2D()->GetOffsetScale().z) / 2;
	float bottom = _colobj->GetObj()->Transform()->GetLocalPos().z + (_colobj->Transform()->GetLocalScale().z * _colobj->Collider2D()->GetOffsetScale().z) / 2;

	if ((left <= CenterX && CenterX < right) || (top <= CenterZ && CenterZ < bottom))
	{
		if ((left - Radius < CenterX && CenterX < right + Radius) &&
			(top - Radius < CenterZ && CenterZ < bottom + Radius)) {
			return true;
		}
	}
	else {
		// 사각형 좌상단이 원 안에 있는지
		if (Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(left, 0.f, top))) return true;

		// 사각형 좌하단이 원 안에 있는지
		if (Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(left, 0.f, bottom))) return true;

		// 사각형 우상단이 원 안에 있는지
		if (Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(right, 0.f, top))) return true;

		// 사각형 우하단이 원 안에 있는지
		if (Radius >= Vec3::Distance(Vec3(CenterX, 0.f, CenterZ), Vec3(right, 0.f, bottom))) return true;
	}
	
	return false;
}

void CPlayerScript::OnCollision(CCollider2D* _pOther)
{
}

void CPlayerScript::OnCollisionExit(CCollider2D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"MonsterSwordCol") SetHit(false);

	if (_pOther->GetObj()->GetName() == L"FireTest" ||
		_pOther->GetObj()->GetName() == L"FireTest2" ||
		_pOther->GetObj()->GetName() == L"FireTest3" ||
		_pOther->GetObj()->GetName() == L"FireBall"
		) SetHitBossSkill(false);

	if (_pOther->GetObj()->GetName() == L"Map_Wall") SetMapCol(false);
}

void CPlayerScript::SwordStrike()
{
	cout << "소드 스트라이크" << endl;
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;

	Vec3 pPos = Get_PlayerPos();

	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"SwordStike");
	m_pSwordStrike->FrustumCheck(false);

	m_pSwordStrike->Transform()->SetLocalPos(Vec3(pPos.x, pPos.y + 100.f, pPos.z) + GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100.f);
	m_pSwordStrike->Transform()->SetLocalRot(Vec3(this->Transform()->GetLocalRot().x, this->Transform()->GetLocalRot().y + XMConvertToRadians(-90.0f), this->Transform()->GetLocalRot().z));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwordStrike);
	m_pSwordStrike->GetScript<CSwordStrike>()->Set_Player(GetObj());

	/*m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));*/

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"SwordStike");
	m_pSwordStrike->FrustumCheck(false);

	m_pSwordStrike->Transform()->SetLocalPos(Vec3(pPos.x, pPos.y + 100.f, pPos.z) + GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100.f);
	m_pSwordStrike->Transform()->SetLocalRot(Vec3(this->Transform()->GetLocalRot().x, this->Transform()->GetLocalRot().y + XMConvertToRadians(-60.0f), this->Transform()->GetLocalRot().z));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwordStrike);
	m_pSwordStrike->GetScript<CSwordStrike>()->Set_Player(GetObj());

	/*m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));*/

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);

	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"SwordStike");
	m_pSwordStrike->FrustumCheck(false);

	m_pSwordStrike->Transform()->SetLocalPos(Vec3(pPos.x, pPos.y + 100.f, pPos.z) + GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100.f);
	m_pSwordStrike->Transform()->SetLocalRot(Vec3(this->Transform()->GetLocalRot().x, this->Transform()->GetLocalRot().y + XMConvertToRadians(-120.0f), this->Transform()->GetLocalRot().z));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwordStrike);
	m_pSwordStrike->GetScript<CSwordStrike>()->Set_Player(GetObj());

	/*m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));*/

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);
}

void CPlayerScript::MegaSlash()
{
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"MegaSlash");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,50.f,0.f });
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalDir(DIR_TYPE::FRONT));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	m_pSwordStrike->AddComponent(new CMegaSlash);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(300.f, 300.f, 300.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);


}

void CPlayerScript::Swing()
{
	//플레이어 따라오는 표창
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Sjuriken1.fbx");

	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Swing");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos());
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	m_pSwordStrike->AddComponent(new CSwing);
	m_pSwordStrike->GetScript<CSwing>()->SetPlayer(GetObj());

	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);

	//// ====================
	////  오브젝트 생성
	//// ====================
	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Swing");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() );
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	m_pSwordStrike->AddComponent(new CSwing2);
	m_pSwordStrike->GetScript<CSwing2>()->SetPlayer(GetObj());

	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);

	//// ====================
	////  오브젝트 생성
	//// ====================
	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Swing");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos());
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	m_pSwordStrike->AddComponent(new CSwing3);
	m_pSwordStrike->GetScript<CSwing3>()->SetPlayer(GetObj());

	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);

	//// ====================
	////  오브젝트 생성
	//// ====================
	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Swing");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() );
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	m_pSwordStrike->AddComponent(new CSwing4);
	m_pSwordStrike->GetScript<CSwing4>()->SetPlayer(GetObj());

	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);



	

}

void CPlayerScript::Sting()
{
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Sting");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,50.f,0.f });
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSting);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);
}

void CPlayerScript::FireBall()
{
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pSwordMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Weapon.mdat", L"MeshData\\Monster_FM_Weapon.mdat");
	


	m_pSwordStrike = pSwordMeshData->Instantiate();
	m_pSwordStrike->SetName(L"FireBall");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,50.f,0.f });
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CFireBall);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);

}

void CPlayerScript::UnleashedPower()
{

	Ptr<CMeshData> pSwordMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Weapon.mdat", L"MeshData\\Monster_FM_Weapon.mdat");
	CGameObject* m_pSwordStrike = new CGameObject;
	


	m_pSwordStrike = pSwordMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pSwordMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 150 + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 100);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pSwordMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100 + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pSwordMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 150 - this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 100);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pSwordMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100 - this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);
}

void CPlayerScript::Meteor()
{

	Ptr<CTexture> pMagic = CResMgr::GetInst()->Load<CTexture>(L"Magic", L"Texture\\aming_point_big00.png");

	CGameObject* pMagicJin = nullptr;
	pMagicJin = new CGameObject;
	pMagicJin->SetName(L"MagicJin Object");
	pMagicJin->AddComponent(new CTransform);
	pMagicJin->AddComponent(new CMeshRender);

	// Transform 설정
	pMagicJin->Transform()->SetLocalPos(Vec3(this->Transform()->GetLocalPos() + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 1000));
	pMagicJin->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	pMagicJin->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	// MeshRender 설정
	pMagicJin->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMagicJin->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
	pMagicJin->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pMagic.GetPointer());


	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pMagicJin);





	Ptr<CMeshData> pSwordMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Weapon.mdat", L"MeshData\\Monster_FM_Weapon.mdat");

	
	CGameObject* m_pSwordStrike = new CGameObject;
	


	m_pSwordStrike = pSwordMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Meteor");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,800.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 1000);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CMeteor);
	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	// AddGameObject
	m_pSwordStrike->MeshRender()->SetDynamicShadow(true);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player_Skill")->AddGameObject(m_pSwordStrike);


	Set_MagicJin(pMagicJin);

}

void CPlayerScript::Delete_Meteor()
{
	pMagicJinpp->SetDead();
	m_bMeteor = false;
	m_fcreate_time = 0.f;
	m_bMeteor2 = false;

}

void CPlayerScript::QuestInit(QUEST questNum)
{
	Ptr<CTexture> pQuest = CResMgr::GetInst()->Load<CTexture>(L"Quest", L"Texture\\Quest\\npc1_quest00.png");
	//Ptr<CTexture> pQuest2 = CResMgr::GetInst()->Load<CTexture>(L"Quest3", L"Texture\\Quest\\UIboard.png");
	Ptr<CTexture> pQuest2_0 = CResMgr::GetInst()->Load<CTexture>(L"Quest2_1", L"Texture\\Quest\\npc2_quest00.png");
	Ptr<CTexture> pQuest3_0 = CResMgr::GetInst()->Load<CTexture>(L"Quest3_1", L"Texture\\Quest\\npc3_quest00.png");
	Ptr<CTexture> pQuest4_0 = CResMgr::GetInst()->Load<CTexture>(L"Quest2_3", L"Texture\\Quest\\npc4_quest00.png");

	switch (questNum) {
	case QUEST::FIRST: {
		//g_net.Send_Stop_Packet(false, GetObj()->GetID());


		// 몬스터 사냥 카운트
		//퀘스트 png
		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3   QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBox");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBox1 = pObject;

		

		break;
	}
	case QUEST::SECOND: {
		//g_net.Send_Stop_Packet(false, GetObj()->GetID());

		//m_pQuestComplete->SetDead();


		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3   QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBox");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest2_0.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);


		m_pQuestBox2_1 = pObject;
		break;
	}
	case QUEST::THIRD: {
		cout << "세번째 퀘스트" << endl;
		m_pQuestBoxExplane5->SetDead();
		//m_pQuestBoxExplane2->SetDead();
		//m_pQuestBoxExplane5->SetDead();
		//m_pQuestComplete->SetDead();
		//m_pQuestComplete->SetDead();
		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3   QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBox");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest3_0.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBox3_1 = pObject;
		break;
	}
	case QUEST::FORTH: {
		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3   QuestBoxinScale = Vec3(800.f, 200.f, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBox");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 5.f), 0.f - (res.fHeight / 8.f), 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest4_0.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBox4_1 = pObject;

		break;
	}
	}
}

void CPlayerScript::QuestDone(QUEST questNum)
{
	Ptr<CTexture> pQuestComplete = CResMgr::GetInst()->Load<CTexture>(L"QuestComplete", L"Texture\\Quest\\QuestComplete.png");

	tResolution res = CRenderMgr::GetInst()->GetResolution();

	Vec3	QuestBoxinScale = Vec3(128, 54, 1.f);
	CGameObject* pObject = new CGameObject;

	pObject = new CGameObject;
	pObject->SetName(L"QuestBoxComplete");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
	pObject->Transform()->SetLocalScale(QuestBoxinScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestComplete.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
	m_pQuestComplete = pObject;


}
void CPlayerScript::QuestExplane(QUEST questNum, int count)
{
	switch (questNum) {
	case QUEST::SECOND:
	{
		if (count == 1) {
			cout << "count == 1" << endl;
			m_pQuestBoxExplane2->SetDead();

			tResolution res = CRenderMgr::GetInst()->GetResolution();

			Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
			CGameObject* pObject = new CGameObject;

			pObject = new CGameObject;
			pObject->SetName(L"QuestBoxExplane");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane2_1.GetPointer());

			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
			m_pQuestBoxExplane3 = pObject;

		}
		else if (count == 2) {
			cout << "count == 2" << endl;

			m_pQuestBoxExplane3->SetDead();

			tResolution res = CRenderMgr::GetInst()->GetResolution();

			Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
			CGameObject* pObject = new CGameObject;

			pObject = new CGameObject;
			pObject->SetName(L"QuestBoxExplane");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane2_2.GetPointer());

			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
			m_pQuestBoxExplane4 = pObject;

		}
		else if (count == 3) {
			cout << "count == 3" << endl;

			m_pQuestBoxExplane4->SetDead();

			tResolution res = CRenderMgr::GetInst()->GetResolution();

			Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
			CGameObject* pObject = new CGameObject;

			pObject = new CGameObject;
			pObject->SetName(L"QuestBoxExplane");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane2_3.GetPointer());

			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
			m_pQuestBoxExplane5 = pObject;

		}



		break;
		
	}
	case QUEST::THIRD: {
		if (count == 1) {
			cout << "count == 1" << endl;
			m_pQuestBoxExplane2->SetDead();

			tResolution res = CRenderMgr::GetInst()->GetResolution();

			Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
			CGameObject* pObject = new CGameObject;

			pObject = new CGameObject;
			pObject->SetName(L"QuestBoxExplane");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_1.GetPointer());

			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
			m_pQuestBoxExplane3 = pObject;

		}
		else if (count == 2) {
			cout << "count == 2" << endl;

			m_pQuestBoxExplane3->SetDead();

			tResolution res = CRenderMgr::GetInst()->GetResolution();

			Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
			CGameObject* pObject = new CGameObject;

			pObject = new CGameObject;
			pObject->SetName(L"QuestBoxExplane");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_2.GetPointer());

			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
			m_pQuestBoxExplane4 = pObject;

		}
		else if (count == 3) {
			cout << "count == 3" << endl;

			m_pQuestBoxExplane4->SetDead();

			tResolution res = CRenderMgr::GetInst()->GetResolution();

			Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
			CGameObject* pObject = new CGameObject;

			pObject = new CGameObject;
			pObject->SetName(L"QuestBoxExplane");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_3.GetPointer());

			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
			m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 4) {
			cout << "count == 3" << endl;

			m_pQuestBoxExplane4->SetDead();

			tResolution res = CRenderMgr::GetInst()->GetResolution();

			Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
			CGameObject* pObject = new CGameObject;

			pObject = new CGameObject;
			pObject->SetName(L"QuestBoxExplane");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_4.GetPointer());

			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
			m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 5) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_5.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 6) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_6.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 7) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_7.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 8) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_8.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 9) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_9.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 10) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_10.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 11) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_11.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 12) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_12.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 13) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_13.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 14) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_14.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}
		else if (count == 15) {
		cout << "count == 3" << endl;

		m_pQuestBoxExplane4->SetDead();

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		Vec3	QuestBoxinScale = Vec3(200, 54, 1.f);
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"QuestBoxExplane");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f, 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxinScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_15.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBoxExplane5 = pObject;

		}

		break;
	}
	}






}
void CPlayerScript::PlaySound_(const Sound_Type& sound)
{
	g_SoundList.find(sound)->second->Play(1);
	
}