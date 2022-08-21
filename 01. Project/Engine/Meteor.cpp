#include "stdafx.h"
#include "Meteor.h"
#include"ParticleSystem.h"
#include"ParticleScript.h"
CMeteor::CMeteor()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{



}

CMeteor::~CMeteor()
{
}

void CMeteor::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CMeteor::update()
{

	m_fcreate_time += DT;
	if (m_fcreate_time >= 5.f)
	{
		GetObj()->SetDead();
	}


	Vec3 WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = Transform()->GetLocalPos();

	
	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	localPos.y -= 150 * DT;



	if (m_fcreate_time >= 0.f)
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Particle");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CParticleSystem);
		pObject->Particlesystem()->SetFrequency(2.f);
		pObject->Particlesystem()->SetType(false);
		pObject->Particlesystem()->SetMaxParticle(2);
		pObject->Particlesystem()->SetMaxScale(50);
		pObject->AddComponent(new CParticleScript);

		pObject->FrustumCheck(false);
		Vec3 particlePos = localPos;
		pObject->Transform()->SetLocalPos(Vec3{ particlePos.x,particlePos.y+150.f,particlePos.z});
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"Particle");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CParticleSystem);
		pObject->Particlesystem()->SetFrequency(2.f);
		pObject->Particlesystem()->SetType(false);
		pObject->Particlesystem()->SetMaxParticle(2);
		pObject->Particlesystem()->SetMaxScale(150);
		pObject->AddComponent(new CParticleScript);

		pObject->FrustumCheck(false);
		particlePos = localPos;
		pObject->Transform()->SetLocalPos(Vec3{ particlePos.x,particlePos.y,particlePos.z });
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"Particle");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CParticleSystem);
		pObject->Particlesystem()->SetFrequency(2.f);
		pObject->Particlesystem()->SetType(false);
		pObject->Particlesystem()->SetMaxParticle(2);
		pObject->Particlesystem()->SetMaxScale(100);
		pObject->AddComponent(new CParticleScript);

		pObject->FrustumCheck(false);
		particlePos = localPos;
		pObject->Transform()->SetLocalPos(Vec3{ particlePos.x,particlePos.y+75,particlePos.z });
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);

	}

	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);
	

}