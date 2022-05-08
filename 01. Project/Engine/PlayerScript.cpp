#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
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
	Vec3 WorldDir;
	Vec3 localPos = GetObj()->Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	CPlayerScript* player = GetObj()->GetScript<CPlayerScript>();

	//Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * m_fSpeed * DT;

		system_clock::time_point start = system_clock::now();

		g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			player->SetPlayerAnimation(2);

		}
		else player->SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * m_fSpeed * DT;


		system_clock::time_point start = system_clock::now();

		g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);
		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			player->SetPlayerAnimation(2);
		}
		else player->SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_A))
	{	
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * m_fSpeed * DT;

		system_clock::time_point start = system_clock::now();

		g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			player->SetPlayerAnimation(2);
		}
		else player->SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * m_fSpeed * DT;

		system_clock::time_point start = system_clock::now();

		g_net.Send_Move_Packet(localPos, WorldDir, vRot.y, start, DT);
		
		
		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			player->SetPlayerAnimation(2);
		}
		else player->SetPlayerAnimation(1);
	}
	else
	{
		player->SetPlayerAnimation(0);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		vRot.y += vDrag.x * DT * 0.5f;
		player->Transform()->SetLocalRot(vRot);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_SPACE))
	{
		player->SetPlayerAnimation(3);
	}

	player->Transform()->SetLocalPos(localPos);


	//Transform()->SetLocalRot(vRot);
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


void CPlayerScript::OnCollisionEnter(CCollider2D* _pOther)
{
}

void CPlayerScript::OnCollision(CCollider2D* _pOther)
{
	BoundingSphere myBS = Collider2D()->GetBS();
	BoundingSphere otherBS = _pOther->Collider2D()->GetBS();

	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
	localPos -= WorldDir * m_fSpeed * DT * 2.0f;

	if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
	{
		localPos -= WorldDir * m_fSpeed * DT;
	}

	if (L"Portal" == _pOther->GetObj()->GetName())
	{
		localPos.y += 1000.f;
	}

	Transform()->SetLocalPos(localPos);

}

void CPlayerScript::OnCollisionExit(CCollider2D* _pOther)
{

	cout << "�浹 ����" << endl;
}