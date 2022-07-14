#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include"CollisionMgr.h"
#include"SwordStrike.h"
#include"MegaSlash.h"
#include"Sting.h"
#include"Swing.h"
#include"FireBall.h"
#include"Meteor.h"
#include"UnleashedPower.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_bAttack(false)
	, m_iCulidx(0)
{	
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CPlayerScript::update()
{




	// Z-up To Y-up
	Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);

	Vec3 WorldDir;
	Vec3 localPos = GetObj()->Transform()->GetLocalPos();
	Vec3 localRot = GetObj()->Transform()->GetLocalRot();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	CPlayerScript* player = GetObj()->GetScript<CPlayerScript>();


	if (m_isMain) {
		if ((KEY_TAB(KEY_TYPE::KEY_W) || KEY_TAB(KEY_TYPE::KEY_A) || KEY_TAB(KEY_TYPE::KEY_S) || KEY_TAB(KEY_TYPE::KEY_D)))
		{
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
		}

		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
			localPos += WorldDir * m_fSpeed * DT;

			system_clock::time_point start = system_clock::now();
			m_eDir == COL_DIR::UP;

			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				localPos += WorldDir * m_fSpeed * DT;
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
		else if (GetAttack()&& m_vecAniClipTime[0] <GetObj()->Animator3D()->GetAnimClip(3).dTimeLength)
		{
			m_vecAniClipTime[0] += (DT*1.5f);
			AnimationPlay(PLAYER_ANI_TYPE::ATTACK);

			if (m_vecAniClipTime[0] > GetObj()->Animator3D()->GetAnimClip(3).dTimeLength)
			{
				m_vecAniClipTime[0] = 0.0f;
				SetAttack();
			}
		}
		

		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
		{
			vRot.y += vDrag.x * DT * 0.5f;
			g_net.Send_Rotate_Packet(g_myid, vRot);
			player->Transform()->SetLocalRot(vRot);
		}

		if (KEY_HOLD(KEY_TYPE::KEY_ENTER))
		{
			localPos.x = 0.f;

			localPos.y = 5000.f;
			vRot.y = XM_PI;

			localPos.z = 600.f;
			player->Transform()->SetLocalRot(vRot);
		}


	if (KEY_AWAY(KEY_TYPE::KEY_2))
	{
		cout << "소드스트라이크!" << endl;
		SwordStrike();


	}
	if (KEY_AWAY(KEY_TYPE::KEY_3))
	{
		cout << "메가슬레시!" << endl;
		MegaSlash();


	}
	if (KEY_AWAY(KEY_TYPE::KEY_4))
	{
		cout << "가세연!" << endl;
		Swing();


	}
	if (KEY_AWAY(KEY_TYPE::KEY_5))
	{
		cout << "찌르기!" << endl;
		Sting();


	}
	if (KEY_AWAY(KEY_TYPE::KEY_6))
	{
		cout << "파이어볼!" << endl;
		FireBall();


	}
	if (KEY_AWAY(KEY_TYPE::KEY_7))
	{
		cout << "메테오!" << endl;
		Meteor();
		m_bMeteor2 = true;

	}
	if (KEY_AWAY(KEY_TYPE::KEY_8))
	{
		UnleashedPower();


	}

	Ptr<CTexture> pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"QuestBase", L"Texture\\Quest\\QuestBase.png");
	Ptr<CTexture> pQuest = CResMgr::GetInst()->Load<CTexture>(L"Quest", L"Texture\\Quest\\UIboard.png");
	
	if (KEY_TAB(KEY_TYPE::KEY_Q))
	{
		++m_Q_Cnt;
		if (m_iClearCnt==0&& m_Q_Cnt%2==1)
		{
			tResolution res = CRenderMgr::GetInst()->GetResolution();
			Vec3	QuestBoxScale = Vec3(300.f, 200.f, 1.f);
			Vec3	QuestBoxinScale = Vec3(600.f, 400.f, 1.f);
			CGameObject* pObject = new CGameObject;
			
			pObject->SetName(L"QuestBoxBase");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);
			
			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (QuestBoxScale.x / 2.f), (res.fHeight / 2.f) - (QuestBoxScale.y / 2.f), 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxScale);

			//MeshRender 설정
			float fUI = 1.0f;
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestBox.GetPointer());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fUI);
			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

			m_pQuestBoxBase = pObject;
			//pObject->SetCameraProj();


			pObject = new CGameObject;
			pObject->SetName(L"QuestBox");
			pObject->FrustumCheck(false);
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);
			
			pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (QuestBoxinScale.x / 2.f), (res.fHeight / 2.f) - (QuestBoxinScale.y / 2.f), 1.f));
			pObject->Transform()->SetLocalScale(QuestBoxinScale);

			//MeshRender 설정
			
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			
			Ptr<CMaterial> pMtrl2 = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
			pObject->MeshRender()->SetMaterial(pMtrl2->Clone());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuest.GetPointer());
			
			// AddGameObject
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

			m_pQuestBox0 = pObject;


		}
		if (m_Q_Cnt % 2 == 0)
		{
			m_pQuestBoxBase->SetDead();
			m_pQuestBox0->SetDead();
		}
		

	}


	
	if (!m_bColCheck)
	{
		Transform()->SetLocalPos(localPos);

		


	}
	else
	{
		if (m_eDir== COL_DIR::UP)
		{
			localPos -= WorldDir * m_fSpeed * 15 * DT;
			Transform()->SetLocalPos(localPos);
		}

			if (m_eDir == COL_DIR::LEFT)
			{
				localPos -= WorldDir * m_fSpeed * 15 * DT;
				Transform()->SetLocalPos(localPos);
			}

			if (m_eDir == COL_DIR::DOWN)
			{
				localPos += WorldDir * m_fSpeed * 15 * DT;
				Transform()->SetLocalPos(localPos);
			}

			if (m_eDir == COL_DIR::RIGHT)
			{
				localPos += WorldDir * m_fSpeed * 15 * DT;
				Transform()->SetLocalPos(localPos);
			}

	}
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
	
	
	cout << "플레이어위치x:" << Transform()->GetWorldPos().x << endl;
	cout << "플레이어위치y:" << Transform()->GetWorldPos().y << endl;
	cout << "플레이어위치z:" << Transform()->GetWorldPos().z << endl;
	
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
}

void CPlayerScript::SwordStrike()
{
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");
	

	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"SwordStike");
	m_pSwordStrike->FrustumCheck(false);
	
	
	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos());
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwordStrike);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);


	


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"SwordStike");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos());
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot() + Vec3(0.f,15.f,0.f));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwordStrike);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);
	


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"SwordStike");
	m_pSwordStrike->FrustumCheck(false);

	
	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos());
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot() + Vec3(0.f, -15.f, 0.f));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwordStrike);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

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


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{0.f,50.f,0.f});
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalDir(DIR_TYPE::FRONT));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
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
	//// ====================
	////  오브젝트 생성
	//// ====================
	CGameObject* m_pSwordStrike = new CGameObject;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");
	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");


	m_pSwordStrike = pPMeshData->Instantiate();
	m_pSwordStrike->SetName(L"Swing");
	m_pSwordStrike->FrustumCheck(false);


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + Vec3{ 0.f,50.f,0.f });
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalRot());
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwing);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

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


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + this->Transform()->GetWorldDir(DIR_TYPE::UP) * 200);
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


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + this->Transform()->GetWorldDir(DIR_TYPE::UP) * 150 + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 100);
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


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + this->Transform()->GetWorldDir(DIR_TYPE::UP) * 100 + this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200);
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


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + this->Transform()->GetWorldDir(DIR_TYPE::UP) * 150 - this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 100);
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


	m_pSwordStrike->Transform()->SetLocalPos(this->Transform()->GetLocalPos() + this->Transform()->GetWorldDir(DIR_TYPE::UP) * 100 - this->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200);
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
	pMagicJin->Transform()->SetLocalPos(Vec3(this->Transform()->GetLocalPos() + this->Transform()->GetWorldDir(DIR_TYPE::FRONT)*1000));
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


void CPlayerScript::OnCollisionEnter(CCollider2D* _pOther)
{
}

void CPlayerScript::OnCollision(CCollider2D* _pOther)
{
	cout << "충돌" << endl;
	m_bColCheck = true;


}

void CPlayerScript::OnCollisionExit(CCollider2D* _pOther)
{

	cout << "충돌 해제" << endl;

	m_bColCheck = false;
}