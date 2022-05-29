#include "stdafx.h"
#include "MonsterScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include "PlayerScript.h"

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_bAttack(false)
	, m_iCulidx(0)
{
	m_fHp = m_fMaxHp;
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CMonsterScript::update()
{
	Vec3 WorldDir;
	Vec3 localPos = GetObj()->Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	
	CSceneMgr::GetInst()->FindPlayerPos(L"Player");
	float fDistanceP_M = Vec3::Distance(CSceneMgr::GetInst()->m_vSavePos, localPos);
	cout << "거리차이:" << fDistanceP_M << endl;
	if (fDistanceP_M > 200.f && fDistanceP_M <= 505.f )
	{
		m_fSpeed = 200.f;
		//이동
		//플레이어위치로이동
		//localPos.z -=  m_fSpeed/10 * DT;


		if (fDistanceP_M >= 0.f && fDistanceP_M <= 200.f)
		{
			//공격
			m_fSpeed = 0.f;




		}

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

void CMonsterScript::SetPlayerAnimation(const int& i)
{
	//if (m_pAniData.size() == 0)	return;
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
	GetObj()->MeshRender()->SetMesh(m_pAniData[i]);
}
void CMonsterScript::SetPlayerAnimation(int other_id, int i)
{
	//if (m_pAniData.size() == 0)	return;
	GameObject.find(other_id)->second->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GameObject.find(other_id)->second->Animator3D()->SetAnimClip(m_pAniData[i]->GetAnimClip());
	GameObject.find(other_id)->second->MeshRender()->SetMesh(m_pAniData[i]);
}

void CMonsterScript::SetOtherMovePacket(sc_packet_move* p, const float& rtt)
{
	m_movePacketTemp = new sc_packet_move;

	m_movePacketTemp = p;
	//m_fRTT = rtt;
}


void CMonsterScript::OnCollisionEnter(CCollider2D* _pOther)
{
	cout << "?" << endl;
}

void CMonsterScript::OnCollision(CCollider2D* _pOther)
{
	cout << "칼과몬스터충돌" << endl;
	m_fHp -= 4.f;

	if (m_fHp <= 0.f)
	{
		GetObj()->SetDead();

	}


}

void CMonsterScript::OnCollisionExit(CCollider2D* _pOther)
{

	cout << "충돌 해제" << endl;
}
