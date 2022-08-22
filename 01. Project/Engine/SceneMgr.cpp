#include "stdafx.h"


#include "SoundMgr.h"
#include "Sound.h"
#include "SceneMgr.h"

#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

#include "ResMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "Animator3D.h"
#include "Collider2D.h"
#include "Light2D.h"
#include "Light3D.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "Device.h"
#include "Core.h"

#include "PlayerScript.h"
#include "MonsterScript.h"
#include "M_MonsterScript.h"
#include "SwordScript.h"
#include "ToolCamScript.h"
#include "GridScript.h"
#include "Sword.h"

#include"GuardHouse.h"
#include"GateHouse.h"
#include"Tree.h"
#include"Somethings.h"
#include"Portal.h"
#include"Miro.h"
#include"Castle.h"
#include"NpcMon.h"
#include"Fire.h"
#include"ParticleSystem.h"
#include "meshdata.h"

#include "stdafx.h"
#include "MonsterScript.h"
#include "TestScript.h"
#include "RenderMgr.h"
#include "Animator3D.h"
#include "PlayerScript.h"

#include "LoginScene.h"
#include "AssemblyAreaScene.h"
#include "BossMap.h"
#include "Dungeon.h"

unordered_map<Sound_Type, CSound*> g_SoundList;


CScene* CSceneMgr::GetCurScene()
{
	return m_pCurScene;

}

void CSceneMgr::ChangeScene(CScene * _pNextScene)
{
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = _pNextScene;
}

void CSceneMgr::ChangeScene(SCENE_TYPE _Type)
{
	if (m_pCurScene == m_arrScene[(UINT)_Type])
		return;

	SAFE_DELETE(m_pCurScene);

	switch (_Type)
	{
	case SCENE_TYPE::LOGIN:
		m_pCurScene = new CAssemblyAreaScene;
		break;
	case SCENE_TYPE::ASSEMBLY:
		m_pCurScene = new CAssemblyAreaScene;
		break;
	case SCENE_TYPE::DUNGEON:
		m_bSound02 = true;
		m_pCurScene = new CDungeonScene;
		break;
	case SCENE_TYPE::BOSSMAP:
		m_bSound03 = true;
		m_pCurScene = new CBossMap;
		break;
	case SCENE_TYPE::END:
		assert(false);
		break;
	default:
		break;
	}
	m_pCurScene->SetType(_Type);
	m_pCurScene->init();

	
	m_pCurScene->awake();
	m_pCurScene->start();
}

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)	
{
	testvec.empty();
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::LoadSound()
{
	for (int i = 0; i < 4; i++)
	{
		CSound* temp = new CSound;
		wstring strFullPath = CPathMgr::GetResPath();
		switch ((Sound_Type)i)
		{
		case Sound_Type::BGM:
			strFullPath += L"Sound\\vol3_04_Chang Chun.mp3";
			break;
		case Sound_Type::BGM2:
			strFullPath += L"Sound\\vol3_03_Frost Jail Plateau.mp3";
			break;
		case Sound_Type::HIT:
			strFullPath += L"Sound\\sword.wav";
			break;
		case Sound_Type::BGM3:
			strFullPath += L"Sound\\vol3_03_Frost Jail Plateau.mp3";
			break;
			
		
		default:
			break;
		}
		temp->Load(strFullPath, (Sound_Type)i);
		g_SoundList.emplace((Sound_Type)i, temp);
	}
}

void CSceneMgr::CreateTargetUI()
{
	Vec3 vScale(150.f, 150.f, 1.f);

	Ptr<CTexture> arrTex[5] = { CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseLightTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularLightTargetTex") };

	for (UINT i = 0; i < 5; ++i)
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();

		pObject->Transform()->SetLocalPos(Vec3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x)
										, (res.fHeight / 2.f) - (vScale.y / 2.f)
										, 1.f));

		pObject->Transform()->SetLocalScale(vScale);
		
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, arrTex[i].GetPointer());
	
		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
	}	
}

void CSceneMgr::init()
{
	LoadSound();
	//m_arrScene[(UINT)SCENE_TYPE::LOGIN] = new CLogin;
	m_arrScene[(UINT)SCENE_TYPE::ASSEMBLY] = new CAssemblyAreaScene;
	//m_arrScene[(UINT)SCENE_TYPE::DUNGEON] = new CDungeonScene;

	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::ASSEMBLY];
	m_pCurScene->init();

	m_pCurScene->awake();
	m_pCurScene->start();
}

void CSceneMgr::update()
{		
	m_pCurScene->update();
	if (m_bSound01)
	{
		g_SoundList.find(Sound_Type::BGM)->second->Play(0);
	}
	if (m_bSound01&& m_bSound02)
	{
		g_SoundList.find(Sound_Type::BGM)->second->Stop();
		g_SoundList.find(Sound_Type::BGM2)->second->Play(0);
	}
	if (m_bSound02 && m_bSound03)
	{
		g_SoundList.find(Sound_Type::BGM2)->second->Stop();
		g_SoundList.find(Sound_Type::BGM3)->second->Play(0);
	}
	m_pCurScene->lateupdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->finalupdate();

	// 충돌 처리
	CCollisionMgr::GetInst()->update();

	if (KEY_TAB(KEY_TYPE::KEY_NUM7)) {
		if (m_pCurScene->GetType() != SCENE_TYPE::ASSEMBLY) {
		}

	}
	if (KEY_TAB(KEY_TYPE::KEY_9)) {
		if (m_pCurScene->GetType() != SCENE_TYPE::DUNGEON) {
			g_net.Send_Dungeon_Packet(true);
		}

	}
	if (KEY_TAB(KEY_TYPE::KEY_0)) {
		if (m_pCurScene->GetType() != SCENE_TYPE::BOSSMAP) {
			cout << "보스맵" << endl;
			g_net.Send_bossmap_Packet(true);
		}
	}
}

void CSceneMgr::update_tool()
{
	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->finalupdate();

}


void CSceneMgr::FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (_strTag == vecObject[j]->GetName())
			{
				_vecFindObj.push_back(vecObject[j]);
			}
		}
	}	
}

void CSceneMgr::FindPlayerPos(const wstring& _strTag)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (_strTag == vecObject[j]->GetName())
			{
				m_vSavePos = { vecObject[j]->Transform()->GetLocalPos().x ,vecObject[j]->Transform()->GetLocalPos().y
					,vecObject[j]->Transform()->GetLocalPos().z };
				m_bfindcheck = true;

				m_vSaveRot=vecObject[j]->Transform()->GetLocalRot();

			}
		}
	}
}

bool Compare(CGameObject* _pLeft, CGameObject* _pRight)
{
	return (_pLeft->Transform()->GetWorldPos().z < _pRight->Transform()->GetWorldPos().z);
}

//void CSceneMgr::FindGameObjectByPoint(POINT _point, vector<CGameObject*>& _vecFindObj, CCamera* _pToolCam)
//{
//	CCamera* pCam = _pToolCam;
//	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
//	{
//		pCam = CRenderMgr::GetInst()->GetCamera(0);
//	}
//
//	tResolution tRes = CRenderMgr::GetInst()->GetResolution();
//	Vec3 vPickPos = Vec3((float)_point.x - (tRes.fWidth / 2.f), (tRes.fHeight / 2.f) - (float)_point.y, 0.f);
//	vPickPos *= pCam->GetScale(); 
//	vPickPos += pCam->Transform()->GetWorldPos();
//
//	for (int i = 0; i < MAX_LAYER; ++i)
//	{
//		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
//		for (size_t j = 0; j < vecObject.size(); ++j)
//		{
//			if (vecObject[j]->Transform()->IsCasting(vPickPos))
//			{
//				_vecFindObj.push_back(vecObject[j]);
//			}
//		}
//	}
//
//	sort(_vecFindObj.begin(), _vecFindObj.end(), Compare);
//}

