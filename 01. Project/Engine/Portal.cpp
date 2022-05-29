#include "stdafx.h"
#include "Portal.h"


CPortal::CPortal()
	: CScript((UINT)SCRIPT_TYPE::PORTALSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CPortal::~CPortal()
{
}

void CPortal::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CPortal::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	vRot.x = 17.13f;
	vRot.y = 21.95f;



	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}