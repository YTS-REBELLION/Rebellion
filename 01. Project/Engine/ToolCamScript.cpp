#include "stdafx.h"
#include "ToolCamScript.h"

#include "Camera.h"

#include "RenderMgr.h"
#include "PlayerScript.h"
#include <random>
#include"M_MonsterScript.h"

default_random_engine dre;
uniform_int_distribution<> uid{ -5,5 };

CToolCamScript::CToolCamScript()
	: CScript(0)
	, m_fSpeed(200.f)
	, m_fScaleSpeed(1.f)
{

	m_pPlayer = nullptr;
	m_VecObj.clear();
}

CToolCamScript::~CToolCamScript()
{
}
void CToolCamScript::CameraMode()
{
	switch (m_eCamState)
	{
	case CAMERASTATE::PLAYER:
		SetPlayerFixedCamera();
		break;
	case CAMERASTATE::FREE:
		SetFreeCamera();
		break;
	case CAMERASTATE::CENEMATIC00:
		SetCenematic0();
		break;
	case CAMERASTATE::CENEMATICBOSS:
		SetCenematicBoss();
		break;

	default:
		break;
	}
}
void CToolCamScript::update()
{
	
	CameraMode();

	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		m_eCamState = CAMERASTATE::FREE;
	}
	if (KEY_HOLD(KEY_TYPE::KEY_Y))
	{
		m_eCamState = CAMERASTATE::PLAYER;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_1))
	{
		m_eCamState = CAMERASTATE::CENEMATIC00;
		m_bCine1 = true;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_2))
	{
		m_eCamState = CAMERASTATE::CENEMATICBOSS;
		m_bCine2 = true;
	}
}

void CToolCamScript::SetFreeCamera()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	if (KEY_HOLD(KEY_TYPE::KEY_NUM0))
	{
		fSpeed *= 100.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vFront * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vBack * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	{
		Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vLeft * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	{
		Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vRight * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_NUM1))
	{
		fScale -= m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);
	}



	if (KEY_HOLD(KEY_TYPE::KEY_RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();

		vRot.x -= vDrag.y * DT * 0.5f;
		vRot.y += vDrag.x * DT * 0.5f;

		Transform()->SetLocalRot(vRot);
	}

	Transform()->SetLocalPos(vPos);
}

void CToolCamScript::SetPlayerFixedCamera()
{
	Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();
	CTransform* vPlayerPos = m_pPlayer->Transform();
	
	// 공통
	float fDistance = 800.f; //200.f;

	//vPos = vPlayerPos->GetLocalPos();
	if (CSceneMgr::GetInst()->GetCurScene()->GetType() == SCENE_TYPE::DUNGEON)
	{
		vPos = vPlayerPos->GetLocalPos() - (400 * vPlayerPos->GetWorldDir(DIR_TYPE::UP));
		vPos.y = vPlayerPos->GetLocalPos().y + 100;
	}
	else {
		vPos = vPlayerPos->GetLocalPos() - (fDistance * vPlayerPos->GetWorldDir(DIR_TYPE::UP));
		vPos.y = vPlayerPos->GetLocalPos().y + 200;
	}
	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vec3(XMConvertToRadians(90.f), XM_PI, 0.f));
}

void CToolCamScript::SetCenematic0()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	//집결지 시네
	if (m_bCine1 == true && m_bCheckFin1 == false)
	{
		checktime += DT;

		//cout << checktime << endl;
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);
		vPos += vFront * fSpeed * 2.f * DT;
		vPos += vUp * fSpeed * 2.f * DT;

		if (//집결지끝도착하면 180도회전으로
			checktime >= 3.f)
		{
			m_bCheckFin1 = true;
			checktime = 0.f;

		}


	}
	if (m_bCheckFin1)
	{


		Vec3 vRot = Transform()->GetLocalRot();

		vRot.y += DT * XM_PI * 1 / 5;

		Transform()->SetLocalRot(vRot);

		checktime2 += DT;

		cout << checktime2 << endl;


		////한바뀌돌면 원래 위치로
		if (checktime2 >= 10.f)
		{
			for (auto Boss : CSceneMgr::GetInst()->GetCurScene()->GetLayer(6)->GetParentObj())
			{
				//안불러지는중
				Boss->GetScript<CM_MonsterScript>()->SetCenematicCheck();
					
			}

			checktime2 = 0;
			m_eCamState = CAMERASTATE::PLAYER;

		}

	}

	Transform()->SetLocalPos(vPos);


}

void CToolCamScript::SetCenematicBoss()
{
	Vec3 Temp = { 0.f,200.f,-100.f };
	Vec3 vPos = Temp;
	
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;
	if (!bBosscamCheck)
	{
		vBossCamRot = Transform()->GetLocalRot();
		vBossCamRot.y = 6.f;
		bBosscamCheck = true;
	}
	//보스 시네
	if (m_bCine2 == true && m_bCheckFin2 == false)
	{
		
		checktime2_1 += DT;
		
		
	
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);
		vPos += vFront * fSpeed * 2.f * DT;
		vPos += vUp * fSpeed * 1.2f * DT;

		vBossCamRot.y += DT * XM_PI * 1 / 20;
		Transform()->SetLocalRot(vBossCamRot);

		if (
			checktime2_1 >= 3.f)
		{
			m_bCheckFin2 = true;
			checktime2_1 = 0.f;
			vBossCamRot.y = 9.f;
		}


	}
	if (m_bCheckFin2)
	{
		

		//보스위치로
		vPos = Vec3{ 400.f,200.f,2850.f };

		
		
		cout <<"롯Y:"<< vBossCamRot.y << endl;
		vBossCamRot.y += DT * XM_PI * 1 / 20;

		Transform()->SetLocalRot(vBossCamRot);

		checktime2_2 += DT;

		cout << checktime2 << endl;


		
		if (checktime2_2 >= 7.f)
		{
			checktime2_2 = 0;
			m_eCamState = CAMERASTATE::PLAYER;

		}

	}

	Transform()->SetLocalPos(vPos);
}
