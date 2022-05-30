#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include"CollisionMgr.h"

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
				g_net.Send_Run_Packet(GetObj()->GetID(),localPos ,true);
				

			}
			else { 
				
				player->SetPlayerAnimation(1);
				g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

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
				g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);


			}
			else {

				player->SetPlayerAnimation(1);
				g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

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
				g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);

			}
			else {

				player->SetPlayerAnimation(1);
				g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

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
				g_net.Send_Run_Packet(GetObj()->GetID(), localPos, true);


			}
			else {

				player->SetPlayerAnimation(1);
				g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

			};
		}
		else
		{
			player->SetPlayerAnimation(0);
		}

		if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_D)))
		{
			cout << "KET_AWAY" << endl;
			g_net.Send_Stop_Packet(false, g_myid);
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
		}
		if (player->GetAttack() && m_vecAniClipTime[0] < GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
			m_vecAniClipTime[0] += DT;

		GetObj()->Collider2D()->SetOffsetPos(Vec3(0.f, 20.f, 70.f));
		GetObj()->Collider2D()->SetOffsetScale(Vec3(800.f, 1150.f, 1700.f));

			g_net.Send_Attack_Animation_Packet(GetObj()->GetID(), player->GetAttack());

			player->SetPlayerAnimation(3);

			if (m_vecAniClipTime[0] > player->Animator3D()->GetAnimClip(0).dTimeLength)
			{
				m_vecAniClipTime[0] = 0.f;
				GetObj()->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
				GetObj()->Collider2D()->SetOffsetScale(Vec3(800.f, 850.f, 1700.f));

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
	
}

void CPlayerScript::SetPlayerAnimation(const int& i)
{
	//if (m_pAniData.size() == 0)	return;
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
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
	//m_fRTT = rtt;
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