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

	CMonsterScript* Monster = GetObj()->GetScript<CMonsterScript>();
	const vector<CGameObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetObjects();
	
	
	if (vecObject.size() > 0)
	{
		m_pPlayer = vecObject[0]->GetScript<CPlayerScript>();

		if (m_movePacketTemp != nullptr && m_movePacketTemp->isMoving) {
			AnimationPlay(MONSTER_ANI_TYPE::WALK);
		}
		else if (m_bHit && m_vecAniClipTime[0] < GetObj()->Animator3D()->GetAnimClip(2).dTimeLength)
		{
			m_vecAniClipTime[0] += DT;
			AnimationPlay(MONSTER_ANI_TYPE::HIT);

			if (m_vecAniClipTime[0] > GetObj()->Animator3D()->GetAnimClip(2).dTimeLength)
			{
				m_vecAniClipTime[0] = 0.0f;
				m_bHit = false;
			}
		}
		else if (m_attackPacket != nullptr && m_attackPacket->isAttack) {
			AnimationPlay(MONSTER_ANI_TYPE::ATTACK);
		}
		else
			AnimationPlay(MONSTER_ANI_TYPE::IDLE);			
	}

	Transform()->SetLocalPos(localPos);
}

void CMonsterScript::SetMonsterAnimationData(Ptr<CMesh> AniDate, const int& i, const UINT& _StartFrame, const UINT& _EndFrame)
{
	m_pAniData.push_back(AniDate);

	tMTAnimClip* tNewAnimClip = new tMTAnimClip;
	tNewAnimClip->iStartFrame = _StartFrame;
	tNewAnimClip->iEndFrame = _EndFrame;
	tNewAnimClip->iFrameLength = _EndFrame - _StartFrame;
	tNewAnimClip->dStartTime = (double)_StartFrame / (double)30;
	tNewAnimClip->dEndTime = (double)_EndFrame / (double)30;
	tNewAnimClip->dTimeLength = tNewAnimClip->dEndTime - tNewAnimClip->dStartTime;

	m_pVecAnimClip.push_back(*tNewAnimClip);

	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);
}

void CMonsterScript::SetMonsterAnimation(const int& i)
{
	GetObj()->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(&m_pVecAnimClip);
	GetObj()->MeshRender()->SetMesh(m_pAniData[i]);
}

void CMonsterScript::SetMonsterAnimation(int other_id, const int& i)
{
	GameObject.find(other_id)->second->Animator3D()->SetBones(m_pAniData[i]->GetBones());
	GameObject.find(other_id)->second->Animator3D()->SetAnimClip(&m_pVecAnimClip);
	GameObject.find(other_id)->second->MeshRender()->SetMesh(m_pAniData[i]);
}

void CMonsterScript::AnimationPlay(const MONSTER_ANI_TYPE& type)
{
	if (type == MONSTER_ANI_TYPE::IDLE)
	{
		GetObj()->Animator3D()->SetCurClip(0);
		SetMonsterAnimation(0);
	}
	if (type == MONSTER_ANI_TYPE::WALK)
	{
		GetObj()->Animator3D()->SetCurClip(1);
		SetMonsterAnimation(1);
	}
	if (type == MONSTER_ANI_TYPE::HIT)
	{
		GetObj()->Animator3D()->SetCurClip(2);
		SetMonsterAnimation(2);
	}
	if (type == MONSTER_ANI_TYPE::ATTACK)
	{
		GetObj()->Animator3D()->SetCurClip(3);
		SetMonsterAnimation(3);
	}
}

void CMonsterScript::SetOtherMovePacket(sc_packet_move* p, const float& rtt)
{
	m_movePacketTemp = new sc_packet_move;
	m_movePacketTemp = p;

}

void CMonsterScript::SetMonsterPacket(sc_packet_npc_attack* p)
{
	m_attackPacket = new sc_packet_npc_attack;

	m_attackPacket = p;
}


void CMonsterScript::OnCollisionEnter(CCollider2D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player_Sword")
	{
	cout << "검과 충돌" << endl;
	m_bHit = true;
	//g_net.Send_Player2MonsterCol_Packet(GetID(), GetObj()->GetID(), true);

	}
}

void CMonsterScript::OnCollision(CCollider2D* _pOther)
{
	//m_fHp -= 4.f;
	if (_pOther->GetObj()->GetName() == L"Player1")
	{
		cout << "플레이어와 충돌" << endl;
	}
	else if (_pOther->GetObj()->GetName() == L"Player_Sword")
	{
		cout << "검과 충돌" << endl;
		m_bHit = true;
		g_net.Send_Player2MonsterCol_Packet(GetID(), GetObj()->GetID(), true);

	}

	if (m_fHp < 0.f)
	{
		cout << "몬스터 사망" << endl;
		GetObj()->SetDead();
		
	}
}

void CMonsterScript::OnCollisionExit(CCollider2D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player_Sword")
	{
		cout << "검과 충돌 해제" << endl;
	}
}
