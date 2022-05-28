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
	//m_pCurScene->GetLayer(5)->SetName(L"Sword");
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
	//pPlayer->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
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
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\asdq.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\asdq.mdat", L"MeshData\\asdq.mdat");
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

	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			pObject = new CGameObject;
			pObject->SetName(L"Map Object");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			// Transform 설정
			pObject->Transform()->SetLocalPos(Vec3(i * 1000.f - 1000.f, 0.f, j*1000.f-1000.f));
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


	//// ====================
	//// Monster 오브젝트 생성
	//// ====================
	//CGameObject* pMonsterObject = nullptr;
	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Standing W_Briefcase Idle.mdat", L"MeshData\\Standing W_Briefcase Idle.mdat");
	////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Standing W_Briefcase Idle.fbx");
	////pMeshData->Save(pMeshData->GetPath());

	//pMonsterObject = pMeshData->Instantiate();
	//pMonsterObject->SetName(L"Monster1");
	//pMonsterObject->FrustumCheck(false);
	//pMonsterObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//pMonsterObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pMonsterObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	//pMonsterObject->AddComponent(new CCollider2D);

	////pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH);

	//pMonsterObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::MESH, L"Monster1");
	//pMonsterObject->Collider2D()->SetBB(BoundingBox(pObject->Transform()->GetLocalPos(), pObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	//pMonsterObject->Collider2D()->SetBS(BoundingSphere(pObject->Transform()->GetLocalPos(), pObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));

	//Ptr<CTexture> MonsterTex = CResMgr::GetInst()->FindRes<CTexture>(L"Monster01");
	//pMonsterObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, MonsterTex.GetPointer());

	//// 플레이어 스크립트 붙여주기.
	//pObject->AddComponent(new CMonsterScript);
	//CMonsterScript* MonsterScript = pMonsterObject->GetScript<CMonsterScript>();
	//m_pCurScene->AddGameObject(L"Monster", pMonsterObject, false);

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
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Monster", L"Monster");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Sword", L"Monster");



	//// obstacle 오브젝트 생성
	//// ==========================================================================================
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Guardhouses.fbx");
	//CGameObject* GuardHouseObject = nullptr;
	//
	//GuardHouseObject = new CGameObject;	
	//GuardHouseObject = pMeshData->Instantiate();
	//GuardHouseObject->SetName(L"Guard_houses");
	//GuardHouseObject->FrustumCheck(false);
	////SwordObject->AddComponent(new CTransform);
	////SwordObject->AddComponent(new CMeshRender);

	//// Transform 설정
	//
	//GuardHouseObject->Transform()->SetLocalPos(Vec3(1000.f, 160.f, 800.f));
	//GuardHouseObject->Transform()->SetLocalScale(Vec3(0.3f, 0.3f, 0.3f));

	////Script 설정
	//GuardHouseObject->AddComponent(new CGuardHouse);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", GuardHouseObject, false);

	// ===========================================================================================
	//CGameObject* GateHouseObject = nullptr;
	//GateHouseObject = new CGameObject;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\wall2.fbx");
	////pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");
	//
	//GateHouseObject = pMeshData->Instantiate();
	//GateHouseObject->SetName(L"Gate_houses");
	//GateHouseObject->FrustumCheck(false);

	//// Transform 설정
	//GateHouseObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//GateHouseObject->Transform()->SetLocalScale(Vec3(10.f, 10.0f, 10.0f));
	////GateHouseObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, XMConvertToRadians(0.f)));

	////Script 설정
	////GateHouseObject->AddComponent(new CGateHouse);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", GateHouseObject, false);

	//// ===========================================================================================
	//CGameObject* GateHouseObject2 = nullptr;
	//GateHouseObject2 = new CGameObject;
	////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");

	//GateHouseObject2 = pMeshData->Instantiate();
	//GateHouseObject2->SetName(L"Gate_houses2");
	//GateHouseObject2->FrustumCheck(false);
	////SwordObject->AddComponent(new CTransform);
	////SwordObject->AddComponent(new CMeshRender);

	//// Transform 설정
	//GateHouseObject2->Transform()->SetLocalPos(Vec3(1000.f, 100.f, -500.f));
	//GateHouseObject2->Transform()->SetLocalScale(Vec3(0.3f, 0.3f, 0.3f));

	////Script 설정
	//GateHouseObject2->AddComponent(new CGateHouse);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", GateHouseObject2, false);

	//// ===========================================================================================
	//CGameObject* GateHouseObject3 = nullptr;
	//GateHouseObject3 = new CGameObject;
	////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Gatehouses.fbx");
	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");
	//
	//GateHouseObject3 = pMeshData->Instantiate();
	//GateHouseObject3->SetName(L"Gate_houses3");
	//GateHouseObject3->FrustumCheck(false);
	////SwordObject->AddComponent(new CTransform);
	////SwordObject->AddComponent(new CMeshRender);

	//// Transform 설정
	//GateHouseObject3->Transform()->SetLocalPos(Vec3(-1400.f, 100.f, 800.f));
	//GateHouseObject3->Transform()->SetLocalScale(Vec3(0.3f, 0.3f, 0.3f));

	////Script 설정
	//GateHouseObject3->AddComponent(new CGateHouse);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", GateHouseObject3, false);


	//===========================================================================================
	
	////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Something.fbx");
	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Something.mdat", L"MeshData\\Something.mdat");

	//CGameObject* SomethingObject = nullptr;
	//SomethingObject = new CGameObject;
	//SomethingObject = pMeshData->Instantiate();
	//SomethingObject->SetName(L"Something");
	//SomethingObject->FrustumCheck(false);
	////SwordObject->AddComponent(new CTransform);
	////SwordObject->AddComponent(new CMeshRender);

	//// Transform 설정
	//SomethingObject->Transform()->SetLocalPos(Vec3(500.f, 10.f, 500.f));
	//SomethingObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	////Script 설정
	//SomethingObject->AddComponent(new CSomethings);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", SomethingObject, false);

	
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

	////미로----

	//CGameObject* MiroObject = nullptr;
	//
	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(250.f, 1100.f, 1000.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-250.f, 1100.f, 750.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1000.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	//

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(0.f, 1100.f, 1750.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(750.f, 400.f, 250.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-500.f, 1100.f, 1150.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(500.f, 400.f, 250.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-800.f, 1100.f, 2000.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f)); 
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-350.f, 1100.f, 2250.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f)); 
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	//
	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-1400.f, 1100.f, 2625.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f)); 
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-1400.f, 1100.f, 4625.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f)); 
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	//
	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(25.f, 1100.f, 3050.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-2000.f, 1100.f, 3550.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 2000.f)); 
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-850.f, 1100.f, 3750.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f)); 
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//
	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(25.f, 1100.f, 3550.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 250.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(400.f, 1100.f, 2750.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 500.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(1200.f, 1100.f, 2550.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 250.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);
	//
	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(1200.f, 1100.f, 4550.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 250.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(1850.f, 1100.f, 3500.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 3000.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(475.f, 1100.f, 4200.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 1100.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-350.f, 1100.f, 4200.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(250.f, 400.f, 1500.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-700.f, 1100.f, 5000.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 200.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-1900.f, 1100.f, 5500.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 1500.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-1400.f, 1100.f, 5500.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 1000.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-600.f, 1100.f, 5900.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 100.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-2600.f, 1100.f, 6200.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1500.f, 400.f, 200.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-950.f, 1100.f, 6000.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(1000.f, 400.f, 200.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);


	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-3100.f, 1100.f, 7500.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 3000.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-400.f, 1100.f, 7500.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(100.f, 400.f, 3000.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	//MiroObject = new CGameObject;
	//MiroObject->SetName(L"MIRO");
	//MiroObject->AddComponent(new CTransform);
	//MiroObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//MiroObject->Transform()->SetLocalPos(Vec3(-1600.f, 1100.f, 8800.f));
	//MiroObject->Transform()->SetLocalScale(Vec3(3000.f, 400.f, 100.f));
	//// MeshRender 설정
	//MiroObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//MiroObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(MiroObject);

	
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

