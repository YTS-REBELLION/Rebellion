#include "stdafx.h"
#include "Somethings.h"



CSomethings::CSomethings()
	: CScript((UINT)SCRIPT_TYPE::OBSTACLESCRIPS)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CSomethings::~CSomethings()
{
}

void CSomethings::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CSomethings::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = 29.85f;
	vRot.y = 30.01f;
	//vRot.y += 1.f*DT;

	//cout << "Rot:" << vRot.y << endl;
	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}