#include "stdafx.h"
#include "SwordStrike.h"
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
	m_fcreate_time += DT;
	if (m_fcreate_time >= 3.f)
	{
		GetObj()->SetDead();
	}


	Vec3 WorldDir= Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = Transform()->GetLocalPos();
	
	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	localPos.y += 50 * DT;


	float Temp = 75 * DT;


	if (this->Transform()->GetLocalDir(DIR_TYPE::FRONT).x >= 1.f)
	{
		WorldDir *= -1;
	}

	localPos += WorldDir * Temp;

	






	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}