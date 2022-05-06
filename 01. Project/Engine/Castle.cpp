#include "stdafx.h"
#include "Castle.h"



CCastle::CCastle()
	: CScript((UINT)SCRIPT_TYPE::OBSTACLESCRIPS)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CCastle::~CCastle()
{
}

void CCastle::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CCastle::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.y = 28.29;
	//cout << "Rot:" << vRot.y << endl;
	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}
