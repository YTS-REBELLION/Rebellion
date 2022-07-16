#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include "SwordScript.h"
#include"CollisionMgr.h"
#include"SwordStrike.h"

bool isReckoning = false;
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
	pSwordObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pSwordObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pSwordObject->AddComponent(new CCollider2D);
	pSwordObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pSwordObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pSwordObject->Collider2D()->SetOffsetScale(Vec3(10.f, 140.f, 5.f));

	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");
	pSwordObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, SwordObject.GetPointer());


	pSwordObject->AddComponent(new CSwordScript);
	CSwordScript* SwordScript = pSwordObject->GetScript<CSwordScript>();
	pSwordObject->GetScript<CSwordScript>()->SetTarget(GetObj());
	pSwordObject->GetScript<CSwordScript>()->SetBoneIdx(25);

	CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", pSwordObject, false);
	GetObj()->AddChild(pSwordObject);
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
			g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), GetAttack());

			if (m_vecAniClipTime[0] > GetObj()->Animator3D()->GetAnimClip(3).dTimeLength)
			{
				m_vecAniClipTime[0] = 0.0f;
				SetAttack();
				g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), GetAttack());
			}
		}

		if (KEY_TAB(KEY_TYPE::KEY_2))
		{
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
			SetSkill();
		}
		else if (GetSkill()&&m_vecAniClipTime[1] < GetObj()->Animator3D()->GetAnimClip(4).dTimeLength)
		{
			m_vecAniClipTime[1] += (DT * 1.5f);
			AnimationPlay(PLAYER_ANI_TYPE::SKILL_1);

			if (m_vecAniClipTime[1] > GetObj()->Animator3D()->GetAnimClip(4).dTimeLength)
			{
				m_vecAniClipTime[1] = 0.0f;
				SetSkill();
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

		if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_D)))
		{
			cout << "KET_AWAY" << endl;
			g_net.Send_Stop_Packet(false, GetObj()->GetID());
		}

		/*if (!m_bColCheck)
		{
			Transform()->SetLocalPos(localPos);
		}
		else
		{
			if (m_eDir == COL_DIR::UP)
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

		}*/
	}

	Transform()->SetLocalPos(localPos);
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
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalDir(DIR_TYPE::FRONT));
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
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalDir(DIR_TYPE::FRONT) + Vec3(0.f,15.f,0.f));
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
	m_pSwordStrike->Transform()->SetLocalRot(this->Transform()->GetLocalDir(DIR_TYPE::FRONT) + Vec3(0.f, -15.f, 0.f));
	m_pSwordStrike->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pSwordStrike->AddComponent(new CSwordStrike);


	m_pSwordStrike->AddComponent(new CCollider2D);
	m_pSwordStrike->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	m_pSwordStrike->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_pSwordStrike->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pSwordStrike);

	








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