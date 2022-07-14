#include "stdafx.h"
#include "MegaSlash.h"

CMegaSlash::CMegaSlash()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CMegaSlash::~CMegaSlash()
{
}

void CMegaSlash::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CMegaSlash::update()
{
	m_fcreate_time += DT;
	if (m_fcreate_time >= 1.5f)
	{
		GetObj()->SetDead();
	}


	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = Transform()->GetLocalPos();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	 
	vRot.y += DT*2;


	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}