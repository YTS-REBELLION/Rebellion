#include "stdafx.h"
#include "BossSkill1.h"


CBossSkill1::CBossSkill1()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CBossSkill1::~CBossSkill1()
{
}

void CBossSkill1::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CBossSkill1::update()
{
	

	float Temp = 500 * DT;
	m_fcreate_time += DT;
	


	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = Transform()->GetLocalPos();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	vRot.y += DT;
	
	if (m_fcreate_time >= 3.f)
	{
		localPos += WorldDir * Temp;
	}
	if (m_fcreate_time >= 5.f)
	{
		GetObj()->SetDead();
	}
	

	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);




}