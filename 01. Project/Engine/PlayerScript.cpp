#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
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
	Transform()->SetWorldDir(DIR_TYPE::UP, vDirFront);
	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);

	Vec3 WorldDir;
	Vec3 localPos = GetObj()->Transform()->GetLocalPos();
	Vec3 localRot = GetObj()->Transform()->GetLocalRot();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	CPlayerScript* player = GetObj()->GetScript<CPlayerScript>();

	//Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();

	if (m_isMain) {
		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
			localPos += WorldDir * m_fSpeed * DT;

			system_clock::time_point start = system_clock::now();
			m_eDir == COL_DIR::UP;

			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				localPos += WorldDir * m_fSpeed * DT;
				player->SetPlayerAnimation(2);
				//g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);

			}
			else {
				player->SetPlayerAnimation(1);
				//player->SetPlayerAnimation(0, 0, 55);
				//g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			};
		}

		else if (KEY_HOLD(KEY_TYPE::KEY_S))
		{
			WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
			localPos += WorldDir * m_fSpeed * DT;


			system_clock::time_point start = system_clock::now();
			m_eDir == COL_DIR::DOWN;
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				localPos += WorldDir * m_fSpeed * DT;
				player->SetPlayerAnimation(2);
				//g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);


			}
			else {

				player->SetPlayerAnimation(1);
				//g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			};
		}

		else if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
			localPos += WorldDir * m_fSpeed * DT;

			system_clock::time_point start = system_clock::now();
			m_eDir == COL_DIR::LEFT;
			g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				localPos += WorldDir * m_fSpeed * DT;
				player->SetPlayerAnimation(2);
				//g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);

			}
			else {

				player->SetPlayerAnimation(1);
				//g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			};
		}

		else if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
			localPos += WorldDir * m_fSpeed * DT;

			system_clock::time_point start = system_clock::now();


			m_eDir == COL_DIR::RIGHT;
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				localPos += WorldDir * m_fSpeed * DT;
				player->SetPlayerAnimation(2);
				//g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);


			}
			else {

				player->SetPlayerAnimation(1);
				//g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			};
		}
		else
		{
			player->SetPlayerAnimation(0, 0, 55); // idle
			//player->SetPlayerAnimation(1, 0, 35); // walk
			//player->SetPlayerAnimation(2, 0, 21); // run
			//player->SetPlayerAnimation(3, 0, 20); // attack - true
			//player->SetPlayerAnimation(3, 0, 45); // attack - all
		}

		if (KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D))
			GetReckoner()->DeadReck(GetObj());
		if ((GetReckoner()->isFollowing() || !isReckoning) &&
			(KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D)))
		{
			isReckoning = true;
			system_clock::time_point start = system_clock::now();
			g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			GetReckoner()->SetDirVec(WorldDir);
			GetReckoner()->SetRotateY(vRot.y);
			GetReckoner()->SetLocalPos(GetObj()->Transform()->GetLocalPos());

		}


		if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_D)))
		{
			cout << "KET_AWAY" << endl;
			isReckoning = false;
			g_net.Send_Stop_Packet(false, GetObj()->GetID());
		}

		

		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
		{
			vRot.y += vDrag.x * DT * 0.5f;
			g_net.Send_Rotate_Packet(g_myid, vRot);
			player->Transform()->SetLocalRot(vRot);
		}

		if (KEY_TAB(KEY_TYPE::KEY_SPACE))
		{
			//player->GetObj()->Animator3D()->SetClipTime(0, 0.f);
			player->Animator3D()->SetClipTime(0, 0.f);
			player->SetAttack();
			player->m_bCol = true;
		}
		if (player->GetAttack() && m_vecAniClipTime[0] < GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
			m_vecAniClipTime[0] += DT;

		/*	GetObj()->Collider2D()->SetOffsetPos(Vec3(0.f, 20.f, 70.f));
			GetObj()->Collider2D()->SetOffsetScale(Vec3(800.f, 1150.f, 1700.f));*/

			g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), player->GetAttack());

			
			player->SetPlayerAnimation(3);
			//cout << player->GetCol() << endl;
			if (m_vecAniClipTime[0] > player->Animator3D()->GetAnimClip(0).dTimeLength - 1.0)
			{
				player->m_bCol = false;
			}
			if (m_vecAniClipTime[0] > player->Animator3D()->GetAnimClip(0).dTimeLength)
			{
				m_vecAniClipTime[0] = 0.f;
				//GetObj()->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
				//GetObj()->Collider2D()->SetOffsetScale(Vec3(800.f, 850.f, 1700.f));

				player->SetAttack();
				g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), player->GetAttack());

			}
		}

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

	

	/*cout << "플레이어위치x:" << Transform()->GetWorldPos().x << endl;
	cout << "플레이어위치y:" << Transform()->GetWorldPos().y << endl;
	cout << "플레이어위치z:" << Transform()->GetWorldPos().z << endl;*/
	
}

void CPlayerScript::SetPlayerAnimation(const int& i)
{
	//if (m_pAniData.size() == 0)	return;
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
	GetObj()->MeshRender()->SetMesh(m_pAniData[i]);
}

void CPlayerScript::SetPlayerAnimation(const int& i, const UINT& _StartFrame, const UINT& _EndFrame)
{
	//if (m_pAniData.size() == 0)	return;
	//GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	//GetObj()->Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
	//GetObj()->MeshRender()->SetMesh(m_pAniData[i]);

	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());

	tMTAnimClip* tNewAnimClip = new tMTAnimClip;
	tNewAnimClip->iStartFrame = _StartFrame;
	tNewAnimClip->iEndFrame = _EndFrame;
	tNewAnimClip->iFrameLength = _EndFrame - _StartFrame;
	tNewAnimClip->dStartTime = (double)_StartFrame / (double)30;
	tNewAnimClip->dEndTime = (double)_EndFrame / (double)30;
	tNewAnimClip->dTimeLength = tNewAnimClip->dEndTime - tNewAnimClip->dStartTime;

	m_pVecAnimClip.push_back(*tNewAnimClip);

	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);

	GetObj()->MeshRender()->SetMesh(m_pAniData[i]);

}

void CPlayerScript::SetPlayerAnimation(int other_id, int i)
{
	//if (m_pAniData.size() == 0)	return;
	GameObject.find(other_id)->second->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GameObject.find(other_id)->second->Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
	GameObject.find(other_id)->second->MeshRender()->SetMesh(m_pAniData[i]);
}

void CPlayerScript::SetOtherMovePacket(sc_packet_move* p, const float& rtt)
{
	m_movePacketTemp = new sc_packet_move;

	m_movePacketTemp = p;
	
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