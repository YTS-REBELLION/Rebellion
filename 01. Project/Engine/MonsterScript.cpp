#include "stdafx.h"
#include "MonsterScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include "PlayerScript.h"
#include "SwordScript.h"
#include "SwordAttackAreaScript.h"
#include"Sting.h"

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_Is_Move(false)
	, m_Is_Attack(false)
	, m_bHit(false)
	, m_iCulidx(0)
	, m_bAniOk(false)
	, m_fAngle(0.0f)
	//,m_pPlayer(nullptr)
{
	m_fHp = m_fMaxHp;
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::init()
{
	CGameObject* pSwordCol = new CGameObject;
	pSwordCol->SetName(L"MonsterSwordCol");
	pSwordCol->AddComponent(new CCollider2D);
	pSwordCol->AddComponent(new CTransform);
	pSwordCol->AddComponent(new CMeshRender);
	pSwordCol->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
	pSwordCol->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pSwordCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pSwordCol->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pSwordCol->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	pSwordCol->Collider2D()->SetOffsetScale(Vec3(40.f, 40.f, 40.f));
	pSwordCol->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pSwordCol->AddComponent(new CSwordAttackAreaScript);
	pSwordCol->GetScript<CSwordAttackAreaScript>()->Set_Object(GetObj());
	pSwordCol->SetActive(false);
	GetObj()->GetScript<CMonsterScript>()->SetColSSA(pSwordCol);

	CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"MonsterSword", pSwordCol, false);
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
	Vec3 vDirFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vDirUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vDirRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

	Transform()->SetWorldDir(DIR_TYPE::FRONT, vDirUp);
	Transform()->SetLocalDir(DIR_TYPE::FRONT, vDirUp);

	Transform()->SetWorldDir(DIR_TYPE::UP, -vDirFront);
	Transform()->SetLocalDir(DIR_TYPE::UP, -vDirFront);

	Vec3 WorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = GetObj()->Transform()->GetLocalPos();
	Vec3 localRot = GetObj()->Transform()->GetLocalRot();
	CTransform* playerTrans = Transform();
	Vec3 WorldPos = GetObj()->Transform()->GetWorldPos();
	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	CSceneMgr::GetInst()->FindPlayerPos(L"Player");
	float fDistanceP_M = Vec3::Distance(CSceneMgr::GetInst()->m_vSavePos, localPos);
	CMonsterScript* Monster = GetObj()->GetScript<CMonsterScript>();
	const vector<CGameObject*>& vecObject = CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetObjects();

	Vec3 vRot;

	if (m_isTarget) {
		for (auto& client : CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetParentObj())
		{
			if (client->GetScript<CPlayerScript>()->GetID() == m_targetId)
			{
				m_pPlayer = client;
			}
		}
		m_fAngle = atan2(localPos.x - m_pPlayer->Transform()->GetLocalPos().x, localPos.z - m_pPlayer->Transform()->GetLocalPos().z) * (180 / XM_PI) * 0.0174532925f;
	}


	vRot = Vec3(localRot.x, m_fAngle, localRot.z);

	Monster->Transform()->SetLocalRot(vRot);

	if (m_Is_Move) {
		AnimationPlay(MONSTER_ANI_TYPE::WALK);

	}
	else if (!m_Is_Move && m_Is_Attack) {
		if (GetObj()->Animator3D()->GetCliTime(3) > 0.75f)
		{
			pSwordColObject->SetActive(true);
		}
		else if (GetObj()->Animator3D()->GetCliTime(3) == 0.f)
			pSwordColObject->SetActive(false);
	}
	else if (GetHit())
	{
		if (GetObj()->Animator3D()->GetCliTime(2) < GetObj()->Animator3D()->GetAnimClip(2).dTimeLength)
		{
			AnimationPlay(MONSTER_ANI_TYPE::HIT);
		}
	}
	else
		AnimationPlay(MONSTER_ANI_TYPE::IDLE);

	UpdateLerpPos();
}
void CMonsterScript::UpdateLerpPos()
{
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Pos = Vec3::Lerp(Pos, LerpPos, DT);

	GetObj()->Transform()->SetLocalPos(Pos);
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

void CMonsterScript::AnimationPlay(int other_id, const MONSTER_ANI_TYPE& type)
{
	if (type == MONSTER_ANI_TYPE::IDLE)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(0);
		SetMonsterAnimation(other_id, 0);
	}
	if (type == MONSTER_ANI_TYPE::WALK)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(1);
		SetMonsterAnimation(other_id, 1);
	}
	if (type == MONSTER_ANI_TYPE::HIT)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(2);
		SetMonsterAnimation(other_id, 2);
	}
	if (type == MONSTER_ANI_TYPE::ATTACK)
	{
		GameObject.find(other_id)->second->Animator3D()->SetCurClip(3);
		SetMonsterAnimation(other_id, 3);
	}
}

void CMonsterScript::SetOtherMovePacket(sc_packet_move* p)
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
	if (_pOther->GetObj()->GetName() == L"PlayerSwordCol")
	{
		SetHit();
		if (GetHit()) {
			g_net.Send_Player2MonsterCol_Packet(GetObj()->GetID(), GetID(), true, 0);
		}
	}
}

void CMonsterScript::OnCollision(CCollider2D* _pOther)
{
}

void CMonsterScript::OnCollisionExit(CCollider2D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"PlayerSwordCol") SetHit();
}
