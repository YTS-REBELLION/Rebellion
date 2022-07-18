#include "stdafx.h"
#include "UnleashedPower.h"



CUnleashedPower::CUnleashedPower()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CUnleashedPower::~CUnleashedPower()
{
}

void CUnleashedPower::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CUnleashedPower::update()
{
	m_fcreate_time += DT;
	if (m_fcreate_time >= 4.f)
	{
		GetObj()->SetDead();
	}

	if (m_fcreate_time >= 2.f)
	{
		Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 localPos = Transform()->GetLocalPos();

		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();
		float Temp = 500 * DT;




		localPos += WorldDir * Temp;



		Transform()->SetLocalPos(localPos);
		Transform()->SetLocalRot(vRot);
	}


}