
#include "stdafx.h"
#include "BossMap.h"

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
#include "M_MonsterScript.h"
#include "SwordScript.h"
#include "ToolCamScript.h"
#include "Network.h"

#include"Boss.h"

void CBossMap::CreateMap()
{

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Map_Wall");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 800.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));

	// AddGameObject
	FindLayer(L"Map")->AddGameObject(pObject);
}

void CBossMap::init()
{
	cout << "보스맵 이닛" << endl;
	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Player");
	GetLayer(2)->SetName(L"Sword");
	GetLayer(3)->SetName(L"Map");
	GetLayer(4)->SetName(L"Portal");
	GetLayer(5)->SetName(L"UI");
	GetLayer(6)->SetName(L"Monster");
	GetLayer(7)->SetName(L"NPC");
	GetLayer(8)->SetName(L"Player_Skill");
	GetLayer(9)->SetName(L"Monster_Skill");
	GetLayer(10)->SetName(L"Boss");

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

	FindLayer(L"Default")->AddGameObject(pLight);

	// ===================
	// Player 파일 로드
	// ===================

	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FM_Idle.mdat", L"MeshData\\FM_Idle.mdat");


	CGameObject* pPlayer = new CGameObject;


	//pPlayer = g_net.GetObj();
	//pPlayer->GetScript<CPlayerScript>()->SetMain();
	//pPlayer->Transform()->SetLocalPos(Vec3(200.f, 0.f, 200.f));

	pPlayer = pMeshData->Instantiate();
	pPlayer->SetName(L"FM_Player");
	pPlayer->FrustumCheck(false);

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
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICam");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);
	pUICam->Camera()->SetFar(100.f);
	pUICam->Camera()->SetLayerCheck(5, true);

	FindLayer(L"Default")->AddGameObject(pUICam);

	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_01.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_01_N.tga");

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


		//// Monster 파일 로드
		//// ===================
		//CGameObject* pMonster = new CGameObject;
		////pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Monster_M_Idle.fbx");
		////pMeshData->Save(pMeshData->GetPath());
		//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Idle.mdat", L"MeshData\\Monster_M_Idle.mdat");
	
		//pMonster = pMeshData->Instantiate();
		//pMonster->SetName(L"M_Monster");
		//pMonster->FrustumCheck(false);
		//pMonster->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		//pMonster->Transform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
		//pMonster->Transform()->SetLocalRot(Vec3(XMConvertToRadians(90.f),0.f, XMConvertToRadians(-90.f)));
		////pMonster->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		//pMonster->AddComponent(new CCollider2D);
		//pMonster->Collider2D()->SetColliderType(COLLIDER2D_TYPE::SPHERE);
		//pMonster->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
		//pMonster->Collider2D()->SetOffsetScale(Vec3(20.f, 20.f, 20.f));
		//pMonster->Collider2D()->SetOffsetRot(Vec3(0.f, XMConvertToRadians(90.f), XMConvertToRadians(90.f)));
	
		//// 몬스터 스크립트 붙여주기.
		//pMonster->AddComponent(new CBoss);
	
		//CBoss* M_MonsterScript = pMonster->GetScript<CBoss>();
		//pMonster->GetScript<CBoss>()->init();
	
		//////몬스터 애니메이션
		//M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 0, 0, 55);								// AniData Index 0
	
		//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Walk.mdat", L"MeshData\\Monster_M_Walk.mdat");
		//M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 1, 0, 41);								// AniData Index 1
	
		//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Hit.mdat", L"MeshData\\Monster_M_Hit.mdat");
		//M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 2, 0, 53);								// AniData Index 2
	
		//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Monster_M_Attack.mdat", L"MeshData\\Monster_M_Attack.mdat");
		//M_MonsterScript->SetMonsterAnimationData(pMeshData->GetMesh(), 3, 0, 53);								// AniData Index 3
		//
		//FindLayer(L"Monster")->AddGameObject(pMonster);

}