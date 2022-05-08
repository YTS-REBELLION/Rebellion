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
	if (KEY_TAB(KEY_TYPE::KEY_Z))
	{

		if (m_eCamState == CAMERASTATE::FREE)
			m_eCamState = CAMERASTATE::PLAYER;
		else
			m_eCamState = CAMERASTATE::FREE;
	}

	if (m_eCamState == CAMERASTATE::FREE)
		SetFreeCamera();
	else SetPlayerFixedCamera();
}

void CToolCamScript::SetFreeCamera()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
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

	vPos = vPlayerPos->GetLocalPos() - (vPlayerPos->GetWorldDir(DIR_TYPE::UP) * fDistance);
	vPos.y = vPlayerPos->GetLocalPos().y + 200.f;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vec3(XMConvertToRadians(98.5f), XM_PI, 0.f));
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
