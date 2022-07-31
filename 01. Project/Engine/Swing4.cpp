#include "stdafx.h"
#include "Swing4.h"


#include"PlayerScript.h"

CSwing4::CSwing4()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CSwing4::~CSwing4()
{
}

void CSwing4::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CSwing4::update()
{

	for (auto client : CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetParentObj())
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
	Vec3 localPos = m_pPlayer->Transform()->GetLocalPos() + Vec3{ -100,100,100 };

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	vRot.y += DT;


	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}