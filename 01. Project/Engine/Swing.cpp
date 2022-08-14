#include "stdafx.h"
#include "Swing.h"
#include"PlayerScript.h"

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
	for (auto client : CSceneMgr::GetInst()->GetCurScene()->GetLayer(2)->GetParentObj())
	{
		if (client->GetScript<CPlayerScript>()->GetMain())
			m_pPlayer = client;
	}


	m_fcreate_time += DT;
	if (m_fcreate_time >= 3.f)
	{
		GetObj()->SetDead();
	}


	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = m_pPlayer->Transform()->GetLocalPos() + Vec3{ 100,100,100 };

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	vRot.y += DT;


	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);




}