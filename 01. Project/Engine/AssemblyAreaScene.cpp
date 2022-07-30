#include "stdafx.h"
#include "AssemblyAreaScene.h"

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
#include "Light3D.h"
#include "MeshData.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"


#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "Device.h"
#include "Core.h"

#include "PlayerScript.h"
#include "SwordScript.h"
#include "ToolCamScript.h"


void CAssemblyAreaScene::CreateMap()
{
	// ====================
	// 바닥 오브젝트 생성
	// ====================

	//Ptr<CMeshData> pTestMapMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Map\\g5.fbx");
	//pTestMapMeshData->Save(pTestMapMeshData->GetPath());
	Ptr<CMeshData> pTestMapMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Ground.mdat", L"MeshData\\Ground.mdat");

	CGameObject* Tile = new CGameObject;

	Tile->SetName(L"Map_Tile");
	Tile = pTestMapMeshData->Instantiate();
	Tile->FrustumCheck(false);
	
	// Transform 설정
	Tile->Transform()->SetLocalPos(Vec3(-2500.f, -50.f, -2500.f));
	Tile->Transform()->SetLocalScale(Vec3(3.f, 3.f, 1.f));
	Tile->Transform()->SetLocalRot(Vec3(XMConvertToRadians(90.0f), 0.f, 0.f));

	FindLayer(L"Map")->AddGameObject(Tile);


	// ====================
	// Map 오브젝트 생성
	// ====================

	Ptr<CMeshData> pMapMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Map\\Assembly_Map.fbx");
	pMapMeshData->Save(pMapMeshData->GetPath());
	//Ptr<CMeshData> pMapMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Assembly_Map.mdat", L"MeshData\\Assembly_Map.mdat");

	CGameObject* pMap = new CGameObject;

	pMap->SetName(L"Assembly_Map");
	pMap = pMapMeshData->Instantiate();
	pMap->FrustumCheck(false);

	// Transform 설정
	pMap->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pMap->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pMap->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));

	FindLayer(L"Map")->AddGameObject(pMap);

	// ====================
	// Skybox 오브젝트 생성
	// ====================

	Ptr<CTexture> pSkyboxTex = CResMgr::GetInst()->Load<CTexture>(L"SB", L"Texture\\Skybox\\Deep Dusk Equirect.png");

	CGameObject* pSkybox = new CGameObject;
	pSkybox->SetName(L"SkyBox");
	pSkybox->FrustumCheck(false);
	pSkybox->AddComponent(new CTransform);
	pSkybox->AddComponent(new CMeshRender);

	// MeshRender 설정
	pSkybox->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pSkybox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSkyboxTex.GetPointer());

	// AddGameObject
	FindLayer(L"Default")->AddGameObject(pSkybox, false);
}

void CAssemblyAreaScene::init()
{
	cout << "집결지 컴백" << endl;
	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Player");
	GetLayer(2)->SetName(L"House");
	GetLayer(3)->SetName(L"Map");
	GetLayer(4)->SetName(L"Portal");
	GetLayer(5)->SetName(L"UI");

	CreateMap();

	// ====================
	// 3D Light Object 추가
	// ====================
	CGameObject* pLight = new CGameObject;
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight3D);

	pLight->Light3D()->SetLightPos(Vec3(0.f, 500.f, 0.f));
	pLight->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pLight->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	pLight->Light3D()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	pLight->Light3D()->SetAmbient(Vec3(0.8f, 0.8f, 0.8f));
	pLight->Light3D()->SetLightDir(Vec3(-1.f, -1.f, -1.f));
	pLight->Light3D()->SetLightRange(1000.f);

	FindLayer(L"Default")->AddGameObject(pLight);

	// ===================
	// Player 파일 로드
	// ===================

	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Idle.mdat", L"MeshData\\Player_FM_Idle.mdat");

	CGameObject* pPlayer = new CGameObject;

	pPlayer = pMeshData->Instantiate();
	pPlayer->SetName(L"FM_Player");
	pPlayer->FrustumCheck(false);
	pPlayer->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
	pPlayer->Transform()->SetLocalRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(180.f), 0.f));
	
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	pPlayer->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetOffsetScale(Vec3(70.f, 70.f, 70.f));
	pPlayer->Collider2D()->SetOffsetRot(Vec3(0.f, 0.f, XMConvertToRadians(-180.f)));

	// 플레이어 스크립트 붙여주기.
	pPlayer->AddComponent(new CPlayerScript);

	CPlayerScript* PlayerScript = pPlayer->GetScript<CPlayerScript>();
	pPlayer->GetScript<CPlayerScript>()->init();

	// 플레이어 애니메이션
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 0, 0, 55);							// AniData Index 0
	g_net.SetAniData(pMeshData->GetMesh());

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Walk.mdat", L"MeshData\\Player_FM_Walk.mdat");
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 1, 0, 30);							// AniData Index 1
	g_net.SetAniData(pMeshData->GetMesh());

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Run.mdat", L"MeshData\\Player_FM_Run.mdat");
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 2, 0, 22);						// AniData Index 2
	g_net.SetAniData(pMeshData->GetMesh());

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Attack.mdat", L"MeshData\\Player_FM_Attack.mdat");
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 3, 0, 45);							// AniData Index 3
	g_net.SetAniData(pMeshData->GetMesh());

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Skill_1.mdat", L"MeshData\\Player_FM_Skill_1.mdat");
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 4, 0, 75);							// AniData Index 3
	g_net.SetAniData(pMeshData->GetMesh());

	FindLayer(L"Player")->AddGameObject(pPlayer);
	
	g_net.SetObj(pPlayer);

	//Main Camera
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CToolCamScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck(5, false);

	CToolCamScript* PlayerCamScript = pMainCam->GetScript<CToolCamScript>();
	PlayerCamScript->SetCameraToPlayer(pPlayer);
	FindLayer(L"Default")->AddGameObject(pMainCam);

	// UI Camera
	/*CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICam");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);	
	
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);
	pUICam->Camera()->SetFar(100.f);	
	pUICam->Camera()->SetLayerCheck(5, true);	
	
	FindLayer(L"Default")->AddGameObject(pUICam);*/


	// ===================
	// 임시 파일 로드
	// ===================

	Ptr<CMeshData> pTestMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\FM_Idle.fbx");

	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX<CMeshData>(L"MeshData\\Player_FM_Idle.mdat", L"MeshData\\Player_FM_Idle.mdat");

	CGameObject* pTest = new CGameObject;

	pTest = pTestMeshData->Instantiate();
	pTest->SetName(L"test_m");
	pTest->FrustumCheck(false);

	pTest->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pTest->Transform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	pTest->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));

	//pTest->AddComponent(new CCollider2D);
	//pTest->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	//pTest->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pTest->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	//pTest->Collider2D()->SetOffsetRot(Vec3(0.f, 0.f, XMConvertToRadians(-180.f)));

	FindLayer(L"Default")->AddGameObject(pTest);
}