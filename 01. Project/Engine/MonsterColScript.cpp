#include "stdafx.h"
#include "MonsterColScript.h"
#include "MonsterScript.h"

CMonsterColScript::CMonsterColScript() :CScript((UINT)SCRIPT_TYPE::OBJCOLSCRIPT) {


}
CMonsterColScript::~CMonsterColScript() {

}
void CMonsterColScript::OnCollisionEnter(CCollider2D* _pOther)
{
	m_pMonster->GetScript<CMonsterScript>()->OnCollisionEnter(_pOther);
}
void CMonsterColScript::OnCollision(CCollider2D* _pOther)
{
	m_pMonster->GetScript<CMonsterScript>()->OnCollision(_pOther);
}
void CMonsterColScript::OnCollisionExit(CCollider2D* _pOther)
{
	m_pMonster->GetScript<CMonsterScript>()->OnCollisionExit(_pOther);
}

void CMonsterColScript::update() {

	Vec3 vRot = Transform()->GetLocalRot();
	vRot = m_vCalRot;
	Transform()->SetLocalPos(m_pMonster->Transform()->GetLocalPos());
	Transform()->SetLocalRot(vRot);

	m_vCalRot = Vec3();
}