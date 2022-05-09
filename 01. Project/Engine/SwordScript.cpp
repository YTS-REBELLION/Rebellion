#include "stdafx.h"
#include "SwordScript.h"
#include "MeshRender.h"
#include "Animator3D.h"
#include "Collider2D.h"

CSwordScript::CSwordScript()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
{
}

CSwordScript::~CSwordScript()
{
}

void CSwordScript::update()
{
	m_pTargetBone = const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(m_iTargetBoneIdx));
	tMTBone* m_pTargetBone2= const_cast<tMTBone*>(m_pTargetObject->MeshRender()->GetMesh()->GetBone(48));

	Vec3 vTrans1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate;
	Vec3 vTrans2 = m_pTargetBone2->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].vTranslate;

	Vec3 vDir = vTrans2 - vTrans1;
	vDir.Normalize();

	////Vec3 vTrans2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].vTranslate;
	Vec4 qRot1 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetFrameIdx()].qRot;
	////Vec4 qRot2 = m_pTargetBone->vecKeyFrame[m_pTargetObject->Animator3D()->GetNextFrameIdx()].qRot;

	//Vec3 vRot = Vec3(0.f, XMConvertToRadians(90.f), 0.f);

	Transform()->SetLocalPos(vTrans1 + vDir * Vec3(0.f, 0.f, 100.f));
	Transform()->SetQuaternion(qRot1);
	//Transform()->SetLocalRot(vRot);
}

