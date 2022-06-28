#include "stdafx.h"
#include "Quest.h"
#include "Transform.h"
#include "MeshRender.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "RenderMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"

CQuest::CQuest()
	:CComponent(COMPONENT_TYPE::QUEST)
{

}

CQuest::~CQuest()
{

}

void CQuest::Init()
{
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	Vec3	QuestBoxScale = Vec3(300.f, 400.f, 1.f);
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"QuestBoxBase");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3((res.fWidth / 2.f) - (QuestBoxScale.x / 2.f), (res.fHeight / 2.f) - (QuestBoxScale.y / 2.f) - 30.f, 1.f));
	pObject->Transform()->SetLocalScale(QuestBoxScale);
	//MeshRender 설정
	float fUI = 1.0f;
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UITexMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"QuestBase").GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fUI);
	if (GetObj()->GetID() == g_myid)
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
	pObject->SetUiRenderCheck(m_bBoxRender);
	m_pQuestBoxBase = pObject;
	for (int i = 0; i < (UINT)QUEST_TYPE::END; ++i) {
		m_vQuestCheck.push_back(0);
		m_bDoQuest[i] = false;
	}

	Vec3 QuestBasePos = m_pQuestBoxBase->Transform()->GetLocalPos();
	for (int i = 0; i < 2; ++i) {
		QuestBoxScale = Vec3(280.f, 60.f, 1.f);
		pObject = new CGameObject;
		pObject->SetName(L"QuestBox");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->Transform()->SetLocalPos(Vec3(QuestBasePos.x, QuestBasePos.y + 190.f - (QuestBoxScale.y / 2.f) - (i * QuestBoxScale.y), 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxScale);
		//MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"QuestBoxMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"QuestBase_2").GetPointer());
		if (GetObj()->GetID() == g_myid)
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		pObject->SetUiRenderCheck(m_bBoxRender);
		m_pQuestBox.push_back(pObject);
		m_vExistQuestBox.push_back(QUESTBOX_TYPE::EMPTY);
	}
}


void CQuest::update()
{
	//npc만들어서 추가해야함


	//// KIIL_MONSTER
	//if (m_vQuestCheck[(UINT)QUEST_TYPE::KILL_MONSTER] >= 10) {
	//	for (int i = 0; i < 2; ++i) {
	//		if (m_vExistQuestBox[i] == QUESTBOX_TYPE::NPC_1)
	//			m_pQuestBox[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"MonsterKill_Complete").GetPointer());
	//	}
	//}

	//// Get item
	//if (m_vQuestCheck[(UINT)QUEST_TYPE::GET_ITEM] >= 3) {
	//	for (int i = 0; i < 2; ++i) {
	//		if (m_vExistQuestBox[i] == QUESTBOX_TYPE::NPC_1)
	//			m_pQuestBox[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"GetItem_Complete").GetPointer());
	//	}
	//}
	//// Buy Potion
	//if (m_vQuestCheck[(UINT)QUEST_TYPE::BUY_POTION] >= 3) {
	//	for (int i = 0; i < 2; ++i) {
	//		if (m_vExistQuestBox[i] == QUESTBOX_TYPE::NPC_2)
	//			m_pQuestBox[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BuyPotion_Complete").GetPointer());
	//	}
	//}
	//// Buy Weapone
	//if (m_vQuestCheck[(UINT)QUEST_TYPE::BUY_WEAPON] >= 1) {
	//	for (int i = 0; i < 2; ++i) {
	//		if (m_vExistQuestBox[i] == QUESTBOX_TYPE::NPC_2)
	//			m_pQuestBox[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BuyWeapone_Complete").GetPointer());
	//	}
	//}

	if (KEY_TAB(KEY_TYPE::KEY_Q)) {
		m_bBoxRender = !m_bBoxRender;
		m_pQuestBoxBase->SetUiRenderCheck(m_bBoxRender);
		for (int i = 0; i < m_pQuestBox.size(); ++i)
			m_pQuestBox[i]->SetUiRenderCheck(m_bBoxRender);
	}
}

void CQuest::lateupdate()
{
}

void CQuest::finalupdate()
{
}

void CQuest::SaveToScene(FILE* _pFile)
{
}

void CQuest::LoadFromScene(FILE* _pFile)
{
}