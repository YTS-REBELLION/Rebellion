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

#include "MonsterColScript.h"

void CAssemblyAreaScene::CreateMap()
{
	//Ptr<CMeshData> pMapMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Dungeon_Map.mdat", L"MeshData\\Dungeon_Map.mdat");

	//CGameObject* Map = new CGameObject;

	//Map->SetName(L"Map_Object");
	//Map = pMapMeshData->Instantiate();
	//Map->FrustumCheck(false);

	//// Transform 설정
	//Map->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//Map->Transform()->SetLocalScale(Vec3(2.3f, 2.3f, 2.0f));
	//Map->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.0f), XMConvertToRadians(180.0f), 0.f));

	//FindLayer(L"Map")->AddGameObject(Map);


	//// 맵 콜라이더 만들기

	//// 1
	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(0.f, 50.f, -150.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(500.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 2
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(250.f, 50.f, 1300.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(50.f, 100.f, 2800.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 3
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-250.f, 50.f, 1300.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(50.f, 100.f, 2800.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 4
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(600.f, 50.f, 2750.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(1000.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 5
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-600.f, 50.f, 2750.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(1000.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 6
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(1150.f, 50.f, 3600.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(50.f, 100.f, 1600.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 7
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(0.f, 50.f, 4400.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(2100.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 8
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-1150.f, 50.f, 4100.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 800.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 9
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-1150.f, 50.f, 3080.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 800.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 10
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-2580.f, 50.f, 3300.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(2800.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 11
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-2580.f, 50.f, 3870.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(2800.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 12
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-6550.f, 50.f, 3600.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(50.f, 100.f, 1600.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 13
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-5300.f, 50.f, 2750.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(2500.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 14
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-4000.f, 50.f, 4100.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 800.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 15
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-4000.f, 50.f, 3080.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 800.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 16
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-4700.f, 50.f, 4450.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(1300.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 17 박스
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-6250.f, 50.f, 4150.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(450.f, 100.f, 450.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// 18
	//pObject = new CGameObject;
	//pObject->SetName(L"Map_Wall");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->Transform()->SetLocalPos(Vec3(-6050.f, 50.f, 4450.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(1025.f, 100.f, 50.f));
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//FindLayer(L"Map")->AddGameObject(pObject);

	//// ====================
	//// 바닥 오브젝트 생성
	//// ====================
	//Ptr<CMeshData> pTestMapMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Ground.mdat", L"MeshData\\Ground.mdat");

	//CGameObject* Tile = new CGameObject;

	//Tile->SetName(L"Map_Tile");
	//Tile = pTestMapMeshData->Instantiate();
	//Tile->FrustumCheck(false);
	//
	//Tile->Transform()->SetLocalPos(Vec3(-2500.f, -50.f, -2500.f));
	//Tile->Transform()->SetLocalScale(Vec3(3.f, 3.f, 1.f));
	//Tile->Transform()->SetLocalRot(Vec3(XMConvertToRadians(90.0f), 0.f, 0.f));

	//Tile->MeshRender()->SetDynamicShadow(true);
	//FindLayer(L"Map")->AddGameObject(Tile);

	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_01.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_01_N.tga");
	// ==================
	// Map 오브젝트 생성
	// ==================
	
	for (int j = 0; j < 5; ++j)
	{
		for (int i = 0; i < 5; ++i)
		{
			CGameObject* pObject = new CGameObject;
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
	GetLayer(2)->SetName(L"Sword");
	GetLayer(3)->SetName(L"House");
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
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	pPlayer->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
	pPlayer->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));

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
	//CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Sword", pSword, false);
	//FindLayer(L"Sword")->AddGameObject(pSword);

	// ===================
	// Sword 파일 로드
	// ===================
	//CGameObject* pSword = new CGameObject;

	//Ptr<CMeshData> pSwordMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Sword.mdat", L"MeshData\\Player_Sword.mdat");
	//Ptr<CTexture> pSwordTex = CResMgr::GetInst()->Load<CTexture>(L"Sword", L"Texture\\Player\\Ax.png");

	//pSword = pSwordMeshData->Instantiate();
	//pSword->SetName(L"Player_Sword");
	//pSword->FrustumCheck(false);
	//pSword->Transform()->SetLocalPos(Vec3(0.f, 100.f, 0.f));
	//pSword->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	//pSword->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSwordTex.GetPointer());

	////pSword->AddComponent(new CSwordScript);
	////CSwordScript* SwordScript = pSword->GetScript<CSwordScript>();
	////pSword->GetScript<CSwordScript>()->init(PERSON_OBJ_TYPE::WARRIOR_PLAYER, pPlayer, 25);

	//FindLayer(L"Sword")->AddGameObject(pSword);
	//pPlayer->AddChild(pSword);

	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TransparencyMtrl");
	//Ptr<CTexture> pTransparency = CResMgr::GetInst()->FindRes<CTexture>(L"Transparency");

	//CGameObject* pSwordCol = new CGameObject;
	//pSwordCol->SetName(L"SwordCol");
	//pSwordCol->AddComponent(new CCollider2D);
	//pSwordCol->AddComponent(new CTransform);
	//pSwordCol->AddComponent(new CMeshRender);
	//pSwordCol->Transform()->SetLocalPos(pSword->Transform()->GetLocalPos());
	//pSwordCol->Transform()->SetLocalScale(pSword->Transform()->GetLocalScale());
	//pSwordCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pSwordCol->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	////pSwordCol->MeshRender()->SetMaterial(pMtrl);
 	//pSwordCol->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	//pSwordCol->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	//pSwordCol->Collider2D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));      // 80.f, 200.f, 80.f ?????
	//pSwordCol->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pSwordCol->AddComponent(new CSwordAttackAreaScript);
	//pSwordCol->GetScript<CSwordAttackAreaScript>()->SetPlayer(pPlayer);

	//pColObj->GetScript<CPlayerColScript>()->SetPlayer(pObject);
	//pObject->GetScript<CPlayerScript>()->SetColPlayer(pColObj);

	//FindLayer(L"Sword")->AddGameObject(pSwordCol, false);

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

	//CGameObject* pObject = nullptr;

	//Ptr<CTexture> pfFire01 = CResMgr::GetInst()->Load<CTexture>(L"Fire01", L"Texture\\Explosion\\fire01.dds");
	//Ptr<CTexture> pfNoise01 = CResMgr::GetInst()->Load<CTexture>(L"Noise01", L"Texture\\Explosion\\noise01.dds");
	//Ptr<CTexture> pfAlpha01 = CResMgr::GetInst()->Load<CTexture>(L"Alpha01", L"Texture\\Explosion\\alpha01.dds");
	//// ====================
	//// Fire 오브젝트 생성
	//// ====================

	//pObject = new CGameObject;
	//pObject->SetName(L"FireTest");
	//pObject->FrustumCheck(false);
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CFire);


	//pObject->Transform()->SetLocalPos(Vec3(200, 200, 200));
	//pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FireMtrl"));
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pfFire01.GetPointer());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pfNoise01.GetPointer());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, pfAlpha01.GetPointer());
	//pObject->GetScript<CFire>()->init();
	//// AddGameObject
	//FindLayer(L"Default")->AddGameObject(pObject);


		

	// ===================
	// NPC 로드
	// ===================

	//Ptr<CMeshData> pAS_NPCMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Idle (8).fbx");
	//pAS_NPCMeshData->Save(pAS_NPCMeshData->GetPath());
	Ptr<CMeshData> pAS_NPCMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FM_Idle.mdat", L"MeshData\\FM_Idle.mdat");

	CGameObject* pAS_NPC = new CGameObject;

	pAS_NPC = pAS_NPCMeshData->Instantiate();
	pAS_NPC->SetName(L"AS_NPC_1");
	pAS_NPC->FrustumCheck(false);

	pAS_NPC->Transform()->SetLocalPos(Vec3(300.f, 100.f, 1000.f));
	pAS_NPC->Transform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	pAS_NPC->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pAS_NPC->AddComponent(new CCollider2D);
	pAS_NPC->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	pAS_NPC->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pAS_NPC->Collider2D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
	pAS_NPC->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	//pAS_NPC->MeshRender()->SetDynamicShadow(true);
	FindLayer(L"NPC")->AddGameObject(pAS_NPC);

	//// 검 테스트

	////Ptr<CMeshData> ptestMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Weapon\\Monster_FM_Weapon.fbx");
	////ptestMeshData->Save(ptestMeshData->GetPath());
	//Ptr<CMeshData> ptestMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Weapon.mdat", L"MeshData\\Monster_FM_Weapon.mdat");

	//CGameObject* ptest = new CGameObject;

	//ptest = ptestMeshData->Instantiate();
	//ptest->SetName(L"Weapon");
	//ptest->FrustumCheck(false);

	//ptest->Transform()->SetLocalPos(Vec3(300.f, 0.f, 300.f));
	//ptest->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//ptest->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));

	//FindLayer(L"NPC")->AddGameObject(ptest);

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

	//Vec3 vScale(150.f, 150.f, 1.f);

	//Ptr<CTexture> arrTex[5] = { CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex")
	//	, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex")
	//	, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex")
	//	, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseLightTargetTex")
	//	, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularLightTargetTex") };

	//for (UINT i = 0; i < 5; ++i)
	//{
	//	CGameObject* pObject = new CGameObject;
	//	pObject->SetName(L"UI Object");
	//	pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
	//	pObject->AddComponent(new CTransform);
	//	pObject->AddComponent(new CMeshRender);

	//	// Transform 설정
	//	tResolution res = CRenderMgr::GetInst()->GetResolution();

	//	pObject->Transform()->SetLocalPos(Vec3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x)
	//		, (res.fHeight / 2.f) - (vScale.y / 2.f)
	//		, 1.f));

	//	pObject->Transform()->SetLocalScale(vScale);

	//	// MeshRender 설정
	//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	//	pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, arrTex[i].GetPointer());

	//	// AddGameObject
	//	FindLayer(L"UI")->AddGameObject(pObject);
	//}

	//CGameObject* pMonster = new CGameObject;
	//Ptr<CMeshData> pMonsterMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Idle.mdat", L"MeshData\\Monster_FM_Idle.mdat");

	//pMonster = pMonsterMeshData->Instantiate();
	//pMonster->SetName(L"FM_Monster");
	//pMonster->FrustumCheck(false);

	//pMonster->Transform()->SetLocalPos(Vec3(300.f, 0.f, 0.f));
	//pMonster->Transform()->SetLocalScale(Vec3(4.5f, 4.5f, 4.5f));
	//pMonster->Transform()->SetLocalRot(Vec3(XMConvertToRadians(180.f), 0.f, 0.f));

	//// 몬스터 스크립트 붙여주기.
	//pMonster->AddComponent(new CMonsterScript);

	//CMonsterScript* MonsterScript = pMonster->GetScript<CMonsterScript>();

	////몬스터 애니메이션
	//MonsterScript->SetMonsterAnimationData(pMonsterMeshData->GetMesh(), 0, 0, 44);

	//pMonsterMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Run.mdat", L"MeshData\\Monster_FM_Run.mdat");
	//MonsterScript->SetMonsterAnimationData(pMonsterMeshData->GetMesh(), 1, 0, 21);

	//pMonsterMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Hit.mdat", L"MeshData\\Monster_FM_Hit.mdat");
	//MonsterScript->SetMonsterAnimationData(pMonsterMeshData->GetMesh(), 2, 0, 21);

	//pMonsterMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Attack.mdat", L"MeshData\\Monster_FM_Attack.mdat");
	//MonsterScript->SetMonsterAnimationData(pMonsterMeshData->GetMesh(), 3, 0, 50);

	//pMonsterMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_FM_Die.mdat", L"MeshData\\Monster_FM_Die.mdat");
	//MonsterScript->SetMonsterAnimationData(pMonsterMeshData->GetMesh(), 4, 0, 68);

	//FindLayer(L"NPC")->AddGameObject(pMonster);

	//CGameObject* pMonsterCol = new CGameObject;
	//pMonsterCol->SetName(L"MonsterCol");
	//pMonsterCol->AddComponent(new CCollider2D);
	//pMonsterCol->AddComponent(new CTransform);
	//pMonsterCol->AddComponent(new CMeshRender);
	//pMonsterCol->Transform()->SetLocalPos(pMonster->Transform()->GetLocalPos());
	//pMonsterCol->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pMonsterCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMonsterCol->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//
	//pMonsterCol->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
	//pMonsterCol->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	//pMonsterCol->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pMonsterCol->AddComponent(new CMonsterColScript);
	//pMonsterCol->GetScript<CMonsterColScript>()->SetMonster(pMonster);
	////GetObj()->GetScript<CPlayerScript>()->SetColPlayer(pSwordCol);

	//CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"NPC", pMonsterCol, false);

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"NPC");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Sword", L"NPC");

}