#include "stdafx.h"
#include "SwordScript.h"
#include "MeshRender.h"
#include "Animator3D.h"
#include "Collider2D.h"

#include "PlayerScript.h"

CSwordScript::CSwordScript()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pTargetBone (nullptr)
{
}

CSwordScript::~CSwordScript()
{
}

void CSwordScript::init(PERSON_OBJ_TYPE type, CGameObject* _target, int _boneIdx)
{
	m_eType = type;
	m_pTargetObject = _target;
	m_iTargetBoneIdx = _boneIdx;
}

void CSwordScript::update()
{
	Set_Sword_To_Fbx();
}

void CSwordScript::Set_Sword_To_Fbx()
{
	m_pTargetBone = const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(m_iTargetBoneIdx));

	Vec3 vTrans = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate;
	Vec4 qRot = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].qRot;
	Vec3 vRot;

	switch (m_eType)
	{
	case PERSON_OBJ_TYPE::WARRIOR_PLAYER: {
		vRot = Vec3(0.f, 0.f, XMConvertToRadians(-90.f));
		break;
	}
	case PERSON_OBJ_TYPE::WIZARD_PLAYER:
		break;
	case PERSON_OBJ_TYPE::M_MONSTER:
		vRot = Vec3(0.f, 0.f, XMConvertToRadians(90.f));
		break;
	case PERSON_OBJ_TYPE::FM_MONSTER:
		vRot = Vec3(0.f, 0.f, XMConvertToRadians(45.f));
		break;
	case PERSON_OBJ_TYPE::BOSS:
		vTrans += Vec3(0.f, -10.f, 0.f);
		vRot = Vec3(0.f, XMConvertToRadians(90.f), 0.f);
		break;
	default:
		break;
	}

	Transform()->SetLocalPos(vTrans);
	Transform()->SetQuaternion(qRot);
	Transform()->SetLocalRot(vRot);
}

void CSwordScript::Set_FM_Player()
{
	m_pTargetBone = const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(m_iTargetBoneIdx));
	//tMTBone* m_pTargetBone2= const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(48));

	Vec3 vTrans1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate;
	//Vec3 vTrans2 = m_pTargetBone2->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate;

	//Vec3 vDir = vTrans2 - vTrans1;
	//vDir.Normalize();

	//////Vec3 vTrans2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].vTranslate;
	Vec4 qRot1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].qRot;
	//////Vec4 qRot2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].qRot;

	Vec3 vRot = Vec3(XMConvertToRadians(180.f), XMConvertToRadians(90.f), 0.f);

	Transform()->SetLocalPos(vTrans1 /*+ vDir * Vec3(0.f, 0.f, 100.f)*/);
	Transform()->SetQuaternion(qRot1);
	Transform()->SetLocalRot(vRot);
}

void CSwordScript::OnCollisionEnter(CCollider2D* _pOther)
{

}

void CSwordScript::OnCollision(CCollider2D* _pOther)
{

}

void CSwordScript::OnCollisionExit(CCollider2D* _pOther)
{

}



