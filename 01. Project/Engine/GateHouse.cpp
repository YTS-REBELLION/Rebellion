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
	
	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}

void CGateHouse::OnCollisionEnter(CCollider2D* _pOther)
{
	cout << "?" << endl;
}

void CGateHouse::OnCollision(CCollider2D* _pOther)
{
	

	cout << "집충돌" << endl;

	

}

void CGateHouse::OnCollisionExit(CCollider2D* _pOther)
{

	cout << "충돌 해제" << endl;
}
