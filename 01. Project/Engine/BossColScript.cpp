#include "stdafx.h"
#include "BossColScript.h"
#include "M_MonsterScript.h"

CBossColScript::CBossColScript() :CScript((UINT)SCRIPT_TYPE::OBJCOLSCRIPT) {


}
CBossColScript::~CBossColScript() {

}
void CBossColScript::OnCollisionEnter(CCollider2D* _pOther)
{
	m_pBoss->GetScript<CM_MonsterScript>()->OnCollisionEnter(_pOther);
}
void CBossColScript::OnCollision(CCollider2D* _pOther)
{
	m_pBoss->GetScript<CM_MonsterScript>()->OnCollision(_pOther);
}
void CBossColScript::OnCollisionExit(CCollider2D* _pOther)
{
	m_pBoss->GetScript<CM_MonsterScript>()->OnCollisionExit(_pOther);
}

void CBossColScript::update() {

	Vec3 vRot = Transform()->GetLocalRot();
	vRot = m_vCalRot;
	Transform()->SetLocalPos(m_pBoss->Transform()->GetLocalPos());
	Transform()->SetLocalRot(vRot);

	m_vCalRot = Vec3();
}