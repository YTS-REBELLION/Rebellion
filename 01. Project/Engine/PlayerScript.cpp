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

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CPlayerScript::update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
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
	}

	// z 키를 누르면 z 축 회전
	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		vRot.z += DT * XM_PI;
							
		// 복사 메테리얼을 MeshRender 에 세팅
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		// z 키를 떼면 원레 메테리얼로 돌아감
		MeshRender()->SetMaterial(m_pOriginMtrl);		
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);

}
