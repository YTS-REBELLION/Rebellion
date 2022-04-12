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
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	//Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();

	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * 200.f * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * 200.f * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	{	
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * 200.f * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * 200.f * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		vRot.y += vDrag.x * DT * 0.5f;
		Transform()->SetLocalRot(vRot);
	}

	Transform()->SetLocalPos(localPos);

	//Transform()->SetLocalRot(vRot);
}
