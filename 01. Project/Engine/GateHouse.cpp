#include "stdafx.h"
#include "GateHouse.h"


CGateHouse::CGateHouse()
	: CScript((UINT)SCRIPT_TYPE::OBSTACLESCRIPS)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CGateHouse::~CGateHouse()
{
}

void CGateHouse::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CGateHouse::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = 17.26f;
	//vRot.x += 1.f*DT;
	//cout << "Rot:" << vRot.x << endl;
	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}
