#include "stdafx.h"
#include "BossFire.h"
#include"PlayerScript.h"

CBossFire::CBossFire()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CBossFire::~CBossFire()
{
}

void CBossFire::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CBossFire::update()
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

	
	Vec3 WorldDir = m_pPlayer->Transform()->GetLocalPos() - Transform()->GetLocalPos();
	Vec3 localPos = Transform()->GetLocalPos();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	float Temp = 50 * DT;




	localPos += WorldDir * Temp;



	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}