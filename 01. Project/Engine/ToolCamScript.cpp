#include "stdafx.h"
#include "ToolCamScript.h"

#include "Camera.h"

#include "RenderMgr.h"
#include "PlayerScript.h"


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


}

void CToolCamScript::SetFreeCamera()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	if (KEY_HOLD(KEY_TYPE::KEY_NUM0))
	{
		fSpeed *= 5.f;
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

	if (KEY_HOLD(KEY_TYPE::KEY_NUM2))
	{
		fScale += m_fScaleSpeed * DT;
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
	float fDistance = 500.f; //200.f;

	vPos = vPlayerPos->GetLocalPos() - (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	vPos.y = vPlayerPos->GetLocalPos().y + 200.f;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vec3(XMConvertToRadians(185.f), XM_PI, 0.f));
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
			checktime2 = 0;
			m_eCamState = CAMERASTATE::PLAYER;

		}

	}

	Transform()->SetLocalPos(vPos);


}

//void CToolCamScript::Fix_Mouse()
//{
//	POINT   ptMouse{ 800 >> 1, 600 >> 1 };
//
//	ClientToScreen(CRenderMgr::GetInst()->GetHwnd(), &ptMouse);
//	SetCursorPos(ptMouse.x, ptMouse.y);
//}
//
//void CToolCamScript::Mouse_Move()
//{
//	// 마우스 좌표	
//
//	m_ptOldMouse = m_ptMouse;
//	GetCursorPos(&m_ptMouse);
//	ScreenToClient(CRenderMgr::GetInst()->GetHwnd(), &m_ptMouse);
//
//	m_vDragDir = Vec2((float)(m_ptMouse.x - m_ptOldMouse.x) * 0.05f, (float)(m_ptOldMouse.y - m_ptMouse.y) * 0.05f);
//
//}
