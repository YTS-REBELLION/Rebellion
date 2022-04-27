#include "stdafx.h"
#include "ToolCamScript.h"

#include "Camera.h"

CToolCamScript::CToolCamScript()
	: CScript(0)
	, m_fSpeed(200.f)
	, m_fScaleSpeed(1.f)
{
}

CToolCamScript::~CToolCamScript()
{
}

void CToolCamScript::update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	CTransform* vPlayerPos = m_pPlayer->Transform();//g_Object.find(g_myid)->second->Transform();

	float fDistance = 500.f; //200.f;

	vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	vPos.y = vPlayerPos->GetLocalPos().y + 200.f;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vec3(XM_PI / 11, XM_PI, 0.f));
}
