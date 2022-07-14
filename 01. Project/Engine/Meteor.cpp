#include "stdafx.h"
#include "Meteor.h"


CMeteor::CMeteor()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CMeteor::~CMeteor()
{
}

void CMeteor::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CMeteor::update()
{
	m_fcreate_time += DT;
	if (m_fcreate_time >= 5.f)
	{
		GetObj()->SetDead();
	}


	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = Transform()->GetLocalPos();

	
	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	localPos.y -= 150 * DT;

	

	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);
	

}