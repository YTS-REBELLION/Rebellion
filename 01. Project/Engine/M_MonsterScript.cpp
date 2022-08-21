#include "stdafx.h"
#include "M_MonsterScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include "PlayerScript.h"
#include "SwordScript.h"
#include"BossSkill1.h"
#include"Meteor.h"
#include"Fire.h"

CM_MonsterScript::CM_MonsterScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSMONSTERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_Is_Move(false)
	, m_Is_Attack(false)
	, m_bHit(false)
	, m_iCulidx(0)
	, m_bAniOk(false)
	, m_fAngle(0.0f)
	, m_pPlayer(nullptr)
{
	m_fHp = m_fMaxHp;
}

CM_MonsterScript::~CM_MonsterScript()
{
}

void CM_MonsterScript::init()
{
	Ptr<CTexture> pBossName = CResMgr::GetInst()->Load<CTexture>(L"BossName", L"Texture\\HpUi\\BossName_ingkells.png");
	Ptr<CTexture> pBossHp = CResMgr::GetInst()->Load<CTexture>(L"BossHp", L"Texture\\HpUi\\BossHp_2.png");

	tResolution res = CRenderMgr::GetInst()->GetResolution();
	CGameObject* pObject = new CGameObject;
	Vec3 NameUiScale = { 100,100,1 };
	Vec3 FrameUiScale = { 500,40,1 };

	//체력 UI
	pObject = new CGameObject;
	pObject->SetName(L"HpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f), res.fHeight / 2.5f, 1.f));
	pObject->Transform()->SetLocalScale(NameUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	Ptr<CMaterial>  pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBossName.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);


	//체력 UI
	pObject = new CGameObject;
	pObject->SetName(L"HpUi");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (res.fWidth / 1.5f), res.fHeight / 2.3f , 1.f));
	pObject->Transform()->SetLocalScale(FrameUiScale);

	//MeshRender 설정

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	  pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBossHp.GetPointer());

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

}

void CM_MonsterScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CM_MonsterScript::update()
{
	if (!m_bcenecheck)
		//시네끝나고 update
	{
		Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
		Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 vDirRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

		Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);
		Transform()->SetLocalDir(DIR_TYPE::FRONT, vDirUp);

		Transform()->SetWorldDir(DIR_TYPE::UP, -vDirRight);
		Transform()->SetLocalDir(DIR_TYPE::UP, -vDirRight);

		Transform()->SetWorldDir(DIR_TYPE::RIGHT, -vDirFront);
		Transform()->SetLocalDir(DIR_TYPE::RIGHT, -vDirFront);

		Vec3 WorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 localPos = GetObj()->Transform()->GetLocalPos();
		Vec3 localRot = GetObj()->Transform()->GetLocalRot();
		CTransform* playerTrans = Transform();

		Vec3 WorldPos = GetObj()->Transform()->GetWorldPos();
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();

		CSceneMgr::GetInst()->FindPlayerPos(L"Player");
		float fDistanceP_M = Vec3::Distance(CSceneMgr::GetInst()->m_vSavePos, localPos);

		CM_MonsterScript* Monster = GetObj()->GetScript<CM_MonsterScript>();
		const vector<CGameObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetObjects();
		Vec3 vRot;

		if (m_isTarget) {
			for (auto client : CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetParentObj())
			{
				if (client->GetScript<CPlayerScript>()->GetID() == m_targetId)
				{
					m_pPlayer = client;
				}
			}

			m_fAngle = atan2(localPos.x - m_pPlayer->Transform()->GetLocalPos().x, localPos.z - m_pPlayer->Transform()->GetLocalPos().z) * (180 / XM_PI) * 0.0174532925f;//acosf(Dot(vDirFront, Monster_Nor));
		}

		//Transform()->SetLocalRot(Vec3(-m_fAngle+ XMConvertToRadians(-90.f), localRot.y, localRot.z));

		vRot = Vec3(localRot.x, m_fAngle, localRot.z);

		Monster->Transform()->SetLocalRot(vRot);


		if (m_Is_Move) {
			AnimationPlay(MONSTER_ANI_TYPE::WALK);
		}
		else if (!m_Is_Move && m_Is_Attack) {
			AnimationPlay(MONSTER_ANI_TYPE::ATTACK);
		}
		else
			AnimationPlay(MONSTER_ANI_TYPE::IDLE);

		m_fskillTime += DT;

		if (m_fskillTime < 5.f)
		{
			check = false;
		}

		if (m_fskillTime >= 5.f && !MonSkill1Check && !check)
		{
			m_skill = MonSkill1;
			MonSkill1Check = true;
			check = true;
		}

		if (m_fskillTime >= 10.f && !MonSkill2Check && check)
		{
			m_skill = MonSkill2;
			MonSkill2Check = true;
			check = false;
		}

		if (m_fskillTime >= 15.f && !MonSkill3Check && !check)
		{
			m_skill = MonSkill3;
			MonSkill3Check = true;
			check = true;


		}

		UpdateLerpPos();

		switch (m_skill)
		{
		case MonSkill1:
			Skill1();
			break;
		case MonSkill2:
			Skill2();
			break;
		case MonSkill3:
			Skill3();
			break;
		case End:
			break;
		Monster_Skill:
			break;
		}

	}
}
void CM_MonsterScript::UpdateLerpPos()
{
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Pos = Vec3::Lerp(Pos, LerpPos, 5 * DT);
	GetObj()->Transform()->SetLocalPos(Pos);
}

void CM_MonsterScript::SetMonsterAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame)
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

void CM_MonsterScript::SetMonsterAnimation(const int& i)
{
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);
	GetObj()->MeshRender()->SetMesh(m_pAniData[i]);
}

void CM_MonsterScript::SetMonsterAnimation(int other_id, const int& i)
{
	GameObject.find(other_id)->second->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GameObject.find(other_id)->second->Animator3D()->SetAnimClip(&m_pVecAnimClip);
	GameObject.find(other_id)->second->MeshRender()->SetMesh(m_pAniData[i]);
}

void CM_MonsterScript::AnimationPlay(const MONSTER_ANI_TYPE& type)
{
	if (type == MONSTER_ANI_TYPE::IDLE)
	{
		GetObj()->Animator3D()->SetCurClip(0);
		SetMonsterAnimation(0);
	}
	if (type == MONSTER_ANI_TYPE::WALK)
	{
		GetObj()->Animator3D()->SetCurClip(1);
		SetMonsterAnimation(1);
	}
	if (type == MONSTER_ANI_TYPE::HIT)
	{
		GetObj()->Animator3D()->SetCurClip(2);
		SetMonsterAnimation(2);
	}
	if (type == MONSTER_ANI_TYPE::ATTACK)
	{
		GetObj()->Animator3D()->SetCurClip(3);
		SetMonsterAnimation(3);
	}
}

void CM_MonsterScript::SetOtherMovePacket(sc_packet_move* p)
{
	m_movePacketTemp = new sc_packet_move;
	m_movePacketTemp = p;

}

void CM_MonsterScript::SetMonsterPacket(sc_packet_npc_attack* p)
{
	m_attackPacket = new sc_packet_npc_attack;

	m_attackPacket = p;
}


void CM_MonsterScript::OnCollisionEnter(CCollider2D* _pOther)
{
	//if (_pOther->GetObj()->GetName() == L"Player_Sword")
	//{
	//	//cout << "검과 충돌" << endl;
	//	m_bHit = true;
	//	//g_net.Send_Player2MonsterCol_Packet(GetID(), GetObj()->GetID(), true);

	//}
}

void CM_MonsterScript::OnCollision(CCollider2D* _pOther)
{

}

void CM_MonsterScript::OnCollisionExit(CCollider2D* _pOther)
{

}
void CM_MonsterScript::Skill1()
{
	if (MonSkill1Check)
	{
		//// ====================
		////  오브젝트 생성
		//// ====================
		//CGameObject* m_pSwordStrike = new CGameObject;
		//Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Sjuriken1.fbx");
	

		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"BossFire");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 50.f,0.f,50.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CBossSkill1);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);



		//m_pSwordStrike = new CGameObject;
		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"BossFire");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 50.f,0.f,-50.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CBossSkill1);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);


		//m_pSwordStrike = new CGameObject;
		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"BossFire");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ -50.f,0.f,50.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CBossSkill1);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);


		//m_pSwordStrike = new CGameObject;
		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"BossFire");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ -50.f,0.f,-50.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CBossSkill1);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);


			// ====================
	// Fire 오브젝트 생성
	// ====================
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"FireTest");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT));
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"FireTest2");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"FireTest3");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



		pObject = new CGameObject;
		pObject->SetName(L"FireTest4");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



		pObject = new CGameObject;
		pObject->SetName(L"FireTest5");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



		pObject = new CGameObject;
		pObject->SetName(L"FireTest6");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"FireTest7");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);
		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"FireTest8");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);
		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"FireTest9");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);
		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(0.6f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);





		MonSkill1Check = false;

	}
}

void CM_MonsterScript::Skill2()
{
	//메테오
	if (MonSkill2Check)
	{
		//CGameObject* m_pSwordStrike = new CGameObject;
		//Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
		//

		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 500.f,800.f,-500.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);


		//m_pSwordStrike = new CGameObject;



		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ -500.f,800.f,-500.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);

		//m_pSwordStrike = new CGameObject;



		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ -500.f,800.f,500.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);

		//m_pSwordStrike = new CGameObject;



		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 500.f,800.f,500.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);



		//m_pSwordStrike = new CGameObject;

		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 500.f,800.f,500.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);



		//m_pSwordStrike = new CGameObject;

		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 500.f,800.f,00.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);

		//m_pSwordStrike = new CGameObject;

		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ -500.f,800.f,0.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);

		//m_pSwordStrike = new CGameObject;

		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,800.f,500.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);


		//m_pSwordStrike = new CGameObject;

		//m_pSwordStrike = pPMeshData->Instantiate();
		//m_pSwordStrike->SetName(L"Meteor");
		//m_pSwordStrike->FrustumCheck(false);


		//m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,800.f,-500.f });
		//m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
		//m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		//m_pSwordStrike->AddComponent(new CMeteor);


		//m_pSwordStrike->AddComponent(new CCollider2D);
		//m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		//m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

		//// AddGameObject
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(m_pSwordStrike);

	// ====================
	// Fire 오브젝트 생성
	// ====================
CGameObject* pObject = new CGameObject;

pObject = new CGameObject;
pObject->SetName(L"FireTest");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);


pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT));
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

pObject = new CGameObject;
pObject->SetName(L"FireTest2");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);


pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

pObject = new CGameObject;
pObject->SetName(L"FireTest3");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);


pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



pObject = new CGameObject;
pObject->SetName(L"FireTest4");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);


pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



pObject = new CGameObject;
pObject->SetName(L"FireTest5");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);


pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



pObject = new CGameObject;
pObject->SetName(L"FireTest6");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);


pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);


pObject = new CGameObject;
pObject->SetName(L"FireTest7");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);
pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);


pObject = new CGameObject;
pObject->SetName(L"FireTest8");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);
pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

pObject = new CGameObject;
pObject->SetName(L"FireTest9");
pObject->FrustumCheck(false);
pObject->AddComponent(new CTransform);
pObject->AddComponent(new CMeshRender);
pObject->AddComponent(new CFire);
pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
// MeshRender 설정
pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
pObject->GetScript<CFire>()->init();
pObject->AddComponent(new CCollider2D);
pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
// AddGameObject
CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);


		MonSkill2Check = false;
	}
}

void CM_MonsterScript::Skill3()
{
	//불지르기
	if (MonSkill3Check)
	{
		// ====================
	// Fire 오브젝트 생성
	// ====================
		CGameObject* pObject = new CGameObject;

		pObject = new CGameObject;
		pObject->SetName(L"FireTest");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT));
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"FireTest2");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"FireTest3");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



		pObject = new CGameObject;
		pObject->SetName(L"FireTest4");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		

		pObject = new CGameObject;
		pObject->SetName(L"FireTest5");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);



		pObject = new CGameObject;
		pObject->SetName(L"FireTest6");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);


		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"FireTest7");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);
		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"FireTest8");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);
		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"FireTest9");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CFire);
		pObject->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,100.f,0.f } + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 500 + this->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 500);
		pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
		pObject->GetScript<CFire>()->init();
		pObject->AddComponent(new CCollider2D);
		pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHEREMESH);
		pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		pObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster_Skill")->AddGameObject(pObject);

		m_fskillTime = 0.f;
		MonSkill1Check = false;
		MonSkill2Check = false;
		MonSkill3Check = false;
	}

}