#include "stdafx.h"
#include "PlayerScript.h"

#include "BulletScript.h"
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
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	
	//Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();
	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * 200.f * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * 200.f * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * 200.f * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * 200.f * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_A))
	{	
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * 200.f * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * 200.f * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * 200.f * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * 200.f * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}
	else
	{
		SetPlayerAnimation(0);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		vRot.y += vDrag.x * DT * 0.5f;
		Transform()->SetLocalRot(vRot);
	}

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
