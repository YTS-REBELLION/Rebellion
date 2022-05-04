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
		vPos += vFront * fSpeed * DT;
		vPos += vUp * fSpeed * 2.f * DT;

		if (//집결지끝도착하면 180도회전으로
			checktime >= 1.f)
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


		cout << "RotY:" << vRot.y << endl;


		////한바뀌돌면 원래 위치로
		if (vRot.y >= 6.1f)
		{
			vRot.y = 0;
			m_CameraMode = PLAYER_IDLE;

	float fDistance = 500.f; //200.f;

	vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	vPos.y = vPlayerPos->GetLocalPos().y + 200.f;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vec3(XM_PI / 11, XM_PI, 0.f));
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
