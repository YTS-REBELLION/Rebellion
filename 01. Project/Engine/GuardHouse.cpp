#include "stdafx.h"
#include "GuardHouse.h"



CGuardHouse::CGuardHouse()
	: CScript((UINT)SCRIPT_TYPE::OBSTACLESCRIPS)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CGuardHouse::~CGuardHouse()
{
}

void CGuardHouse::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CGuardHouse::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = 29.85f;
	vRot.y = 26.58f;
	//vRot.y += 1.f*DT;
	
	//cout << "Rot:" << vRot.y << endl;
	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}
