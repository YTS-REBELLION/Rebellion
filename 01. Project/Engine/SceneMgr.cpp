#include "stdafx.h"
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
#include "ToolCamScript.h"
#include "GridScript.h"

#include "meshdata.h"

CScene* CSceneMgr::GetCurScene()
{
	return m_pCurScene;
}

void CSceneMgr::ChangeScene(CScene * _pNextScene)
{
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = _pNextScene;
}

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)	
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
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
	// =================
	// 필요한 리소스 로딩
	// =================
	// Texture 로드
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");	
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");

	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_01.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_01_N.tga");

	Ptr<CTexture> pDiffuseTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex");
	Ptr<CTexture> pNormalTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex");
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");

	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());

	pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_2, pSky01.GetPointer());
	
	// ===============
	// Test Scene 생성
	// ===============
	m_pCurScene = new CScene;
	m_pCurScene->SetName(L"Test Scene");

	// ===============
	// Layer 이름 지정
	// ===============
	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer(1)->SetName(L"Player");
	m_pCurScene->GetLayer(2)->SetName(L"Monster");
	m_pCurScene->GetLayer(3)->SetName(L"Map");

	m_pCurScene->GetLayer(30)->SetName(L"UI");
	m_pCurScene->GetLayer(31)->SetName(L"Tool");

	CGameObject* pObject = nullptr;
	
	//// UI Camera
	//CGameObject* pUICam = new CGameObject;
	//pUICam->SetName(L"MainCam");
	//pUICam->AddComponent(new CTransform);
	//pUICam->AddComponent(new CCamera);	

	//pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);
	//pUICam->Camera()->SetFar(100.f);	
	//pUICam->Camera()->SetLayerCheck(30, true);	

	//m_pCurScene->FindLayer(L"Default")->AddGameObject(pUICam);


	//CreateTargetUI();

	// ====================
	// 3D Light Object 추가
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight3D);	   	
	
	pObject->Light3D()->SetLightPos(Vec3(0.f, 500.f, 0.f));
	pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light3D()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	pObject->Light3D()->SetAmbient(Vec3(0.3f, 0.3f, 0.3f));
	pObject->Light3D()->SetLightDir(Vec3(-1.f, -1.f, -1.f));
	pObject->Light3D()->SetLightRange(1000.f);
	   
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// =============
	// FBX 파일 로드
	// =============
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale@nIdle1.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	
	pObject = pMeshData->Instantiate();
	pObject->SetName(L"Player1");
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);

	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH);

	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH, L"Player1");
	pObject->Collider2D()->SetBB(BoundingBox(pObject->Transform()->GetLocalPos(), pObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pObject->Collider2D()->SetBS(BoundingSphere(pObject->Transform()->GetLocalPos(), pObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));

	// 플레이어 스크립트 붙여주기.
	pObject->AddComponent(new CPlayerScript);
	CPlayerScript* PlayerScript = pObject->GetScript<CPlayerScript>();
	m_pCurScene->AddGameObject(L"Player", pObject, false);

	// 플레이어 애니메이션
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 0

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale@nWalk_F.fbx");
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 1

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale@nRun_F.fbx");
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 2

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale@Attack1.fbx");
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 3

	//// 더미 플레이어 -> 초기 캐릭터가 누워있는거를 회전 시키면 카메라도 같이 회전해서 생성.
	//Ptr<CMeshData> DmypMeshData;

	//CGameObject* DmypObject = new CGameObject;
	//DmypObject = DmypMeshData->DmyInstantiate();
	//DmypObject->SetName(L"Dummy_Player");
	//DmypObject->FrustumCheck(false);
	//DmypObject->Transform()->SetLocalPos(Vec3(0.f,0.f,0.f));

	//DmypObject->AddComponent(new CPlayerScript);
	//CPlayerScript* Dmy_PlayerScript = DmypObject->GetScript<CPlayerScript>();
	//m_pCurScene->AddGameObject(L"Player", DmypObject, false);

	// ==================
	// Camera Object 생성
	// ==================
	//Main Camera
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CToolCamScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck(30, false);

	CToolCamScript* PlayerCamScript = pMainCam->GetScript<CToolCamScript>();
	PlayerCamScript->SetCameraToPlayer(pObject);
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pMainCam);

	// ==================
	// Map 오브젝트 생성
	// ==================
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
	pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	////Script 설정
	//pMapObject->AddComponent(new CPlayerScript);

	// AddGameObject
	m_pCurScene->FindLayer(L"Map")->AddGameObject(pObject);


	// ====================
	// Monster 오브젝트 생성
	// ====================
	//CGameObject* pMObject = nullptr;
	//pMObject = new CGameObject;
	//pMObject->SetName(L"Monster");
	//pMObject->AddComponent(new CTransform);
	//pMObject->AddComponent(new CMeshRender);
	//pMObject->AddComponent(new CCollider2D);

	//// Transform 설정
	//pMObject->Transform()->SetLocalPos(Vec3(100.f, 100.f, 0.f));
	//pMObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

	//// MeshRender 설정
	//pMObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pMObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	//pMObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pNormalTargetTex.GetPointer());
	//pMObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	//pMObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX, L"Monster");
	//pMObject->Collider2D()->SetBB(BoundingBox(pMObject->Transform()->GetLocalPos(), pMObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	//pMObject->Collider2D()->SetBS(BoundingSphere(pMObject->Transform()->GetLocalPos(), pMObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));

	 //Script 설정
	//pMObject->AddComponent(new CMonsterScript);

	// AddGameObject
	//m_pCurScene->FindLayer(L"Monster")->AddGameObject(pMObject);

	// ====================
	// Potal 오브젝트 생성
	// ====================
	CGameObject* pPObject = nullptr;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DragonJawPortal.fbx");

	pPObject = pPMeshData->Instantiate();
	pPObject->SetName(L"Portal");
	pPObject->FrustumCheck(false);
	pPObject->Transform()->SetLocalPos(Vec3(300.f, 0.f, 0.f));
	pPObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pPObject->AddComponent(new CCollider2D);

	pPObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH, L"Portal");
	pPObject->Collider2D()->SetBB(BoundingBox(pPObject->Transform()->GetLocalPos(), pPObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pPObject->Collider2D()->SetBS(BoundingSphere(pPObject->Transform()->GetLocalPos(), pPObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));

	// 플레이어 스크립트 붙여주기.
	//pObject->AddComponent(new CPlayerScript);
	//CPlayerScript* PlayerScript = pObject->GetScript<CPlayerScript>();
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pPObject);

	// ====================
	// Skybox 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);	

	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	//// ====================
	//// Grid 오브젝트 생성
	//// ====================
	//pObject = new CGameObject;
	//pObject->SetName(L"Grid");
	//pObject->FrustumCheck(false);
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CGridScript);

	//// Transform 설정
	//pObject->Transform()->SetLocalScale(Vec3(100000.f, 100000.f, 1.f));
	//pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GridMtrl"));	

	//// Script 설정	
	//pObject->GetScript<CGridScript>()->SetToolCamera(pMainCam);
	//pObject->GetScript<CGridScript>()->SetGridColor(Vec3(0.8f, 0.2f, 0.2f));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Tool")->AddGameObject(pObject);
		

	// =================================
	// CollisionMgr 충돌 그룹(Layer) 지정
	// =================================
	// Player Layer 와 Monster Layer 는 충돌 검사 진행
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Monster");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Monster");
	
	m_pCurScene->awake();
	m_pCurScene->start();
}

void CSceneMgr::update()
{		
	m_pCurScene->update();
	m_pCurScene->lateupdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();

	m_pCurScene->finalupdate();
	   
	// 충돌 처리
	CCollisionMgr::GetInst()->update();
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

