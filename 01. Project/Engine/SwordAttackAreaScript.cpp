#include "stdafx.h"
#include "SwordAttackAreaScript.h"
#include "PlayerScript.h"
#include "MonsterScript.h"

CSwordAttackAreaScript::CSwordAttackAreaScript() :CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT) {


}
CSwordAttackAreaScript::~CSwordAttackAreaScript() {

}
void CSwordAttackAreaScript::OnCollisionEnter(CCollider2D* _pOther)
{
}
void CSwordAttackAreaScript::OnCollision(CCollider2D* _pOther)
{
}
void CSwordAttackAreaScript::OnCollisionExit(CCollider2D* _pOther)
{
}

void CSwordAttackAreaScript::update() {
	
	Vec3 vRot = Transform()->GetLocalRot();
	vRot = m_vCalRot;

	if (m_pObject->GetName() == L"FM_Player")
		Transform()->SetLocalPos(m_pObject->Transform()->GetLocalPos() + m_pObject->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100);
	else if (m_pObject->GetName() == L"FM_Monster")
		Transform()->SetLocalPos(m_pObject->Transform()->GetLocalPos() - m_pObject->Transform()->GetWorldDir(DIR_TYPE::UP) * 100);
	
	Transform()->SetLocalRot(vRot);

	m_vCalRot = Vec3();
}