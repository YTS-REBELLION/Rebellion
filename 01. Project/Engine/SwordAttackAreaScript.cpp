#include "stdafx.h"
#include "SwordAttackAreaScript.h"
#include "PlayerScript.h"

CSwordAttackAreaScript::CSwordAttackAreaScript() :CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT) {


}
CSwordAttackAreaScript::~CSwordAttackAreaScript() {

}
void CSwordAttackAreaScript::OnCollisionEnter(CCollider2D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollisionEnter(_pOther);
}
void CSwordAttackAreaScript::OnCollision(CCollider2D* _pOther)
{
	//m_pPlayer->GetScript<CPlayerScript>()->GetColSSA()->SetActive(false);
	m_pPlayer->GetScript<CPlayerScript>()->OnCollision(_pOther);
}
void CSwordAttackAreaScript::OnCollisionExit(CCollider2D* _pOther)
{
	m_pPlayer->GetScript<CPlayerScript>()->OnCollisionExit(_pOther);
}

void CSwordAttackAreaScript::update() {

	Vec3 vRot = Transform()->GetLocalRot();
	vRot = m_vCalRot;
	Transform()->SetLocalPos(m_pPlayer->Transform()->GetLocalPos() + m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::UP) * 100);
	Transform()->SetLocalRot(vRot);

	m_vCalRot = Vec3();
}