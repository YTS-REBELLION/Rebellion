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
	Ptr<CTexture> pMonsterTex = CResMgr::GetInst()->Load<CTexture>(L"Monster01", L"Texture\\Monster.png");
	Ptr<CTexture> pTrail01 = CResMgr::GetInst()->Load<CTexture>(L"Trail01", L"Texture\\trail\\SwordTrail_0.png");
	Ptr<CTexture> pTrail02 = CResMgr::GetInst()->Load<CTexture>(L"Trail02", L"Texture\\trail\\SwordTrail_1.png");

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
	m_pCurScene->GetLayer(4)->SetName(L"Portal");
	m_pCurScene->GetLayer(5)->SetName(L"Sword");
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
	pObject->Light3D()->SetAmbient(Vec3(0.5f, 0.5f, 0.5f));
	pObject->Light3D()->SetLightDir(Vec3(-1.f, -1.f, -1.f));
	pObject->Light3D()->SetLightRange(1000.f);
	   
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// =============
	// FBX 파일 로드
	// =============
	//CGameObject* pPObject = new CGameObject;
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Idle.mdat", L"MeshData\\Player_Idle.mdat");
	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Idle.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	
	CGameObject* pPlayer = new CGameObject;

	pPlayer = pMeshData->Instantiate(); 
	pPlayer->SetName(L"Player1");
	pPlayer->FrustumCheck(false);
	pPlayer->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vec3(0.08f, 0.08f, 0.08f));
	pPlayer->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pPlayer->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH);


	pPlayer->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pPlayer->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
	pPlayer->Collider2D()->SetOffsetScale(Vec3(850.f, 850.f, 1700.f));

	// 플레이어 스크립트 붙여주기.
	pPlayer->AddComponent(new CPlayerScript);

	CPlayerScript* PlayerScript = pPlayer->GetScript<CPlayerScript>();
	// 플레이어 애니메이션
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 0
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Walk.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Walk.mdat", L"MeshData\\Player_Walk.mdat");
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 1
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Run.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Run.mdat", L"MeshData\\Player_Run.mdat");
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 2
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Attack.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Attack.mdat", L"MeshData\\Player_Attack.mdat");
	PlayerScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 3
	g_net.SetAniData(pMeshData->GetMesh());

	m_pCurScene->AddGameObject(L"Player", pPlayer, false);


	// Sword 객체
	CGameObject* pSwordObject = new CGameObject;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");

	Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");

	pSwordObject = pMeshData->Instantiate();
	pSwordObject->SetName(L"Player_Sword");
	pSwordObject->FrustumCheck(false);
	pSwordObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pSwordObject->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	pSwordObject->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(-90.f), XMConvertToRadians(-90.f)));
	pSwordObject->AddComponent(new CCollider2D);
	pSwordObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pSwordObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pSwordObject->Collider2D()->SetOffsetScale(Vec3(10.f, 140.f, 5.f));

	Ptr<CTexture> SwordObject = CResMgr::GetInst()->FindRes<CTexture>(L"Sword");
	pSwordObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, SwordObject.GetPointer());

	pSwordObject->AddComponent(new CSwordScript);
	CSwordScript* SwordScript = pSwordObject->GetScript<CSwordScript>();
	pSwordObject->GetScript<CSwordScript>()->SetTarget(pPlayer);
	pSwordObject->GetScript<CSwordScript>()->SetBoneIdx(36);

	m_pCurScene->AddGameObject(L"Player", pSwordObject, false);
	pPlayer->AddChild(pSwordObject);



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
	PlayerCamScript->SetCameraToPlayer(pPlayer);
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
			pObject->Transform()->SetLocalPos(Vec3(i * 1000.f - 2000.f, 0.f, j * 1000.f - 1000.f));
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

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Idle.mdat", L"MeshData\\Player_Idle.mdat");
	
	CGameObject* pMonster = new CGameObject;

	pMonster = pMeshData->Instantiate();
	pMonster->SetName(L"Monster1");
	pMonster->FrustumCheck(false);
	pMonster->Transform()->SetLocalPos(Vec3(0.f, 0.f, 500.f));
	pMonster->Transform()->SetLocalScale(Vec3(0.08f, 0.08f, 0.08f));
	pMonster->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pMonster->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH);


	pMonster->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pMonster->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
	pMonster->Collider2D()->SetOffsetScale(Vec3(850.f, 850.f, 1700.f));

	// 플레이어 스크립트 붙여주기.
	pMonster->AddComponent(new CMonsterScript);

	CMonsterScript* MonsterScript = pMonster->GetScript<CMonsterScript>();
	// 플레이어 애니메이션
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 0
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Walk.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Walk.mdat", L"MeshData\\Player_Walk.mdat");
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 1
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Run.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Run.mdat", L"MeshData\\Player_Run.mdat");
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 2
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Attack.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Attack.mdat", L"MeshData\\Player_Attack.mdat");
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 3
	g_net.SetAniData(pMeshData->GetMesh());

	m_pCurScene->AddGameObject(L"Monster", pMonster, false);

	//// ====================
	//// Potal 오브젝트 생성
	//// ====================
	CGameObject* pPotalObject = nullptr;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\DragonJawPortal.mdat", L"MeshData\\DragonJawPortal.mdat");
	//Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DragonJawPortal.fbx");

	pPotalObject = pPMeshData->Instantiate();
	pPotalObject->SetName(L"Portal");
	pPotalObject->FrustumCheck(false);
	pPotalObject->Transform()->SetLocalPos(Vec3(0.f, 50.f, 0.f));
	pPotalObject->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	pPotalObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pPotalObject->AddComponent(new CCollider2D);

	pPotalObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pPotalObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 30.f));
	pPotalObject->Collider2D()->SetOffsetScale(Vec3(150.f, 100.f, 170.f));
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pPotalObject);

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
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject, false);

	g_net.SetObj(pPlayer);
	//CGameObject* MiroObject = nullptr;

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//MiroObject->AddComponent(new CCollider2D);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(500.f, 0.f, 0.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//MiroObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//MiroObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//MiroObject->Collider2D()->SetOffsetScale(Vec3(1.f, 1.0f, 1.f));
	//m_pCurScene->FindLayer(L"Monster")->AddGameObject(MiroObject);

 
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
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Monster", L"Monster");
	



	// obstacle 오브젝트 생성
	// ==========================================================================================
	CGameObject* GuardHouseObject = nullptr;

	GuardHouseObject = new CGameObject;
	 pPMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");
	GuardHouseObject = pPMeshData->Instantiate();
	GuardHouseObject->SetName(L"Guard_houses");
	GuardHouseObject->FrustumCheck(false);


	// Transform 설정

	GuardHouseObject->Transform()->SetLocalPos(Vec3(1000.f, 360.f, 2000.f));
	GuardHouseObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	//Script 설정
	GuardHouseObject->AddComponent(new CGuardHouse);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Default", GuardHouseObject, false);

	//// ===========================================================================================
	//CGameObject* GateHouseObject = nullptr;
	//GateHouseObject = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	//GateHouseObject = pMeshData->Instantiate();
	//GateHouseObject->SetName(L"Gate_houses");
	//GateHouseObject->FrustumCheck(false);

	//// Transform 설정
	//GateHouseObject->Transform()->SetLocalPos(Vec3(-1400.f, 200.f, 0.f));
	//GateHouseObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	////Script 설정
	//GateHouseObject->AddComponent(new CGateHouse);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", GateHouseObject, false);

	//// ===========================================================================================
	//CGameObject* GateHouseObject2 = nullptr;
	//GateHouseObject2 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	//GateHouseObject2 = pMeshData->Instantiate();
	//GateHouseObject2->SetName(L"Gate_houses2");
	//GateHouseObject2->FrustumCheck(false);
	////SwordObject->AddComponent(new CTransform);
	////SwordObject->AddComponent(new CMeshRender);

	//// Transform 설정
	//GateHouseObject2->Transform()->SetLocalPos(Vec3(1000.f, 200.f, 0.f));
	//GateHouseObject2->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	////Script 설정
	//GateHouseObject2->AddComponent(new CGateHouse);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", GateHouseObject2, false);

	//// ===========================================================================================
	//CGameObject* GateHouseObject3 = nullptr;
	//GateHouseObject3 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	//GateHouseObject3 = pMeshData->Instantiate();
	//GateHouseObject3->SetName(L"Gate_houses3");
	//GateHouseObject3->FrustumCheck(false);
	////SwordObject->AddComponent(new CTransform);
	////SwordObject->AddComponent(new CMeshRender);

	//// Transform 설정
	//GateHouseObject3->Transform()->SetLocalPos(Vec3(-1400.f, 200.f, 1000.f));
	//GateHouseObject3->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	////Script 설정
	//GateHouseObject3->AddComponent(new CGateHouse);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", GateHouseObject3, false);

	
	//// ===========================================================================================
	//CGameObject* Castle = nullptr;
	//Castle = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Full Castle_New.fbx");
	//Castle = pMeshData->Instantiate();
	//Castle->SetName(L"Castle");
	//Castle->FrustumCheck(false);

	//// Transform 설정
	//Castle->Transform()->SetLocalPos(Vec3(0.f, 200.f, 1200.f));
	//Castle->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));

	////Script 설정
	//Castle->AddComponent(new CCastle);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", Castle, false);


	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");

	//CGameObject* WallObject2 = nullptr;
	//WallObject2 = new CGameObject;
	//WallObject2 = pMeshData->Instantiate();
	//WallObject2->SetName(L"Miro");
	//WallObject2->FrustumCheck(false);

	//// Transform 설정
	//WallObject2->Transform()->SetLocalPos(Vec3(0.f, 200.f, 3500.f));
	//WallObject2->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));

	////Script 설정
	//WallObject2->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject2, false);


	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster_X_Black.fbx");

	//CGameObject* NpcMon = nullptr;
	//NpcMon = new CGameObject;
	//NpcMon = pMeshData->Instantiate();
	//NpcMon->SetName(L"Miro");
	//NpcMon->FrustumCheck(false);

	//// Transform 설정
	//NpcMon->Transform()->SetLocalPos(Vec3(950.f, 110.f, -560.f));
	//NpcMon->Transform()->SetLocalScale(Vec3(0.03f, 0.03f, 0.03f));

	////Script 설정
	//NpcMon->AddComponent(new CNpcMon);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", NpcMon, false);



	////===================
	//// Map 오브젝트 생성
	////===================
	//for (int j = 0; j < 10; ++j)
	//{
	//	for (int i = -2; i < 10; ++i)
	//	{
	//		pObject = new CGameObject;
	//		pObject->SetName(L"Map Object");
	//		pObject->AddComponent(new CTransform);
	//		pObject->AddComponent(new CMeshRender);

	//		// Transform 설정
	//		pObject->Transform()->SetLocalPos(Vec3(i * 1000.f - 1000.f, 2000.f, j * 1000.f - 1000.f));
	//		pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
	//		pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	//		// MeshRender 설정
	//		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	//		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	//		// AddGameObject
	//		m_pCurScene->FindLayer(L"Map")->AddGameObject(pObject);
	//	}
	//}



	////미로----
	////===========================================================================================

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");

	//CGameObject* WallObject = nullptr;
	//WallObject = new CGameObject;
	//WallObject = pMeshData->Instantiate();
	//WallObject->SetName(L"Miro");
	//WallObject->FrustumCheck(false);

	//// Transform 설정
	//WallObject->Transform()->SetLocalPos(Vec3(400.f, 2200.f, 1000.f));
	//WallObject->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));

	////Script 설정
	//WallObject->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject, false);



	//CGameObject* WallObject3 = nullptr;
	//WallObject3 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject3 = pMeshData->Instantiate();
	//WallObject3->SetName(L"Miro");
	//WallObject3->FrustumCheck(false);

	//// Transform 설정
	//WallObject3->Transform()->SetLocalPos(Vec3(-400.f, 2200.f, 1000.f));
	//WallObject3->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject3->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject3->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject3, false);


	//CGameObject* WallObject4 = nullptr;
	//WallObject4 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject4 = pMeshData->Instantiate();
	//WallObject4->SetName(L"Miro");
	//WallObject4->FrustumCheck(false);

	//// Transform 설정
	//WallObject4->Transform()->SetLocalPos(Vec3(300.f, 2200.f, 2600.f));
	//WallObject4->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject4->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject4->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject4, false);


	//CGameObject* WallObject5 = nullptr;
	//WallObject5 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject5 = pMeshData->Instantiate();
	//WallObject5->SetName(L"Miro");
	//WallObject5->FrustumCheck(false);

	//// Transform 설정
	//WallObject5->Transform()->SetLocalPos(Vec3(-1400.f, 2200.f, 1700.f));
	//WallObject5->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject5->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject5->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject5, false);

	//CGameObject* WallObject6 = nullptr;
	//WallObject6 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject6 = pMeshData->Instantiate();
	//WallObject6->SetName(L"Miro");
	//WallObject6->FrustumCheck(false);

	//// Transform 설정
	//WallObject6->Transform()->SetLocalPos(Vec3(-1600.f, 2200.f, 2450.f));
	//WallObject6->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject6->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject6->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject6, false);



	//CGameObject* WallObject7 = nullptr;
	//WallObject7 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject7 = pMeshData->Instantiate();
	//WallObject7->SetName(L"Miro");
	//WallObject7->FrustumCheck(false);

	//// Transform 설정
	//WallObject7->Transform()->SetLocalPos(Vec3(-400.f, 2200.f, 2750.f));
	//WallObject7->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject7->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject7->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject7, false);


	//CGameObject* WallObject8 = nullptr;
	//WallObject8 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject8 = pMeshData->Instantiate();
	//WallObject8->SetName(L"Miro");
	//WallObject8->FrustumCheck(false);

	//// Transform 설정
	//WallObject8->Transform()->SetLocalPos(Vec3(-1100.f, 2200.f, 3550.f));
	//WallObject8->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject8->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject8->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject8, false);




	//CGameObject* WallObject9 = nullptr;
	//WallObject9 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject9 = pMeshData->Instantiate();
	//WallObject9->SetName(L"Miro");
	//WallObject9->FrustumCheck(false);

	//// Transform 설정
	//WallObject9->Transform()->SetLocalPos(Vec3(-2600.f, 2200.f, 3300.f));
	//WallObject9->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject9->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject9->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject9, false);




	//CGameObject* WallObject10 = nullptr;
	//WallObject10 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject10 = pMeshData->Instantiate();
	//WallObject10->SetName(L"Miro");
	//WallObject10->FrustumCheck(false);

	//// Transform 설정
	//WallObject10->Transform()->SetLocalPos(Vec3(-3200.f, 2200.f, 4000.f));
	//WallObject10->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject10->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject10->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject10, false);



	//CGameObject* WallObject11 = nullptr;
	//WallObject11 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject11 = pMeshData->Instantiate();
	//WallObject11->SetName(L"Miro");
	//WallObject11->FrustumCheck(false);

	//// Transform 설정
	//WallObject11->Transform()->SetLocalPos(Vec3(-1800.f, 2200.f, 4800.f));
	//WallObject11->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject11->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject11->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject11, false);


	//CGameObject* WallObject12 = nullptr;
	//WallObject12 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject12 = pMeshData->Instantiate();
	//WallObject12->SetName(L"Miro");
	//WallObject12->FrustumCheck(false);

	//// Transform 설정
	//WallObject12->Transform()->SetLocalPos(Vec3(-2600.f, 2200.f, 5600.f));
	//WallObject12->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject12->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject12->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject12, false);



	//CGameObject* WallObject13 = nullptr;
	//WallObject13 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject13 = pMeshData->Instantiate();
	//WallObject13->SetName(L"Miro");
	//WallObject13->FrustumCheck(false);

	//// Transform 설정
	//WallObject13->Transform()->SetLocalPos(Vec3(-3300.f, 2200.f, 5200.f));
	//WallObject13->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject13->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject13->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject13, false);


	//CGameObject* WallObject14 = nullptr;
	//WallObject14 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject14 = pMeshData->Instantiate();
	//WallObject14->SetName(L"Miro");
	//WallObject14->FrustumCheck(false);

	//// Transform 설정
	//WallObject14->Transform()->SetLocalPos(Vec3(-3500.f, 2200.f, 6700.f));
	//WallObject14->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject14->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject14->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject14, false);


	//CGameObject* WallObject15 = nullptr;
	//WallObject15 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject15 = pMeshData->Instantiate();
	//WallObject15->SetName(L"Miro");
	//WallObject15->FrustumCheck(false);

	//// Transform 설정
	//WallObject15->Transform()->SetLocalPos(Vec3(-650.f, 2200.f, 5000.f));
	//WallObject15->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject15->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject15->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject15, false);



	//CGameObject* WallObject16 = nullptr;
	//WallObject16 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject16 = pMeshData->Instantiate();
	//WallObject16->SetName(L"Miro");
	//WallObject16->FrustumCheck(false);

	//// Transform 설정
	//WallObject16->Transform()->SetLocalPos(Vec3(-250.f, 2200.f, 4250.f));
	//WallObject16->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject16->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject16->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject16, false);


	//CGameObject* WallObject17 = nullptr;
	//WallObject17 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject17 = pMeshData->Instantiate();
	//WallObject17->SetName(L"Miro");
	//WallObject17->FrustumCheck(false);

	//// Transform 설정
	//WallObject17->Transform()->SetLocalPos(Vec3(800.f, 2200.f, 4250.f));
	//WallObject17->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject17->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject17->AddComponent(new CMiro);
	//// AddGameObjectd
	//m_pCurScene->AddGameObject(L"Default", WallObject17, false);

	//CGameObject* WallObject18 = nullptr;
	//WallObject18 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject18 = pMeshData->Instantiate();
	//WallObject18->SetName(L"Miro");
	//WallObject18->FrustumCheck(false);

	//// Transform 설정
	//WallObject18->Transform()->SetLocalPos(Vec3(1600.f, 2200.f, 5200.f));
	//WallObject18->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject18->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject18->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject18, false);

	//CGameObject* WallObject19 = nullptr;
	//WallObject19 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject19 = pMeshData->Instantiate();
	//WallObject19->SetName(L"Miro");
	//WallObject19->FrustumCheck(false);

	//// Transform 설정
	//WallObject19->Transform()->SetLocalPos(Vec3(-50.f, 2200.f, 5800.f));
	//WallObject19->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject19->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject19->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject19, false);


	//CGameObject* WallObject20 = nullptr;
	//WallObject20 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject20 = pMeshData->Instantiate();
	//WallObject20->SetName(L"Miro");
	//WallObject20->FrustumCheck(false);

	//// Transform 설정
	//WallObject20->Transform()->SetLocalPos(Vec3(700.f, 2200.f, 6700.f));
	//WallObject20->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject20->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject20->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject20, false);



	//CGameObject* WallObject21 = nullptr;
	//WallObject21 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject21 = pMeshData->Instantiate();
	//WallObject21->SetName(L"Miro");
	//WallObject21->FrustumCheck(false);

	//// Transform 설정
	//WallObject21->Transform()->SetLocalPos(Vec3(1450.f, 2200.f, 6700.f));
	//WallObject21->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject21->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject21->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject21, false);

	////두번쨰 던전방까지

	//CGameObject* WallObject22 = nullptr;
	//WallObject22 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject22 = pMeshData->Instantiate();
	//WallObject22->SetName(L"Miro");
	//WallObject22->FrustumCheck(false);

	//// Transform 설정
	//WallObject22->Transform()->SetLocalPos(Vec3(-3600.f, 2200.f, 8300.f));
	//WallObject22->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject22->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject22->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject22, false);


	//CGameObject* WallObject23 = nullptr;
	//WallObject23 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject23 = pMeshData->Instantiate();
	//WallObject23->SetName(L"Miro");
	//WallObject23->FrustumCheck(false);

	//// Transform 설정
	//WallObject23->Transform()->SetLocalPos(Vec3(-2700.f, 2200.f, 8500.f));
	//WallObject23->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject23->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject23->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject23, false);


	//CGameObject* WallObject24 = nullptr;
	//WallObject24 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject24 = pMeshData->Instantiate();
	//WallObject24->SetName(L"Miro");
	//WallObject24->FrustumCheck(false);

	//// Transform 설정
	//WallObject24->Transform()->SetLocalPos(Vec3(-1200.f, 2200.f, 8500.f));
	//WallObject24->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject24->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject24->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject24, false);

	//CGameObject* WallObject25 = nullptr;
	//WallObject25 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject25 = pMeshData->Instantiate();
	//WallObject25->SetName(L"Miro");
	//WallObject25->FrustumCheck(false);

	//// Transform 설정
	//WallObject25->Transform()->SetLocalPos(Vec3(300.f, 2200.f, 8500.f));
	//WallObject25->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject25->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject25->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject25, false);



	//CGameObject* WallObject26 = nullptr;
	//WallObject26 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject26 = pMeshData->Instantiate();
	//WallObject26->SetName(L"Miro");
	//WallObject26->FrustumCheck(false);

	//// Transform 설정
	//WallObject26->Transform()->SetLocalPos(Vec3(1300.f, 2200.f, 8200.f));
	//WallObject26->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject26->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject26->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject26, false);


	////3번째 방끝


	//CGameObject* WallObject27 = nullptr;
	//WallObject27 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject27 = pMeshData->Instantiate();
	//WallObject27->SetName(L"Miro");
	//WallObject27->FrustumCheck(false);

	//// Transform 설정
	//WallObject27->Transform()->SetLocalPos(Vec3(-1300.f, 2200.f, -1400.f));
	//WallObject27->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject27->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject27->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject27, false);

	//CGameObject* WallObject28 = nullptr;
	//WallObject28 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject28 = pMeshData->Instantiate();
	//WallObject28->SetName(L"Miro");
	//WallObject28->FrustumCheck(false);

	//// Transform 설정
	//WallObject28->Transform()->SetLocalPos(Vec3(1300.f, 2200.f, -1400.f));
	//WallObject28->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject28->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject28->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject28, false);


	//CGameObject* WallObject29 = nullptr;
	//WallObject29 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject29 = pMeshData->Instantiate();
	//WallObject29->SetName(L"Miro");
	//WallObject29->FrustumCheck(false);

	//// Transform 설정
	//WallObject29->Transform()->SetLocalPos(Vec3(-2000.f, 2200.f, -500.f));
	//WallObject29->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject29->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject29->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject29, false);



	//CGameObject* WallObject30 = nullptr;
	//WallObject30 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject30 = pMeshData->Instantiate();
	//WallObject30->SetName(L"Miro");
	//WallObject30->FrustumCheck(false);

	//// Transform 설정
	//WallObject30->Transform()->SetLocalPos(Vec3(-2100.f, 2200.f, 1000.f));
	//WallObject30->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject30->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject30->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject30, false);


	//CGameObject* WallObject31 = nullptr;
	//WallObject31 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject31 = pMeshData->Instantiate();
	//WallObject31->SetName(L"Miro");
	//WallObject31->FrustumCheck(false);

	//// Transform 설정
	//WallObject31->Transform()->SetLocalPos(Vec3(2000.f, 2200.f, -600.f));
	//WallObject31->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject31->Transform()->SetLocalRot(Vec3(0.f, 1.5f, 0.f));
	////Script 설정
	//WallObject31->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject31, false);


	//CGameObject* WallObject32 = nullptr;
	//WallObject32 = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Wall 1_New.fbx");
	//WallObject32 = pMeshData->Instantiate();
	//WallObject32->SetName(L"Miro");
	//WallObject32->FrustumCheck(false);

	//// Transform 설정
	//WallObject32->Transform()->SetLocalPos(Vec3(1300.f, 2200.f, 400.f));
	//WallObject32->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//WallObject32->Transform()->SetLocalRot(Vec3(0.f, 40.8351f, 0.f));
	////Script 설정
	//WallObject32->AddComponent(new CMiro);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", WallObject32, false);

	
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

