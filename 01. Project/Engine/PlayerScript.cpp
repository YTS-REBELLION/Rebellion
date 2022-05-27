#include "stdafx.h"
#include "PlayerScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_bAttack(false)
	, m_iCulidx(0)
{	
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CPlayerScript::update()
{
	// Z-up To Y-up
	Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Transform()->SetWorldDir(DIR_TYPE::UP, vDirFront);
	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);

	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();

	//Vec3 vPos = Transform()->GetLocalPos();
	//Vec3 vRot = Transform()->GetLocalRot();
	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_A))
	{	
		WorldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		WorldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += WorldDir * m_fSpeed * DT;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			localPos += WorldDir * m_fSpeed * DT;
			SetPlayerAnimation(2);
		}
		else SetPlayerAnimation(1);
	}
	else
	{
		SetPlayerAnimation(0);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		vRot.y += vDrag.x * DT * 0.5f;
		Transform()->SetLocalRot(vRot);
	}

	if (KEY_TAB(KEY_TYPE::KEY_SPACE))
	{
		GetObj()->Animator3D()->SetClipTime(0, 0.f);
		SetAttack();
	}
	if (GetAttack() && m_vecAniClipTime[0] < Animator3D()->GetAnimClip(0).dTimeLength) {
		m_vecAniClipTime[0] += DT;

		cout << m_vecAniClipTime[0] << endl;
		GetObj()->Collider2D()->SetOffsetPos(Vec3(0.f, 20.f, 70.f));
		GetObj()->Collider2D()->SetOffsetScale(Vec3(800.f, 1150.f, 1700.f));


		SetPlayerAnimation(3);

		if (m_vecAniClipTime[0] > Animator3D()->GetAnimClip(0).dTimeLength)
		{
			m_vecAniClipTime[0] = 0.f;
			GetObj()->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
			GetObj()->Collider2D()->SetOffsetScale(Vec3(800.f, 850.f, 1700.f));

			SetAttack();
		}
	}

	if (KEY_HOLD(KEY_TYPE::KEY_ENTER))
	{
		localPos.y = 0.f;
	}
	Transform()->SetLocalPos(localPos);


	//Transform()->SetLocalRot(vRot);
}

void CPlayerScript::SetPlayerAnimation(const int i)
{
	if (m_pAniData.size() == 0)	return;
	Animator3D()->SetBones(m_pAniData[i]->GetBones());
	Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
	MeshRender()->SetMesh(m_pAniData[i]);
}

void CPlayerScript::OnCollisionEnter(CCollider2D* _pOther)
{
	cout << "?" << endl;
}

void CPlayerScript::OnCollision(CCollider2D* _pOther)
{
	//BoundingSphere myBS = Collider2D()->GetBS();
	//BoundingSphere otherBS = _pOther->Collider2D()->GetBS();

	cout << "충돌" << endl;

	//Vec3 WorldDir;
	//Vec3 localPos = Transform()->GetLocalPos();
	//CTransform* playerTrans = Transform();

	//WorldDir = playerTrans->GetWorldDir(DIR_TYPE::UP);
	//localPos -= WorldDir * m_fSpeed * DT * 2.0f;

	//if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
	//{
	//	localPos -= WorldDir * m_fSpeed * DT;
	//}

	////if (L"MIRO" == _pOther->GetObj()->GetName())
	////{
	////	//localPos.y += 1000.f;
	////}

	//Transform()->SetLocalPos(localPos);

}

void CPlayerScript::OnCollisionExit(CCollider2D* _pOther)
{

	cout << "충돌 해제" << endl;
}