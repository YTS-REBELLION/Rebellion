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

void CPlayerScript::Move_Player()
{
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();
	Vec3 vRight=Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3 vLook = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vRightLook = vLook + vRight;
	Vec3 vLeftLook = vLook - vRight;
	vRight=Vector3::Normalize(vRight);
	vRightLook = Vector3::Normalize(vRightLook);
	vLeftLook = Vector3::Normalize(vLeftLook);
	vLook = Vector3::Normalize(vLook);
	
	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{

		vPos += vLook;

		if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			vRot.y -= DT * XM_PI;

		}
		else if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			vRot.y += DT * XM_PI;
		}


	}
	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{

		vPos -= vLook;

		if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			vRot.y += DT * XM_PI;

		}
		else if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			vRot.y -= DT * XM_PI;
		}


	}

	


	

	/*if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		vPos.z += DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		vPos.z -= DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		vPos.x -= DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		vPos.x += DT * 200.f;
	}*/

	// z 키를 누르면 z 축 회전
	if (KEY_HOLD(KEY_TYPE::KEY_Y))
	{
		vRot.y += DT * XM_PI;

		// 복사 메테리얼을 MeshRender 에 세팅
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Y))
	{
		// z 키를 떼면 원레 메테리얼로 돌아감
		MeshRender()->SetMaterial(m_pOriginMtrl);
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
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
	Move_Player();

	

}
