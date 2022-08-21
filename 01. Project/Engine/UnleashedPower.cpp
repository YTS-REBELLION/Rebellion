#include "stdafx.h"
#include "UnleashedPower.h"
#include"ParticleSystem.h"
#include"ParticleScript.h"


CUnleashedPower::CUnleashedPower()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CUnleashedPower::~CUnleashedPower()
{
}

void CUnleashedPower::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CUnleashedPower::update()
{
	m_fcreate_time += DT;
	if (m_fcreate_time >= 4.f)
	{
		GetObj()->SetDead();
	}

	if (m_fcreate_time >= 2.f)
	{
		Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::UP);
		Vec3 localPos = Transform()->GetLocalPos();

		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();
		float Temp = 500 * DT;




		localPos += WorldDir * Temp;
		if (m_fcreate_time >= 0.f)
		{
			CGameObject* pObject = new CGameObject;
			pObject->SetName(L"Particle");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CParticleSystem);
			pObject->Particlesystem()->SetFrequency(2.f);
			pObject->Particlesystem()->SetType(false);
			pObject->Particlesystem()->SetMaxParticle(10);
			pObject->Particlesystem()->SetMaxScale(20);
			pObject->Particlesystem()->SetStartColor(Vec4{1.f,0.8f,0.f,1.4f});
			pObject->Particlesystem()->SetEndColor(Vec4{ 0.8f,0.6f,0.f,0.8f });
			pObject->AddComponent(new CParticleScript);

			pObject->FrustumCheck(false);
			Vec3 particlePos = localPos;
			pObject->Transform()->SetLocalPos(Vec3{ particlePos.x + 20,particlePos.y,particlePos.z - 20 });
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);


			pObject = new CGameObject;
			pObject->SetName(L"Particle");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CParticleSystem);
			pObject->Particlesystem()->SetFrequency(2.f);
			pObject->Particlesystem()->SetType(false);
			pObject->Particlesystem()->SetMaxParticle(10);
			pObject->Particlesystem()->SetMaxScale(20);
			pObject->Particlesystem()->SetStartColor(Vec4{ 1.f,0.8f,0.f,1.4f });
			pObject->Particlesystem()->SetEndColor(Vec4{ 0.8f,0.6f,0.f,0.8f });
			pObject->AddComponent(new CParticleScript);

			pObject->FrustumCheck(false);
			particlePos = localPos;
			pObject->Transform()->SetLocalPos(Vec3{ particlePos.x - 20,particlePos.y,particlePos.z - 40 });
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);

		}


		Transform()->SetLocalPos(localPos);
		Transform()->SetLocalRot(vRot);
	}


}