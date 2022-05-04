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


	Vec3 vRot = Transform()->GetLocalRot();
	vRot.y = -78.57f;
	Transform()->SetLocalRot(vRot);






	}

void CPlayerScript::update()
{
	
	
	

	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();
	
	//Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();
	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_A))
	{	
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}
	else
	{
		SetPlayerAnimation(0);
	}


	else if (KEY_TAB(KEY_TYPE::KEY_ENTER))
	{
		vPos = { 0.f,0.f,0.f };
		vPos.y += 1000.f;
		
	}

	cout << "PlatervPos:" << vPos.x<<","<< vPos.y<<","<<vPos.z << endl;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);

	if (KEY_HOLD(KEY_TYPE::KEY_SPACE))
	{
		SetPlayerAnimation(3);
	}

	Transform()->SetLocalPos(localPos);



	//Transform()->SetLocalRot(vRot);
}

void CPlayerScript::SetPlayerAnimation(const int i)
{
	if (m_pAniData.size() == 0)	return;
	Animator3D()->SetBones(m_pAniData[i]->GetBones());
	Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
	MeshRender()->SetMesh(m_pAniData[i]);
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

	Transform()->SetLocalPos(localPos);

	//cout << GetPlayerDir().x << ", " << GetPlayerDir().y << ", " << GetPlayerDir().z << endl;
	//cout << endl;
	//cout << localPos.x << ", " << localPos.y << ", " << localPos.z << endl;
	//cout << "뭘 할 수 있을까?" << endl;
}

void CPlayerScript::OnCollisionExit(CCollider2D* _pOther)
{

	cout << "충돌 해제" << endl;
}