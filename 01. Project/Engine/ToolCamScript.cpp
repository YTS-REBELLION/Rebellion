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

void CToolCamScript::update()
{
	CameraMode();
	

	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	if (KEY_AWAY(KEY_TYPE::KEY_1))
	{
		//집결지앞의 충돌체와 충돌시 시네마틱으로
		m_CameraMode = CENEMATIC_0;
		m_bCine1 = true;

	}
	if (KEY_HOLD(KEY_TYPE::KEY_2))
	{
		fScale += m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);
	}

	
		
		
	
	
}


void CToolCamScript::CameraMode()
{
	switch (m_CameraMode)
	{
	case PLAYER_IDLE:
		Player_Mode();
		break;
	case CENEMATIC_0:
		Cenematic0_Mode();
		break;

	default:
		break;
	}
}

void CToolCamScript::Player_Mode()
{
	Vec3 vPos = Transform()->GetLocalPos();
	CTransform* vPlayerPos = m_pPlayer->Transform();//g_Object.find(g_myid)->second->Transform();

	float fDistance = 300.f; //200.f;

	vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	vPos.y = vPlayerPos->GetLocalPos().y + 200.f;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vec3(XM_PI / 8.5, XM_PI, 0.f));


	

}

void CToolCamScript::Cenematic0_Mode()
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
		if (checktime2 >=10.f)
		{
			checktime2 = 0;
			m_CameraMode = PLAYER_IDLE;

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
