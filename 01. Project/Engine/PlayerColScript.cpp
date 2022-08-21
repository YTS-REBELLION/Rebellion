#include "stdafx.h"
#include "PlayerColScript.h"
#include "PlayerScript.h"

CPlayerColScript::CPlayerColScript() :CScript((UINT)SCRIPT_TYPE::OBJCOLSCRIPT) {


}
CPlayerColScript::~CPlayerColScript() {

}
void CPlayerColScript::OnCollisionEnter(CCollider2D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollisionEnter(_pOther);
}
void CPlayerColScript::OnCollision(CCollider2D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollision(_pOther);
}
void CPlayerColScript::OnCollisionExit(CCollider2D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollisionExit(_pOther);
}

void CPlayerColScript::update() {

	Vec3 vRot = Transform()->GetLocalRot();
	vRot = m_vCalRot;
	Transform()->SetLocalPos(m_pPlayer->Transform()->GetLocalPos());
	Transform()->SetLocalRot(vRot);

	m_vCalRot = Vec3();
}