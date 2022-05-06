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
#include "Sword.h"

#include"GuardHouse.h"
#include"GateHouse.h"
#include"Tree.h"
#include"Somethings.h"
#include"Portal.h"
#include"Miro.h"
#include"Castle.h"
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
	testvec.empty();
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

	Ptr<CTexture> pTrail01 = CResMgr::GetInst()->Load<CTexture>(L"Trail01", L"Texture\\trail\\SwordTrail_0.png");
	Ptr<CTexture> pTrail02 = CResMgr::GetInst()->Load<CTexture>(L"Trail02", L"Texture\\trail\\SwordTrail_1.png");

	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");	
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");
	Ptr<CTexture> pBlock = CResMgr::GetInst()->Load<CTexture>(L"Block", L"Texture\\Bricks.png");


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
	m_pCurScene->GetLayer(4)->SetName(L"Portal");

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
	pObject->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pObject->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	pObject->Light3D()->SetLightRange(1000.f);
	   
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// =============
	// FBX 파일 로드
	// =============
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Basic_Bandit.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	// MeshData 로드
	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\House.mdat", L"MeshData\\monster.mdat");
	
	pObject = pMeshData->Instantiate();
	pObject->SetName(L"Player");
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));

	pObject->AddComponent(new CPlayerScript);
	CPlayerScript* PlayerScript = pObject->GetScript<CPlayerScript>();
	m_pCurScene->AddGameObject(L"Player", pObject, false);

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

	for (int j = 0; j < 5; ++j)
	{
		for (int i = 0; i < 5; ++i)
		{
			pObject = new CGameObject;
			pObject->SetName(L"Map Object");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			// Transform 설정
			pObject->Transform()->SetLocalPos(Vec3(i * 1000.f - 2000.f, 0.f, j*1000.f-1000.f));
			pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
			pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

			// MeshRender 설정
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

			// AddGameObject
			m_pCurScene->FindLayer(L"Map")->AddGameObject(pObject);
		}
	}

	// AddGameObject
	m_pCurScene->FindLayer(L"Map")->AddGameObject(pObject);

	// ===================
	// Player 오브젝트 생성
	// ===================
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale@nIdle1.fbx");
	////pMeshData->Save(pMeshData->GetPath());
	//// MeshData 로드
	////Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\House.mdat", L"MeshData\\monster.mdat");

	//pObject = pMeshData->Instantiate();
	//pObject->SetName(L"House");
	//pObject->FrustumCheck(false);
	//pObject->Transform()->SetLocalPos(Vec3(-250.f, 0.f, -250.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//m_pCurScene->AddGameObject(L"Default", pObject, false);

	//CGameObject* pPlayerObject = nullptr;
	//pPlayerObject = new CGameObject;
	//Ptr<CMeshData> pPlayerMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale@nIdle1.fbx");

	//pPlayerObject = pPlayerMeshData->Instantiate();
	//pPlayerObject->SetName(L"Player Objcet");
	//pPlayerObject->FrustumCheck(false);
	//pPlayerObject->Transform()->SetLocalPos(Vec3(500.f, 125.f, 500.f));
	//pPlayerObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//m_pCurScene->AddGameObject(L"Player", pObject, false);

	//pObject = new CGameObject;
	//pObject->SetName(L"Player Object");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);	

	//// Transform 설정
	//pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
	//pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	//
	//// Script 설정
	//pObject->AddComponent(new CPlayerScript);

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Player")->AddGameObject(pObject);


	//// ====================
	//// Monster 오브젝트 생성
	//// ====================
	//pObject = new CGameObject;
	//pObject->SetName(L"Monster Object");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);	

	//// Transform 설정
	//pObject->Transform()->SetLocalPos(Vec3(0.f, 200.f, 500.f));
	//pObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 1.f));

	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));	
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pNormalTargetTex.GetPointer());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// //Script 설정
	// pObject->AddComponent(new CMonsterScript);

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

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
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky02.GetPointer());

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


		
	
	

	//// SWORD 오브젝트 생성
	//// ====================
	//CGameObject* SwordObject = nullptr;
	//SwordObject = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\sword_2.fbx");
	//SwordObject = pMeshData->Instantiate();
	//SwordObject->SetName(L"Player_Sword");
	//SwordObject->FrustumCheck(false);
	////SwordObject->AddComponent(new CTransform);
	////SwordObject->AddComponent(new CMeshRender);

	//// Transform 설정
	//SwordObject->Transform()->SetLocalPos(Vec3(20.f, 20.f, 20.f));
	//SwordObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	// //Script 설정
	//SwordObject->AddComponent(new CSword);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", SwordObject, false);


	// obstacle 오브젝트 생성
	// ==========================================================================================
	CGameObject* GuardHouseObject = nullptr;

	GuardHouseObject = new CGameObject;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Guardhouses.fbx");
	GuardHouseObject = pMeshData->Instantiate();
	GuardHouseObject->SetName(L"Guard_houses");
	GuardHouseObject->FrustumCheck(false);
	

	// Transform 설정
	
	GuardHouseObject->Transform()->SetLocalPos(Vec3(1000.f, 370.f, 1500.f));
	GuardHouseObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	//Script 설정
	GuardHouseObject->AddComponent(new CGuardHouse);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", GuardHouseObject, false);

	// ===========================================================================================
	CGameObject* GateHouseObject = nullptr;
	GateHouseObject = new CGameObject;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	GateHouseObject = pMeshData->Instantiate();
	GateHouseObject->SetName(L"Gate_houses");
	GateHouseObject->FrustumCheck(false);
	
	// Transform 설정
	GateHouseObject->Transform()->SetLocalPos(Vec3(-1400.f, 200.f,0.f));
	GateHouseObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	//Script 설정
	GateHouseObject->AddComponent(new CGateHouse);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", GateHouseObject, false);

	// ===========================================================================================
	CGameObject* GateHouseObject2 = nullptr;
	GateHouseObject2 = new CGameObject;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	GateHouseObject2 = pMeshData->Instantiate();
	GateHouseObject2->SetName(L"Gate_houses2");
	GateHouseObject2->FrustumCheck(false);
	//SwordObject->AddComponent(new CTransform);
	//SwordObject->AddComponent(new CMeshRender);

	// Transform 설정
	GateHouseObject2->Transform()->SetLocalPos(Vec3(1000.f, 200.f, 0.f));
	GateHouseObject2->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	//Script 설정
	GateHouseObject2->AddComponent(new CGateHouse);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", GateHouseObject2, false);

	// ===========================================================================================
	CGameObject* GateHouseObject3 = nullptr;
	GateHouseObject3 = new CGameObject;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	GateHouseObject3 = pMeshData->Instantiate();
	GateHouseObject3->SetName(L"Gate_houses3");
	GateHouseObject3->FrustumCheck(false);
	//SwordObject->AddComponent(new CTransform);
	//SwordObject->AddComponent(new CMeshRender);

	// Transform 설정
	GateHouseObject3->Transform()->SetLocalPos(Vec3(-1400.f, 200.f, 1000.f));
	GateHouseObject3->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	//Script 설정
	GateHouseObject3->AddComponent(new CGateHouse);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", GateHouseObject3, false);


	//===========================================================================================
	//for (int i = 0; i < 5;  ++i)
	//{
	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Tree.fbx");
	//	CGameObject* Treebject = nullptr;
	//	Treebject = pMeshData->Instantiate();  
	//	Treebject = new CGameObject;
	//	
	//	Treebject->SetName(L"Tree");
	//	Treebject->FrustumCheck(false);
	//	//SwordObject->AddComponent(new CTransform);
	//	//SwordObject->AddComponent(new CMeshRender);

	//	// Transform 설정
	//	Treebject->Transform()->SetLocalPos(Vec3(-500.f, 160.f, -1000.f+i*500));
	//	Treebject->Transform()->SetLocalScale(Vec3(0.1f, 0.1f, 0.1f));

	//	//Script 설정
	//	Treebject->AddComponent(new CTree);
	//	// AddGameObject
	//	m_pCurScene->AddGameObject(L"Default", Treebject, false);

	//}
	////===========================================================================================
	//for (int i = 0; i < 5; ++i)
	//{

	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Tree.fbx");
	//	CGameObject* Treebject = nullptr;
	//	Treebject = pMeshData->Instantiate();
	//	Treebject = new CGameObject;
	//	Treebject->SetName(L"Tree");
	//	Treebject->FrustumCheck(false);
	//	//SwordObject->AddComponent(new CTransform);
	//	//SwordObject->AddComponent(new CMeshRender);

	//	// Transform 설정
	//	Treebject->Transform()->SetLocalPos(Vec3(1400.f, 160.f, -1000.f + i * 500));
	//	Treebject->Transform()->SetLocalScale(Vec3(0.1f, 0.1f, 0.1f));

	//	//Script 설정
	//	Treebject->AddComponent(new CTree);
	//	// AddGameObject
	//	m_pCurScene->AddGameObject(L"Default", Treebject, false);

	//}


	//===========================================================================================
	
	 pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Something.fbx");

	CGameObject* SomethingObject = nullptr;
	SomethingObject = new CGameObject;
	SomethingObject = pMeshData->Instantiate();
	SomethingObject->SetName(L"Something");
	SomethingObject->FrustumCheck(false);
	//SwordObject->AddComponent(new CTransform);
	//SwordObject->AddComponent(new CMeshRender);

	// Transform 설정
	SomethingObject->Transform()->SetLocalPos(Vec3(500.f, 10.f, 500.f));
	SomethingObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	//Script 설정
	SomethingObject->AddComponent(new CSomethings);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", SomethingObject, false);

	
	//==========================================================================================
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DragonJawPortal.fbx");

	CGameObject* PortalObject = nullptr;
	PortalObject = new CGameObject;
	
	PortalObject = pMeshData->Instantiate();
	PortalObject->SetName(L"DragonJawPortal");
	PortalObject->FrustumCheck(false);
	

	// Transform 설정
	PortalObject->Transform()->SetLocalPos(Vec3(12.5f,70.f,3100.f));
	PortalObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	//Script 설정
	PortalObject->AddComponent(new CPortal);
	// AddGameObject
	
	m_pCurScene->AddGameObject(L"Default", PortalObject, false);
	
	

	// ===========================================================================================
	CGameObject* Castle = nullptr;
	Castle = new CGameObject;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Full Castle_New.fbx");
	Castle = pMeshData->Instantiate();
	Castle->SetName(L"Castle");
	Castle->FrustumCheck(false);

	// Transform 설정
	Castle->Transform()->SetLocalPos(Vec3(0.f, 200.f, 1200.f));
	Castle->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));

	//Script 설정
	Castle->AddComponent(new CCastle);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", Castle, false);


	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");

	CGameObject* WallObject2 = nullptr;
	WallObject2 = new CGameObject;
	WallObject2 = pMeshData->Instantiate();
	WallObject2->SetName(L"Miro");
	WallObject2->FrustumCheck(false);

	// Transform 설정
	WallObject2->Transform()->SetLocalPos(Vec3(0.f, 200.f, 3500.f));
	WallObject2->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));

	//Script 설정
	WallObject2->AddComponent(new CMiro);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", WallObject2, false);






	//===================
	// Map 오브젝트 생성
	//===================
	for (int j = 0; j < 10; ++j)
	{
		for (int i = -2; i < 10; ++i)
		{
			pObject = new CGameObject;
			pObject->SetName(L"Map Object");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			// Transform 설정
			pObject->Transform()->SetLocalPos(Vec3(i * 1000.f - 1000.f, 1000.f, j * 1000.f - 1000.f));
			pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
			pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

			// MeshRender 설정
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

			// AddGameObject
			m_pCurScene->FindLayer(L"Map")->AddGameObject(pObject);
		}
	}



	//미로----
	//===========================================================================================

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");

	CGameObject* WallObject = nullptr;
	WallObject = new CGameObject;
	WallObject = pMeshData->Instantiate();
	WallObject->SetName(L"Miro");
	WallObject->FrustumCheck(false);

	// Transform 설정
	WallObject->Transform()->SetLocalPos(Vec3(250.f, 1100.f, 1000.f));
	WallObject->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));

	//Script 설정
	WallObject->AddComponent(new CMiro);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", WallObject, false);


	




	CGameObject* MiroObject = nullptr;
	
	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(250.f, 1100.f, 1000.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	
	
	
	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-250.f, 1100.f, 750.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1000.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(0.f, 1100.f, 1750.f));
	MiroObject->Transform()->SetLocalScale(Vec3(750.f, 400.f, 250.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-500.f, 1100.f, 1150.f));
	MiroObject->Transform()->SetLocalScale(Vec3(500.f, 400.f, 250.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-800.f, 1100.f, 2000.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f)); 
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-350.f, 1100.f, 2250.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f)); 
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	
	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-1400.f, 1100.f, 2625.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f)); 
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-1400.f, 1100.f, 4625.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f)); 
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	
	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(25.f, 1100.f, 3050.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-2000.f, 1100.f, 3550.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 2000.f)); 
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-850.f, 1100.f, 3750.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f)); 
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	
	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(25.f, 1100.f, 3550.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(400.f, 1100.f, 2750.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 500.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(1200.f, 1100.f, 2550.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 250.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	
	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(1200.f, 1100.f, 4550.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 250.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(1850.f, 1100.f, 3500.f));
	MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 3000.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(475.f, 1100.f, 4200.f));
	MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 1100.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-350.f, 1100.f, 4200.f));
	MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-700.f, 1100.f, 5000.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 200.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-1900.f, 1100.f, 5500.f));
	MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 1500.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-1400.f, 1100.f, 5500.f));
	MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 1000.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-600.f, 1100.f, 5900.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 100.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-2600.f, 1100.f, 6200.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 200.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-950.f, 1100.f, 6000.f));
	MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 200.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-3100.f, 1100.f, 7500.f));
	MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 3000.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-400.f, 1100.f, 7500.f));
	MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 3000.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	MiroObject = new CGameObject;
	MiroObject->SetName(L"MIRO");
	MiroObject->AddComponent(new CTransform);
	MiroObject->AddComponent(new CMeshRender);
	// Transform 설정
	MiroObject->Transform()->SetLocalPos(Vec3(-1600.f, 1100.f, 8800.f));
	MiroObject->Transform()->SetLocalScale(Vec3(3000.f, 400.f, 100.f));
	// MeshRender 설정
	MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	
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

