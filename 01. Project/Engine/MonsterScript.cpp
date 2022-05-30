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
	, m_bHit(false)
	, m_iCulidx(0)
	, m_bAniOk(false)
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
	//cout << "거리차이:" << fDistanceP_M << endl;
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
	
	//if (m_bHit)
	//{
	//	//cout << "공격하는 도중에 맞았다." << endl;
	//	SetPlayerAnimation(1, 0, 40);
	//	cout << GetObj()->Animator3D()->GetFrameIdx() << endl;
	//	if (GetObj()->Animator3D()->GetCurTime() > 2.6f && !GetHit())
	//	{
	//		SetHit();
	//		cout << "애니메이션 끝" << endl;
	//	}
	//}
	//else {
	//}

	SetPlayerAnimation(0, 0, 76);
	Transform()->SetLocalPos(localPos);
}

void CMonsterScript::SetPlayerAnimation(const int& i, const UINT& _StartFrame, const UINT& _EndFrame)
{
	//if (m_pAniData.size() == 0)	return;
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());

	tMTAnimClip* tNewAnimClip = new tMTAnimClip;
	tNewAnimClip->iStartFrame = _StartFrame;
	tNewAnimClip->iEndFrame = _EndFrame;
	tNewAnimClip->iFrameLength = _EndFrame - _StartFrame;
	tNewAnimClip->dStartTime = (double)_StartFrame / (double)30;
	tNewAnimClip->dEndTime = (double)_EndFrame / (double)30;
	tNewAnimClip->dTimeLength = tNewAnimClip->dEndTime - tNewAnimClip->dStartTime;

	m_pVecAnimClip.push_back(*tNewAnimClip);

	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);

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
}

void CMonsterScript::OnCollision(CCollider2D* _pOther)
{
	m_fHp -= 4.f;
	m_bHit = true;

	if (m_fHp <= 0.f)
	{
		GetObj()->SetDead();
	}
}

void CMonsterScript::OnCollisionExit(CCollider2D* _pOther)
{
	m_bHit = false;
}
