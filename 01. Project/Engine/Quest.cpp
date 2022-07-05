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

	

	//Vec3 QuestBasePos = m_pQuestBoxBase->Transform()->GetLocalPos();
	//for (int i = 0; i < 2; ++i) {
	//	QuestBoxScale = Vec3(280.f, 60.f, 1.f);
	//	pObject = new CGameObject;
	//	pObject->SetName(L"QuestBox");
	//	pObject->FrustumCheck(false);
	//	pObject->AddComponent(new CTransform);
	//	pObject->AddComponent(new CMeshRender);
	//	pObject->Transform()->SetLocalPos(Vec3(QuestBasePos.x, QuestBasePos.y + 190.f - (QuestBoxScale.y / 2.f) - (i * QuestBoxScale.y), 1.f));
	//	pObject->Transform()->SetLocalScale(QuestBoxScale);
	//	//MeshRender 설정
	//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"QuestBoxMtrl"));
	//	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"QuestBase").GetPointer());
	//	if (GetObj()->GetID() == g_myid)
	//		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
	//	pObject->SetUiRenderCheck(m_bBoxRender);
	//	m_pQuestBox.push_back(pObject);
	//	m_vExistQuestBox.push_back(QUESTBOX_TYPE::EMPTY);
	//}
}


void CQuest::update()
{
	Ptr<CTexture> pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"QuestBase", L"Texture\\Quest\\QuestBase.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"QuestBase", L"Texture\\Quest\\QuestBase.png");

	if (KEY_TAB(KEY_TYPE::KEY_Q)) {
		

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
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"QuestBoxMtrl"));
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pQuestBox.GetPointer());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fUI);

		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
	/*	pObject->SetUiRenderCheck(m_bBoxRender);
		m_pQuestBoxBase = pObject;
		for (int i = 0; i < (UINT)QUEST_TYPE::END; ++i) {
			m_vQuestCheck.push_back(0);
			m_bDoQuest[i] = false;
		}*/
		cout << "퀘스트창:" << endl;
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