#include "stdafx.h"
#include "Swing.h"


CSwing::CSwing()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CSwing::~CSwing()
{
}

void CSwing::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CSwing::update()
{
	m_fcreate_time += DT;
	if (m_fcreate_time >= 1.f)
	{
		GetObj()->SetDead();
	}


	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = Transform()->GetLocalPos();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	


	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}