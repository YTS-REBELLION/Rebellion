#include "stdafx.h"
#include "SwordStrike.h"
#include "PlayerScript.h"

CSwordStrike::CSwordStrike()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CSwordStrike::~CSwordStrike()
{
}

void CSwordStrike::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CSwordStrike::update()
{
	//if (m_fcreate_time >= 3.f)
	//{
	//	GetObj()->SetDead();
	//}

	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 localPos = Transform()->GetLocalPos();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	m_fcreate_time += DT;

	if (m_pPlayer->GetScript<CPlayerScript>()->GetSkillStrat(0))
	{
		localPos -= WorldDir * m_fspeed * DT;
		Transform()->SetLocalPos(localPos);
		Transform()->SetLocalRot(Vec3(vRot.x, vRot.y, vRot.z));
	}
	else if(!m_pPlayer->GetScript<CPlayerScript>()->GetSkillStrat(0) && m_fcreate_time >= 1.f)
		GetObj()->SetDead();

}