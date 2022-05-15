#include "stdafx.h"
#include "NpcMon.h"




CNpcMon::CNpcMon()
	: CScript((UINT)SCRIPT_TYPE::NPCSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CNpcMon::~CNpcMon()
{
}

void CNpcMon::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CNpcMon::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	vRot.y =-111.8f;
	

	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}