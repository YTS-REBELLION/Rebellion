
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

#include "PlayerColScript.h"
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
	GetLayer(2)->SetName(L"PlayerSword");
	GetLayer(3)->SetName(L"Monster");
	GetLayer(4)->SetName(L"MonsterSword");
	GetLayer(5)->SetName(L"Map");
	GetLayer(6)->SetName(L"UI");
	GetLayer(7)->SetName(L"Player_Skill");
	GetLayer(8)->SetName(L"Monster_Skill");

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
	PlayerScript->MeshRender()->SetDynamicShadow(true);
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
	pPlayerCol->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	pPlayerCol->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pPlayerCol->AddComponent(new CPlayerColScript);
	pPlayerCol->GetScript<CPlayerColScript>()->SetPlayer(pPlayer);
	pPlayer->GetScript<CPlayerScript>()->SetColPlayer(pPlayerCol);

	FindLayer(L"Player")->AddGameObject(pPlayerCol);


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

}