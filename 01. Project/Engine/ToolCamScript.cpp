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
	CTransform* vPlayerPos = m_pPlayer->Transform();

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
//	// ¸¶¿ì½º ÁÂÇ¥	
//
//	m_ptOldMouse = m_ptMouse;
//	GetCursorPos(&m_ptMouse);
//	ScreenToClient(CRenderMgr::GetInst()->GetHwnd(), &m_ptMouse);
//
//	m_vDragDir = Vec2((float)(m_ptMouse.x - m_ptOldMouse.x) * 0.05f, (float)(m_ptOldMouse.y - m_ptMouse.y) * 0.05f);
//
//}
