#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"

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

	}

void CPlayerScript::update()
{
	//Vec3 WorldDir;
	//Vec3 localPos = Transform()->GetLocalPos();
	//CTransform* playerTrans = Transform();

	//Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	//Vec3 vRot = Transform()->GetLocalRot();

	////Vec3 vPos = Transform()->GetLocalPos();
	////Vec3 vRot = Transform()->GetLocalRot();

	//if (KEY_HOLD(KEY_TYPE::KEY_W))
	//{
	//	WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
	//	localPos += WorldDir * 200.f * DT;
	//}

	//if (KEY_HOLD(KEY_TYPE::KEY_S))
	//{
	//	WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
	//	localPos += WorldDir * 200.f * DT;
	//}

	//if (KEY_HOLD(KEY_TYPE::KEY_A))
	//{	
	//	WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
	//	localPos += WorldDir * 200.f * DT;
	//}

	//if (KEY_HOLD(KEY_TYPE::KEY_D))
	//{
	//	WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
	//	localPos += WorldDir * 200.f * DT;
	//}

	//if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	//{
	//	vRot.y += vDrag.x * DT * 0.5f;
	//	Transform()->SetLocalRot(vRot);
	//}

	//Transform()->SetLocalPos(localPos);

	////Transform()->SetLocalRot(vRot);


	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();
	Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3 vLook = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vRightLook = vLook + vRight;
	Vec3 vLeftLook = vLook - vRight;
	vRight = Vector3::Normalize(vRight);
	vRightLook = Vector3::Normalize(vRightLook);
	vLeftLook = Vector3::Normalize(vLeftLook);
	vLook = Vector3::Normalize(vLook);

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{

		vPos += 2.5 * vLook;

		if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			vRot.y -= 2.5*DT * XM_PI;

		}
		else if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			vRot.y += 2.5 * DT * XM_PI;
		}


	}
	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{

		vPos -= 2.5 * vLook;

		if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			vRot.y += 2.5 * DT * XM_PI;

		}
		else if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			vRot.y -= 2.5 * DT * XM_PI;
		}


	}

	else if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		vPos -= 2.5 * vRight;

	}

	else if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		vPos += 2.5 * vRight;

	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);

}
