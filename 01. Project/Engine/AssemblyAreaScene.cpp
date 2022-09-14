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
#include "MonsterScript.h"
#include "SwordScript.h"
#include "ToolCamScript.h"
#include "Fire.h"
#include "ParticleSystem.h"
#include "ParticleScript.h"
#include "Boss.h"

#include "PlayerColScript.h"
#include "MonsterColScript.h"
#include "SwordAttackAreaScript.h"

void CAssemblyAreaScene::CreateMap()
{
	
	// 맵 콜라이더 만들기

	// 1 가운데나무
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(38.f, 50.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(550.f, 100.f, 450.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);


	// 2
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(40.f, 50.f, -1950.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(3500.f, 100.f, 100.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);

	// 3
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(-1850.f, 50.f, -120.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 3500.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);


	// 4
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(1850.f, 50.f, -120.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 3500.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);

	// 5 우물
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(-950.f, 50.f, -1042.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(200.f, 100.f, 200.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);

	// 6 마차
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(1043.f, 50.f, -1300.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(1000.f, 100.f, 1000.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);

	// 7 캠프
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(1353.f, 50.f, 420.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 100.f, 600.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);


	// 8 캠프
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(1353.f, 50.f, 1131.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 100.f, 600.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);


	// 9 돌
	pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(796, 50.f, 723));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(150.f, 100.f, 150.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	FindLayer(L"Map")->AddGameObject(pObject);

	

	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_01.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_01_N.tga");
	// ==================
	// Map 오브젝트 생성
	// ==================

	//g_SoundList.find(Sound_Type::BGM)->second->Play(0);

	for (int j = 0; j < 6; ++j)
	{
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pObject = new CGameObject;
			pObject->SetName(L"Map Object");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);
	
			// Transform 설정
			pObject->Transform()->SetLocalPos(Vec3(i * 1000.f - 2000.f, 0.f, j * 1000.f - 2000.f));
			pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
			pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	
			// MeshRender 설정
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	
			// AddGameObject
			FindLayer(L"Default")->AddGameObject(pObject);
	
		}
	}
	// ====================
	// Map 오브젝트 생성
	// ====================
	Ptr<CMeshData> pMapMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Assembly_Map.mdat", L"MeshData\\Assembly_Map.mdat");
	CGameObject* pMap = new CGameObject;

	pMap->SetName(L"Assembly_Map");
	pMap = pMapMeshData->Instantiate();
	pMap->FrustumCheck(false);
	pMap->MeshRender()->SetDynamicShadow(true);

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

	pSkybox->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pSkybox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSkyboxTex.GetPointer());

	FindLayer(L"Default")->AddGameObject(pSkybox, false);
}

void CAssemblyAreaScene::init()
{
	cout << "집결지 컴백" << endl;
	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Player");
	GetLayer(2)->SetName(L"PlayerSword");
	GetLayer(3)->SetName(L"PlayerCollider");
	GetLayer(4)->SetName(L"Map");
	GetLayer(5)->SetName(L"Portal");
	GetLayer(6)->SetName(L"UI");
	GetLayer(7)->SetName(L"NPC");
	GetLayer(8)->SetName(L"Player_Skill");

	CreateMap();

	// ====================
	// 3D Light Object 추가
	// ====================
	CGameObject* pLight = new CGameObject;
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight3D);

	pLight->Light3D()->SetLightPos(Vec3(2000.f, 3000.f, 2000.f));
	pLight->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pLight->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	pLight->Light3D()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	pLight->Light3D()->SetAmbient(Vec3(0.8f, 0.8f, 0.8f));
	pLight->Light3D()->SetLightDir(Vec3(-1.f, -1.f, -1.f));
	pLight->Light3D()->SetLightRange(10000.f);
	//pLight->Transform()->SetLocalPos(Vec3(0.f, 1000.f, 0.f));
	FindLayer(L"Default")->AddGameObject(pLight);

	// ===================
	// Player 파일 로드
	// ===================

	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\FM_Player\\FP_Idle.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FM_Idle.mdat", L"MeshData\\FM_Idle.mdat");

	CGameObject* pPlayer = new CGameObject;

	pPlayer = pMeshData->Instantiate();
	pPlayer->SetName(L"FM_Player");
	pPlayer->FrustumCheck(false);

	pPlayer->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	pPlayer->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));

	pPlayer->MeshRender()->SetDynamicShadow(true);
	// 플레이어 스크립트 붙여주기.
	pPlayer->AddComponent(new CPlayerScript);

	CPlayerScript* PlayerScript = pPlayer->GetScript<CPlayerScript>();
	pPlayer->GetScript<CPlayerScript>()->init();

	//// 플레이어 애니메이션
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 0, 0, 100);
	g_net.SetAniData(pMeshData->GetMesh());

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FP_Walk.mdat", L"MeshData\\FP_Walk.mdat");
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 1, 0, 36);
	g_net.SetAniData(pMeshData->GetMesh());

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FP_Run.mdat", L"MeshData\\FP_Run.mdat");
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 2, 0, 21);
	g_net.SetAniData(pMeshData->GetMesh());

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FP_Attack.mdat", L"MeshData\\FP_Attack.mdat");
	PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 3, 0, 45);
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Skill_1.mdat", L"MeshData\\Player_FM_Skill_1.mdat");
	//PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 4, 0, 75);							// AniData Index 3
	//g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_FM_Die.mdat", L"MeshData\\Player_FM_Die.mdat");
	//PlayerScript->SetPlayerAnimationData(pMeshData->GetMesh(), 5, 0, 100);
	//g_net.SetAniData(pMeshData->GetMesh());
	FindLayer(L"Player")->AddGameObject(pPlayer);

	g_net.SetObj(pPlayer);

	Ptr<CMeshData> pSwordMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Weapon.mdat", L"MeshData\\Monster_FM_Weapon.mdat");

	CGameObject* pSword = new CGameObject;

	pSword = pSwordMeshData->Instantiate();
	pSword->SetName(L"FP_Weapon");
	pSword->FrustumCheck(false);
	pSword->Transform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
	pSword->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pSword->AddComponent(new CSwordScript);
	pSword->GetScript<CSwordScript>()->init(PERSON_OBJ_TYPE::WARRIOR_PLAYER, pPlayer, 17);
	pPlayer->AddChild(pSword);
	pSword->MeshRender()->SetDynamicShadow(true);

	CGameObject* pPlayerCol = new CGameObject;
	pPlayerCol->SetName(L"PlayerCol");
	pPlayerCol->AddComponent(new CCollider2D);
	pPlayerCol->AddComponent(new CTransform);
	pPlayerCol->AddComponent(new CMeshRender);
	pPlayerCol->Transform()->SetLocalPos(pPlayer->Transform()->GetLocalPos());
	pPlayerCol->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pPlayerCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPlayerCol->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));

	pPlayerCol->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	pPlayerCol->Collider2D()->SetOffsetScale(Vec3(70.f, 70.f, 70.f));
	pPlayerCol->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pPlayerCol->AddComponent(new CPlayerColScript);
	pPlayerCol->GetScript<CPlayerColScript>()->SetPlayer(pPlayer);
	pPlayer->GetScript<CPlayerScript>()->SetColPlayer(pPlayerCol);

	FindLayer(L"PlayerCollider")->AddGameObject(pPlayerCol);

	CGameObject* pSwordCol = new CGameObject;
	pSwordCol->SetName(L"PlayerSwordCol");
	pSwordCol->AddComponent(new CCollider2D);
	pSwordCol->AddComponent(new CTransform);
	pSwordCol->AddComponent(new CMeshRender);
	pSwordCol->Transform()->SetLocalPos(pPlayer->Transform()->GetLocalPos());
	pSwordCol->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pSwordCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pSwordCol->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pSwordCol->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	pSwordCol->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	pSwordCol->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pSwordCol->AddComponent(new CSwordAttackAreaScript);
	pSwordCol->GetScript<CSwordAttackAreaScript>()->Set_Object(pPlayer);
	pSwordCol->SetActive(false);
	pPlayer->GetScript<CPlayerScript>()->SetColSSA(pSwordCol);

	FindLayer(L"PlayerSword")->AddGameObject(pSwordCol);

	//Main Camera
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CToolCamScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck(6, false);

	CToolCamScript* PlayerCamScript = pMainCam->GetScript<CToolCamScript>();
	PlayerCamScript->SetCameraToPlayer(pPlayer);
	FindLayer(L"Default")->AddGameObject(pMainCam);

	// UI Camera
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICam");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);	
	
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);
	pUICam->Camera()->SetFar(100.f);	
	pUICam->Camera()->SetLayerCheck(6, true);	
	
	FindLayer(L"UI")->AddGameObject(pUICam);


	// ===================
	// NPC 로드
	// ===================

	//Ptr<CMeshData> pAS_NPCMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\King_Idle.mdat", L"MeshData\\King_Idle.mdat");

	//CGameObject* pAS_NPC = new CGameObject;

	//pAS_NPC = pAS_NPCMeshData->Instantiate();
	//pAS_NPC->SetName(L"AS_NPC_1");
	//pAS_NPC->FrustumCheck(false);

	//pAS_NPC->Transform()->SetLocalPos(Vec3(300.f, 0.f, 500.f));
	//pAS_NPC->Transform()->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
	//pAS_NPC->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pAS_NPC->AddComponent(new CCollider2D);
	//pAS_NPC->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	//pAS_NPC->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pAS_NPC->Collider2D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
	//pAS_NPC->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	////pAS_NPC->MeshRender()->SetDynamicShadow(true);

	//pAS_NPC->AddComponent(new CBossScript);
	//CBossScript* BossScript = pAS_NPC->GetScript<CBossScript>();
	//pAS_NPC->GetScript<CBossScript>()->init();

	////// 보스 애니메이션
	//pAS_NPC->GetScript<CBossScript>()->SetBossAnimationData(pAS_NPCMeshData->GetMesh(), 0, 0, 55);

	//pAS_NPCMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\King_Attack.mdat", L"MeshData\\King_Attack.mdat");
	//pAS_NPC->GetScript<CBossScript>()->SetBossAnimationData(pAS_NPCMeshData->GetMesh(), 1, 0, 68);

	////pAS_NPCMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\King_Die.mdat", L"MeshData\\King_Die.mdat");
	////pAS_NPC->GetScript<CBossScript>()->SetBossAnimationData(pAS_NPCMeshData->GetMesh(), 2, 0, 78);

	//FindLayer(L"NPC")->AddGameObject(pAS_NPC);

	//CGameObject* pbSword = new CGameObject;

	//Ptr<CMeshData> pbSwordMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Weapon.mdat", L"MeshData\\Monster_FM_Weapon.mdat");

	//pbSword = pbSwordMeshData->Instantiate();
	//pbSword->SetName(L"King_Sword");
	//pbSword->FrustumCheck(false);
	//pbSword->Transform()->SetLocalScale(Vec3(1.7f, 1.7f, 1.7f));

	//pbSword->AddComponent(new CSwordScript);
	//pbSword->GetScript<CSwordScript>()->init(PERSON_OBJ_TYPE::BOSS, pAS_NPC, 25);
	//pAS_NPC->AddChild(pbSword);

	/*Ptr<CMeshData> pTestMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_Lv1_Idle.mdat", L"MeshData\\Monster_Lv1_Idle.mdat");

	CGameObject* ptest = new CGameObject;

	ptest = pTestMeshData->Instantiate();
	ptest->SetName(L"mtest");
	ptest->FrustumCheck(false);

	ptest->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	ptest->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	ptest->Transform()->SetLocalRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));

	ptest->AddComponent(new CMonsterScript);

	CMonsterScript* MonsterScript = ptest->GetScript<CMonsterScript>();

	MonsterScript->SetMonsterAnimationData(pTestMeshData->GetMesh(), 0, 0, 55);			

	pTestMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_Lv1_Run.mdat", L"MeshData\\Monster_Lv1_Run.mdat");
	MonsterScript->SetMonsterAnimationData(pTestMeshData->GetMesh(), 1, 0, 21);			

	pTestMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_Lv1_Hit.mdat", L"MeshData\\Monster_Lv1_Hit.mdat");
	MonsterScript->SetMonsterAnimationData(pTestMeshData->GetMesh(), 2, 0, 29);		

	pTestMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_Lv1_Attack.mdat", L"MeshData\\Monster_Lv1_Attack.mdat");
	MonsterScript->SetMonsterAnimationData(pTestMeshData->GetMesh(), 3, 0, 45);			

	FindLayer(L"NPC")->AddGameObject(ptest);*/	

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"PostEffect");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Material 값 셋팅
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl");
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl, 0);
	pObject->Transform()->SetLocalScale(Vec3(300.f, 700.f, 300.f));
	//pObject->Transform()->SetLocalRot(Vec3(0.0f, -XM_PI, 0.0f));
	pObject->Transform()->SetLocalPos(Vec3(0, 10, 1700));
	FindLayer(L"Portal")->AddGameObject(pObject);

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"PlayerCollider", L"Map");
}