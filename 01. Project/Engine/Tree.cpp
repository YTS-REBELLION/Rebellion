#include "stdafx.h"
#include "Tree.h"
CTree::CTree()
	: CScript((UINT)SCRIPT_TYPE::OBSTACLESCRIPS)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CTree::~CTree()
{
}

void CTree::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CTree::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = 17.13f;
	
	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}