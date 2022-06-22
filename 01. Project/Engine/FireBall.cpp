#include "stdafx.h"
#include "FireBall.h"

CFireBall::CFireBall()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CFireBall::~CFireBall()
{
}

void CFireBall::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CFireBall::update()
{
	m_fcreate_time += DT;
	if (m_fcreate_time >= 2.f)
	{
		GetObj()->SetDead();
	}

	
	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 localPos = Transform()->GetLocalPos();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	float Temp = 500 * DT;


	

	localPos += WorldDir * Temp;



	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}