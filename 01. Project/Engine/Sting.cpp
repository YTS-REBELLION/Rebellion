#include "stdafx.h"
#include "Sting.h"


CSting::CSting()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CSting::~CSting()
{
}

void CSting::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CSting::update()
{
	m_fcreate_time += DT;
	if (m_fcreate_time >= 0.2f)
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