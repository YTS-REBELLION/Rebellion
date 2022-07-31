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

#include"ParticleScript.h"

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
	// ===================
	// Sword 파일 로드
	// ===================
	CGameObject* pSwordObject = new CGameObject;

	Ptr<CMeshData>pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Sword.mdat", L"MeshData\\Player_Sword.mdat");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");

	pSwordObject = pMeshData->Instantiate();
	pSwordObject->SetName(L"Player_Sword");
	pSwordObject->FrustumCheck(false);
	//pSwordObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pSwordObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pSwordObject->AddComponent(new CCollider2D);
	pSwordObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pSwordObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pSwordObject->Collider2D()->SetOffsetScale(Vec3(20.f, 140.f, 20.f));

	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");
	pSwordObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, SwordObject.GetPointer());


	pSwordObject->AddComponent(new CSwordScript);
	CSwordScript* SwordScript = pSwordObject->GetScript<CSwordScript>();
	pSwordObject->GetScript<CSwordScript>()->init(PERSON_OBJ_TYPE::WARRIOR_PLAYER, GetObj(), 25);

	CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", pSwordObject, false);
	GetObj()->AddChild(pSwordObject);
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
	
	//체력 UI
	pObject = new CGameObject;
	pObject->SetName(L"HpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f), res.fHeight / 2.5f, 1.f));
	pObject->Transform()->SetLocalScale(FrameUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	Ptr<CMaterial>  pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pCharacter_Box.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"HpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), res.fHeight / 2.5f, 1.f));
	pObject->Transform()->SetLocalScale(CharacterBoxScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayer_Frame.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"MpUiCover");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f), res.fHeight / 2.7f, 1.f));
	pObject->Transform()->SetLocalScale(HpcoverUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pHealthMana_Cover.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);


	 pObject = new CGameObject;

	pObject = new CGameObject;
	pObject->SetName(L"MpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f), res.fHeight / 2.7f, 1.f));
	pObject->Transform()->SetLocalScale(MpUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pMana.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	pManaobj = pObject;

	


	pObject = new CGameObject;
	pObject->SetName(L"HpUiCover");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f), res.fHeight / 2.3f, 1.f));
	pObject->Transform()->SetLocalScale(HpcoverUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pHealthMana_Cover.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	


	pObject = new CGameObject;
	pObject->SetName(L"HpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f), res.fHeight / 2.3f, 1.f));
	pObject->Transform()->SetLocalScale(HpUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pHealth.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);


	//스킬 UI


	Vec3	SkillUiScale = Vec3(50.f, 50.f, 1.f);

	pObject = new CGameObject;
	pObject->SetName(L"SkillIcon");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f), 0.f - (res.fHeight / 3.f), 1.f));
	pObject->Transform()->SetLocalScale(SkillUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pS_astrostone.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);


	pObject = new CGameObject;
	pObject->SetName(L"SkillIcon");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f) + SkillUiScale.x, 0.f - (res.fHeight / 3.f), 1.f));
	pObject->Transform()->SetLocalScale(SkillUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pS_Blood_splash.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"SkillIcon");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 4.f) - (res.fWidth / 1.5f) + SkillUiScale.x * 2, 0.f - (res.fHeight / 3.f), 1.f));
	pObject->Transform()->SetLocalScale(SkillUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pS_Blue_firework.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

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

	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirFront);
	Transform()->SetLocalDir(DIR_TYPE::FRONT, vDirFront);

	Transform()->SetWorldDir(DIR_TYPE::UP, -vDirUp);
	Transform()->SetLocalDir(DIR_TYPE::UP, -vDirUp);

	Transform()->SetWorldDir(DIR_TYPE::RIGHT, -vDirRight);
	Transform()->SetLocalDir(DIR_TYPE::RIGHT, -vDirRight);

	Vec3 WorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = GetObj()->Transform()->GetLocalPos();
	Vec3 localRot = GetObj()->Transform()->GetLocalRot();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	/*cout << "playerPosX:"<< localPos.x << endl;
	cout << "playerPosY:" << localPos.y << endl;
	cout << "playerPosZ:" << localPos.z << endl;*/

	CPlayerScript* player = GetObj()->GetScript<CPlayerScript>();

	if (m_isMain) {
		if ((KEY_TAB(KEY_TYPE::KEY_W) || KEY_TAB(KEY_TYPE::KEY_A) || KEY_TAB(KEY_TYPE::KEY_S) || KEY_TAB(KEY_TYPE::KEY_D)))
		{
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
			if (localPos.z >= 1400.f) {
				g_net.Send_Dungeon_Packet(true);
			}
		}
		
		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			system_clock::time_point start = system_clock::now();
			if(!m_bColCheck)
				localPos += WorldDir * (m_bDash ? m_fSpeed * 2.0f : m_fSpeed) * DT;

			AnimationPlay(PLAYER_ANI_TYPE::WALK);
			g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				isDash(true);
				AnimationPlay(PLAYER_ANI_TYPE::RUN);
				g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);
			}
			else if (KEY_AWAY(KEY_TYPE::KEY_LSHIFT)) isDash(false);

		}
		else if (KEY_HOLD(KEY_TYPE::KEY_S))
		{
			localPos -= WorldDir * m_fSpeed * DT;

			system_clock::time_point start = system_clock::now();
			m_eDir = COL_DIR::BACK;

			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				localPos -= WorldDir * m_fSpeed * DT;
				AnimationPlay(PLAYER_ANI_TYPE::RUN);
				g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);


			}
			else {
				AnimationPlay(PLAYER_ANI_TYPE::WALK);
				g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			};
		}

		else if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
			localPos -= WorldDir * m_fSpeed * DT;

			system_clock::time_point start = system_clock::now();
			m_eDir = COL_DIR::LEFT;

			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				localPos -= WorldDir * m_fSpeed * DT;
				AnimationPlay(PLAYER_ANI_TYPE::RUN);
				g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);


			}
			else {
				AnimationPlay(PLAYER_ANI_TYPE::WALK);
				g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			};
		}

		else
		{
			AnimationPlay(PLAYER_ANI_TYPE::IDLE);
		}

		if (KEY_TAB(KEY_TYPE::KEY_SPACE))
		{
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
			SetAttack();
		}

		else if (GetAttack() && m_vecAniClipTime[0] < GetObj()->Animator3D()->GetAnimClip(3).dTimeLength)
		{
			m_vecAniClipTime[0] += (DT * 1.5f);
			AnimationPlay(PLAYER_ANI_TYPE::ATTACK);
			g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), GetAttack());

			if (m_vecAniClipTime[0] > GetObj()->Animator3D()->GetAnimClip(3).dTimeLength)
			{
				m_vecAniClipTime[0] = 0.0f;
				SetAttack();
				g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), GetAttack());
			}
		}

		if (KEY_TAB(KEY_TYPE::KEY_3))
		{
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
			SetSkill();
			SwordStrike();
			fdamage = 20.f;
			MpUiScale.x -= fdamage;
		}
		else if (GetSkill() && m_vecAniClipTime[1] < GetObj()->Animator3D()->GetAnimClip(4).dTimeLength)
		{
			m_vecAniClipTime[1] += (DT * 1.5f * 1.5f);
			AnimationPlay(PLAYER_ANI_TYPE::SKILL_1);
			g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_1, GetSkill());

			if (GetObj()->Animator3D()->GetCliTime(4) >= 0.9f)
			{
				m_bSkill_Start[0] = true;
			}
			if (m_vecAniClipTime[1] > GetObj()->Animator3D()->GetAnimClip(4).dTimeLength)
			{

				m_vecAniClipTime[1] = 0.0f;
				m_bSkill_Start[0] = false;
				SetSkill();
				g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_1, GetSkill());

			}
		}

		if (KEY_AWAY(KEY_TYPE::KEY_4))
		{
			cout << "메가슬레시!" << endl;
			MegaSlash();
			fdamage = 20.f;
			MpUiScale.x -= fdamage;
		}

		if (KEY_AWAY(KEY_TYPE::KEY_5))
		{
			cout << "표창!" << endl;
			Swing();
			fdamage = 20.f;
			MpUiScale.x -= fdamage;

		}

		if (KEY_AWAY(KEY_TYPE::KEY_6))
		{
			cout << "파이어볼!" << endl;
			FireBall();
			g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_2, true);
			g_net.Send_Skill_Packet(GetObj()->GetID(), PLAYER_ANI_TYPE::SKILL_2, false);
			fdamage = 20.f;
			MpUiScale.x -= fdamage;
		}

		if (KEY_AWAY(KEY_TYPE::KEY_7))
		{
			cout << "메테오!" << endl;
			Meteor();
			m_bMeteor2 = true;
			fdamage = 20.f;
			MpUiScale.x -= fdamage;
		}

		if (KEY_AWAY(KEY_TYPE::KEY_8))
		{
			UnleashedPower();
			fdamage = 20.f;
			MpUiScale.x -= fdamage;

			CGameObject* pObject = nullptr;
			//	Particle
			pObject = new CGameObject;
			pObject->SetName(L"Particle");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CParticleSystem);
			pObject->Particlesystem()->SetFrequency(0.1f);
			pObject->Particlesystem()->SetType(false);
			pObject->Particlesystem()->SetMaxParticle(60);
			pObject->AddComponent(new CParticleScript);
			pObject->GetScript<CParticleScript>()->SetLifeTime(pObject->Particlesystem()->GetMaxLifeTime());
			pObject->FrustumCheck(false);

			Vec3 particlePos = Vec3(100.f, 300.f, 10.f);

			pObject->Transform()->SetLocalPos(particlePos);
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);

		}

		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
		{
			vRot.y += vDrag.x * DT * 0.5f;
			g_net.Send_Rotate_Packet(g_myid, vRot);
			player->Transform()->SetLocalRot(vRot);
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
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest3_1.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox3_2 = pObject;




				}
				if (m_iClearCnt == QUEST::THIRD && m_Q_Cnt == 2)
				{
					m_pQuestBox3_2->SetDead();
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
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest3_2.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBox3_3 = pObject;


				}
				if (m_iClearCnt == QUEST::THIRD && m_Q_Cnt == 3)
				{
					m_pQuestBox3_3->SetDead();
					m_Q_Cnt = 0;
					m_bQuest_03_clear = true;

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
					pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestexplane3_0.GetPointer());

					// AddGameObject
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
					m_pQuestBoxExplane3 = pObject;



				}
			}
		}

		//마나달기 UI
		pManaobj->SetDead();
		Ptr<CTexture> pMana = CResMgr::GetInst()->Load<CTexture>(L"Mana", L"Texture\\HpUi\\Mana.png");


		tResolution res = CRenderMgr::GetInst()->GetResolution();
		CGameObject* pObject = new CGameObject;
		Ptr<CMaterial>  pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject = new CGameObject;
		pObject->SetName(L"MpUi");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		ftempmp = fdamage * 2;
		pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f) - ftempmp, res.fHeight / 2.7f, 1.f));
		pObject->Transform()->SetLocalScale(MpUiScale);

		//MeshRender 설정

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pMana.GetPointer());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		pManaobj = pObject;


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

		if (!m_bColCheck)
		{
			Transform()->SetLocalPos(localPos);
		}
		else
		{
			Vec3 dir_p_f = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
			if (m_pColObj->GetObj()->GetName() == L"Map Object")
			{

				switch (m_pColObj->GetPlane())
				{
				case COL_PLANE::Z_PLANE:
					cout << "여기1 " << endl;
					localPos.z -= dir_p_f.z * m_fSpeed * DT;
					break;
				case COL_PLANE::X_PLANE:
					cout << "여기2 " << endl;
					localPos.x -= dir_p_f.x * m_fSpeed * DT;
					break;
				default:
					break;
				}
			}

			if (m_pColObj->GetObj()->GetName() == L"M_Monster"
				|| m_pColObj->GetObj()->GetName() == L"M_Monster2"
				)
			{
				Vec3 Col_Pos_1 = localPos;
				Vec3 Col_Pos_2 = m_pColObj->Transform()->GetLocalPos();
				Vec3 CNormal_1 = Col_Pos_2 - Col_Pos_1;
				CNormal_1.Normalize();
				Vec3 CNormal_2 = -CNormal_1;
				Vec3 Dir = WorldDir + CNormal_1;
				Dir.Normalize();
				int a = 0;

				Vec3 Reflect_vec = WorldDir + 2 * CNormal_2 * (Dot(-WorldDir, CNormal_2));

				switch (m_eDir)
				{
				case COL_DIR::FRONT:

					Dot(-WorldDir, CNormal_2) >= 0 ?
						localPos += Reflect_vec * m_fSpeed * DT :
						localPos += WorldDir * m_fSpeed * DT;
					break;
				case COL_DIR::BACK:
					Dot(-WorldDir, CNormal_2) >= 0 ?
						localPos -= WorldDir * m_fSpeed * DT :
						localPos -= Reflect_vec * m_fSpeed * DT;
					break;
				case COL_DIR::RIGHR:
					break;
				case COL_DIR::LEFT:
					break;
				default:
					break;
				}
				localPos.y = 0.0f;
			}
			Transform()->SetLocalPos(localPos);
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

void CPlayerScript::SetPlayerAnimation(const int& i)
{
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);
	GetObj()->MeshRender()->SetMesh(m_pAniData[i]);
}

void CPlayerScript::SetPlayerAnimation(int other_id, int i)
{
	//if (m_pAniData.size() == 0)	return;
	GameObject.find(other_id)->second->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GameObject.find(other_id)->second->Animator3D()->SetAnimClip(&m_pVecAnimClip);
	GameObject.find(other_id)->second->MeshRender()->SetMesh(m_pAniData[i]);
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
}

void CPlayerScript::AnimationPlay(int other_id, const PLAYER_ANI_TYPE& type)
{
	if (type == PLAYER_ANI_TYPE::IDLE)
	{
		GetObj()->Animator3D()->SetCurClip(0);
		SetPlayerAnimation(other_id, 0);
	}
	if (type == PLAYER_ANI_TYPE::WALK)
	{
		GetObj()->Animator3D()->SetCurClip(1);
		SetPlayerAnimation(other_id, 1);
	}
	if (type == PLAYER_ANI_TYPE::RUN)
	{
		GetObj()->Animator3D()->SetCurClip(2);
		SetPlayerAnimation(other_id, 2);
	}
	if (type == PLAYER_ANI_TYPE::ATTACK)
	{
		GetObj()->Animator3D()->SetCurClip(3);
		SetPlayerAnimation(other_id, 3);
	}
	if (type == PLAYER_ANI_TYPE::SKILL_1)
	{
		GetObj()->Animator3D()->SetCurClip(4);
		SetPlayerAnimation(other_id, 4);
	}
}

void CPlayerScript::OnCollisionEnter(CCollider2D* _pOther)
{
}

void CPlayerScript::OnCollision(CCollider2D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"M_Monster" 
		|| _pOther->GetObj()->GetName() == L"M_Monster2"
		|| _pOther->GetObj()->GetName() == L"Map Object"
		)
	{
		m_bColCheck = true;
		SetColObj(_pOther);
		Vec3 dir_vec = m_pColObj->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
		//cout << "충돌" << endl;

	
	}
	
	
	
	

}

void CPlayerScript::OnCollisionExit(CCollider2D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"M_Monster"
		|| _pOther->GetObj()->GetName() == L"M_Monster2"
		|| _pOther->GetObj()->GetName() == L"Map Object"
		|| _pOther->GetObj()->GetName() == L"FM_Monster")
	{
		m_bColCheck = false;
	}
}

void CPlayerScript::SwordStrike()
{
	cout << "소드 스트라이크" << endl;
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	//Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	//Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");

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
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


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
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);

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
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);
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
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(300.f, 300.f, 300.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


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


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);

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
	m_pSwordStrike->AddComponent(new CSwing2);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);

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


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);

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
	m_pSwordStrike->AddComponent(new CSwing4);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);
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
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);
}

void CPlayerScript::FireBall()
{
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"FireBall");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,50.f,0.f });
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CFireBall);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);

}

void CPlayerScript::UnleashedPower()
{
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 150 + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 100);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100 + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 150 - this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 100);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


	m_pSwordStrike = new CGameObject;



	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"UnleashedPower");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,300.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100 - this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CUnleashedPower);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);
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
	





	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Meteor");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,800.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 1000);
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CMeteor);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


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
	switch (questNum) {
	case QUEST::FIRST: {
		//g_net.Send_Stop_Packet(false, GetObj()->GetID());

		cout << "-------------------------------------------------" << endl;
		cout << "      첫번째 퀘스트" << endl;
		cout << "   문 앞에 있는 병사를 처치하라!" << endl;
		cout << "-------------------------------------------------" << endl;

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
		cout << "-------------------------------------------------" << endl;
		cout << "      두번째 퀘스트" << endl;
		cout << "   중앙 홀에 있는 병사를 처치하라!" << endl;
		cout << "-------------------------------------------------" << endl;


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
