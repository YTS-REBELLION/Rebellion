#include "stdafx.h"
#include "MonsterScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include "PlayerScript.h"
#include "SwordScript.h"

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_Is_Move(false)
	,m_Is_Attack(false)
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
	// ===================
	// Sword 파일 로드
	// ===================
	CGameObject* pSwordObject = new CGameObject;

	Ptr<CMeshData>pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Weapon.mdat", L"MeshData\\Monster_FM_Weapon.mdat");

	pSwordObject = pMeshData->Instantiate();
	pSwordObject->SetName(L"FM_Monster_Sword");
	pSwordObject->FrustumCheck(false);
	pSwordObject->Transform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
	pSwordObject->AddComponent(new CCollider2D);

	pSwordObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pSwordObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pSwordObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));

	pSwordObject->AddComponent(new CSwordScript);
	CSwordScript* SwordScript = pSwordObject->GetScript<CSwordScript>();
	pSwordObject->GetScript<CSwordScript>()->init(PERSON_OBJ_TYPE::FM_MONSTER, GetObj(), 18);

	CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", pSwordObject, false);
	GetObj()->AddChild(pSwordObject);
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
	
	//m_fAngle = acosf(Dot(vDirFront, Monster_Dir) / (Length(vDirFront) * Length(Monster_Dir)));
	Vec3 vRot;

	if (m_isTarget) {
		for (auto& client : CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetParentObj())
		{
			if (client->GetScript<CPlayerScript>()->GetID() == m_targetId)
			{
				m_pPlayer = client;
			}
		}

		m_fAngle = atan2(localPos.x - m_pPlayer->Transform()->GetLocalPos().x, localPos.z - m_pPlayer->Transform()->GetLocalPos().z) * (180 / XM_PI) * 0.0174532925f;//acosf(Dot(vDirFront, Monster_Nor));

	}


	vRot = Vec3(localRot.x, m_fAngle, localRot.z);
	
	Monster->Transform()->SetLocalRot(vRot);

	if (m_Is_Move) {
		AnimationPlay(MONSTER_ANI_TYPE::WALK);

	}
	else if (!m_Is_Move && m_Is_Attack) {
		AnimationPlay(MONSTER_ANI_TYPE::ATTACK);
	}
	else
		AnimationPlay(MONSTER_ANI_TYPE::HIT);

	if (m_bHit && m_vecAniClipTime[0] < GetObj()->Animator3D()->GetAnimClip(2).dTimeLength)
	{
		m_vecAniClipTime[0] += (DT * 1.5f);
		AnimationPlay(MONSTER_ANI_TYPE::HIT);
		if (m_vecAniClipTime[0] > GetObj()->Animator3D()->GetAnimClip(2).dTimeLength)
		{
			m_vecAniClipTime[0] = 0.0f;
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
			m_bHit = false;
		}

	}

	//UpdateLerpPos();
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
	if (_pOther->GetObj()->GetName() == L"Player_Sword")
	{
		//cout << "검과 충돌1" << endl;
		m_bHit = true;
		g_net.Send_Player2MonsterCol_Packet(GetID(), GetObj()->GetID(), true);

	}
	if (_pOther->GetObj()->GetName() == L"Player1")
	{
		//cout << "플레이어와 충돌" << endl;
	}
	if (_pOther->GetObj()->GetName() == L"FM_Monster")
	{
		m_colEnter = true;
		//cout << "ENTER" << endl;
		//if (GetObj()->GetScript<CMonsterScript>()->GetID() < _pOther->GetObj()->GetScript<CMonsterScript>()->GetID())
		//g_net.Send_MobToMobCol_Packet(GetObj()->GetScript<CMonsterScript>()->GetID(), _pOther->GetObj()->GetScript<CMonsterScript>()->GetID(), m_colEnter, MONSTER_MOVE::STOP);
		//	cout << "충돌체 1 : " << GetObj()->GetScript<CMonsterScript>()->GetID() << " " << "충돌체 2 :" << _pOther->GetObj()->GetScript<CMonsterScript>()->GetID() << endl;
	}
}

void CMonsterScript::OnCollision(CCollider2D* _pOther)
{
	//m_fHp -= 4.f;
	if (_pOther->GetObj()->GetName() == L"Player1")
	{
		//cout << "플레이어와 충돌" << endl;
	}
	else if (_pOther->GetObj()->GetName() == L"Player_Sword")
	{
		//cout << "검과 충돌2" << endl;
		//m_bHit = true;
		g_net.Send_Player2MonsterCol_Packet(GetID(), GetObj()->GetID(), true);

	}
	//else if (_pOther->GetObj()->GetName() == L"FM_Monster")
	//{
	//	//cout << "몬스터 몬스터 충돌?" << endl;
	//	m_bColCheck = true;
	//	SetColObj(_pOther);
	//}

	if (m_fHp < 0.f)
	{
		//cout << "몬스터 사망" << endl;
		GetObj()->SetDead();
		
	}
}

void CMonsterScript::OnCollisionExit(CCollider2D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player_Sword")
	{
		//cout << "검과 충돌 해제" << endl;
	}
	else if (_pOther->GetObj()->GetName() == L"FM_Monster")
	{
		cout << "EXIT" << endl;
		m_colEnter = false;
		//if (GetObj()->GetScript<CMonsterScript>()->GetID() < _pOther->GetObj()->GetScript<CMonsterScript>()->GetID())
		//g_net.Send_MobToMobCol_Packet(GetObj()->GetScript<CMonsterScript>()->GetID(), _pOther->GetObj()->GetScript<CMonsterScript>()->GetID(), m_colEnter, MONSTER_MOVE::START);
		m_bColCheck = false;
	}
}
