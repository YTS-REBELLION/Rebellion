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

#include "LoginScene.h"

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

void CSceneMgr::CreateMap()
{
	CGameObject* pTileObjects = nullptr;
	Ptr<CTexture> pBirckColor = CResMgr::GetInst()->Load<CTexture>(L"Brick", L"Texture\\Brick.png");
	Ptr<CTexture> pWallColor = CResMgr::GetInst()->Load<CTexture>(L"Wall_tex", L"Texture\\wall.jpg");

	for (int j = 0; j < 13; ++j)
	{
		for (int i = 0; i < 15; ++i)
		{
			pTileObjects = new CGameObject;
			pTileObjects->SetName(L"Tile");
			pTileObjects->AddComponent(new CTransform);
			pTileObjects->AddComponent(new CMeshRender);

			// Transform 설정
			pTileObjects->Transform()->SetLocalPos(Vec3(-7000.f + i * 1000.f, 5000.f, 500.f + j * 1000.f));
			pTileObjects->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
			pTileObjects->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

			// MeshRender 설정
			pTileObjects->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pTileObjects->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMtrl"));
			pTileObjects->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBirckColor.GetPointer());

			// AddGameObject
			m_pCurScene->FindLayer(L"Map")->AddGameObject(pTileObjects);
		}
	}

	for (int j = 0; j < 13; ++j)
	{
		for (int i = 0; i < 15; ++i)
		{
			pTileObjects = new CGameObject;
			pTileObjects->SetName(L"Tile");
			pTileObjects->AddComponent(new CTransform);
			pTileObjects->AddComponent(new CMeshRender);

			// Transform 설정
			pTileObjects->Transform()->SetLocalPos(Vec3(-7000.f + i * 1000.f, 5490.f, 500.f + j * 1000.f));
			pTileObjects->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
			pTileObjects->Transform()->SetLocalRot(Vec3(1.5 * XM_PI, 0.f, 0.f));

			// MeshRender 설정
			pTileObjects->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pTileObjects->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMtrl"));
			pTileObjects->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBirckColor.GetPointer());

			// AddGameObject
			m_pCurScene->FindLayer(L"Map")->AddGameObject(pTileObjects);
		}
	}

	CGameObject* pCastleObject = nullptr;
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MAP10.mdat", L"MeshData\\MAP10.mdat");

	pCastleObject = pMeshData->Instantiate();
	pCastleObject->SetName(L"Castle");
	pCastleObject->FrustumCheck(false);
	pCastleObject->Transform()->SetLocalPos(Vec3(0.f, 5110.f, 0.f));
	pCastleObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pCastleObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));

	pCastleObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pWallColor.GetPointer());

	m_pCurScene->FindLayer(L"Map")->AddGameObject(pCastleObject);


	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_01.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_01_N.tga");

	CGameObject* pObject = nullptr;
	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(870.f, 5110.f, 1000.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 2000.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);




	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(380.f, 5110.f, 2500.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 800.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(600.f, 5110.f, 3250.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 800.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(370.f, 5110.f, 3800.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(295.f, 5110.f, 22.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(2000.f, 500.f, 300.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-925.f, 5110.f, 1200.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 2000.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);




	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-380.f, 5110.f, 2500.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 800.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);





	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-600.f, 5110.f, 3250.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 800.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-370.f, 5110.f, 3800.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);



	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(720.f, 5110.f, 4080.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 500.f, 100.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-720.f, 5110.f, 4080.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 500.f, 100.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);



	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(1800.f, 5110.f, 5220.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-1800.f, 5110.f, 5220.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(1800.f, 5110.f, 6520.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-1800.f, 5110.f, 6520.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 500.f, 600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);



	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(675.f, 5110.f, 7655.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 500.f, 100.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-675.f, 5110.f, 7655.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 500.f, 100.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);


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
	Ptr<CTexture> pSky04 = CResMgr::GetInst()->Load<CTexture>(L"Sky04", L"Texture\\Skybox\\Sky08.png");

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
	m_pCurScene->GetLayer(6)->SetName(L"Object"); 
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
	CreateMap();
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

	Ptr<CTexture> pPlayerColor = CResMgr::GetInst()->Load<CTexture>(L"Player_d", L"Texture\\Player\\Dreyar_diffuse.png");
	Ptr<CTexture> pPlayerNormal = CResMgr::GetInst()->Load<CTexture>(L"Player_n", L"Texture\\Player\\Dreyar_normal.png");

	pPlayer->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerColor.GetPointer());
	pPlayer->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pPlayerNormal.GetPointer());

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

	// Monster 객체 생성
	CGameObject* pMonster = new CGameObject;
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Sword And Shield Idle (2).mdat", L"MeshData\\Sword And Shield Idle (2).mdat");
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Sword And Shield Idle (2).fbx");
	//pMeshData->Save(pMeshData->GetPath());

	pMonster = pMeshData->Instantiate();
	pMonster->SetName(L"Monster1");
	pMonster->FrustumCheck(false);
	pMonster->Transform()->SetLocalPos(Vec3(0.f, 5000.f, 3200.f));
	pMonster->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	pMonster->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pMonster->AddComponent(new CCollider2D);
	pMonster->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pMonster->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 70.f));
	pMonster->Collider2D()->SetOffsetScale(Vec3(20.f, 20.f, 40.f));

	// 플레이어 스크립트 붙여주기.
	pMonster->AddComponent(new CMonsterScript);

	CMonsterScript* MonsterScript = pMonster->GetScript<CMonsterScript>();
	// 플레이어 애니메이션
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 0
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Player_Walk.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Big Hit To Head", L"MeshData\\Big Hit To Head.mdat");
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 1
	g_net.SetAniData(pMeshData->GetMesh());

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Shoulder Hit And Fall.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Shoulder Hit And Fall", L"MeshData\\Shoulder Hit And Fall.mdat");
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 2
	g_net.SetAniData(pMeshData->GetMesh());


	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\FMMonster_Walk.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FMMonster_Walk.mat", L"MeshData\\FMMonster_Walk.mdat");
	MonsterScript->GetPlayerAnimation(pMeshData->GetMesh());							// AniData Index 3
	g_net.SetAniData(pMeshData->GetMesh());

	m_pCurScene->AddGameObject(L"Monster", pMonster, false);


	//// Tree 오브젝트
	//
	////Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Player_Idle.mdat", L"MeshData\\Player_Idle.mdat");
	//Ptr<CMeshData> pTreeMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\jul2.fbx");
	////pMeshData->Save(pMeshData->GetPath());

	//CGameObject* pTree = new CGameObject;

	//pTree = pTreeMeshData->Instantiate();
	//pTree->SetName(L"Tree");
	//pTree->FrustumCheck(false);
	//pTree->Transform()->SetLocalPos(Vec3(-2500.f, 340.f, 2570.f));
	//pTree->Transform()->SetLocalScale(Vec3(40.f, 20.f, 63.f));
	//pTree->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTree->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(pTree);

	//// ====================
	//// Potal 오브젝트 생성
	//// ====================
	CGameObject* pPotalObject = nullptr;
	Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\DragonJawPortal.mdat", L"MeshData\\DragonJawPortal.mdat");
	//Ptr<CMeshData> pPMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DragonJawPortal.fbx");

	pPotalObject = pPMeshData->Instantiate();
	pPotalObject->SetName(L"Portal");
	pPotalObject->FrustumCheck(false);
	pPotalObject->Transform()->SetLocalPos(Vec3(114.f, 150.f, 3300.f));
	pPotalObject->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	pPotalObject->Transform()->SetLocalRot(Vec3(17.13f, 21.95f, 0.f));

	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(114.f, 150.f, 3450.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(300.f, 300.f, 300.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pPotalObject);

	// ====================
	// Skybox 오브젝트 생성
	// ====================
	//pObject = new CGameObject;
	//pObject->SetName(L"SkyBox");
	//pObject->FrustumCheck(false);
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);	

	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky04.GetPointer());

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject, false);

	g_net.SetObj(pPlayer);
	 
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
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Monster", L"Sword");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Object", L"Player");



	// obstacle 오브젝트 생성
	// ==========================================================================================
	CGameObject* GuardHouseObject = nullptr;

	//GuardHouseObject = new CGameObject;
	// pPMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");
	//GuardHouseObject = pPMeshData->Instantiate();
	//GuardHouseObject->SetName(L"Guard_houses");
	//GuardHouseObject->FrustumCheck(false);


	//// Transform 설정

	//GuardHouseObject->Transform()->SetLocalPos(Vec3(1000.f, 360.f, 2000.f));
	//GuardHouseObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));
	//GuardHouseObject->Transform()->SetLocalRot(Vec3(17.26f, 0.f, 0.f));

	//// 더미
	//pObject = new CGameObject;
	//pObject->SetName(L"Map Object");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//// Transform 설정
	//pObject->Transform()->SetLocalPos(Vec3(1000.f, 360.f, 2000.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->AddComponent(new CCollider2D);
	//pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->Collider2D()->SetOffsetScale(Vec3(1000.f, 2200.f, 1000.f));
	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);
	//// AddGameObject
	//m_pCurScene->FindLayer(L"Monster")->AddGameObject(GuardHouseObject);
	//
	// ===========================================================================================
	CGameObject* GateHouseObject = nullptr;
	GateHouseObject = new CGameObject;
	pPMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");
	GateHouseObject = pPMeshData->Instantiate();
	GateHouseObject->SetName(L"Gate_houses");
	GateHouseObject->FrustumCheck(false);

	// Transform 설정
	GateHouseObject->Transform()->SetLocalPos(Vec3(-1400.f, 200.f, 0.f));
	GateHouseObject->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));
	GateHouseObject->Transform()->SetLocalRot(Vec3(17.26f, 0.f, 0.f));

	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-1400.f, 200.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 1000.f, 1600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(GateHouseObject);

	// ===========================================================================================
	CGameObject* GateHouseObject2 = nullptr;
	GateHouseObject2 = new CGameObject;
	pPMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");
	GateHouseObject2 = pPMeshData->Instantiate();
	GateHouseObject2->SetName(L"Gate_houses2");
	GateHouseObject2->FrustumCheck(false);
	//SwordObject->AddComponent(new CTransform);
	//SwordObject->AddComponent(new CMeshRender);

	// Transform 설정
	GateHouseObject2->Transform()->SetLocalPos(Vec3(1000.f, 200.f, 0.f));
	GateHouseObject2->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));

	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(1000.f, 200.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 1000.f, 1600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

	//Script 설정
	GateHouseObject2->AddComponent(new CGateHouse);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Monster", GateHouseObject2, false);

	// ===========================================================================================
	CGameObject* GateHouseObject3 = nullptr;
	GateHouseObject3 = new CGameObject;
	pPMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Gatehouses.mdat", L"MeshData\\Gatehouses.mdat");
	GateHouseObject3 = pPMeshData->Instantiate();
	GateHouseObject3->SetName(L"Gate_houses3");
	GateHouseObject3->FrustumCheck(false);
	//SwordObject->AddComponent(new CTransform);
	//SwordObject->AddComponent(new CMeshRender);

	// Transform 설정
	GateHouseObject3->Transform()->SetLocalPos(Vec3(-1400.f, 200.f, 2000.f));
	GateHouseObject3->Transform()->SetLocalScale(Vec3(0.7f, 0.7f, 0.7f));


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-1400.f, 200.f, 2000.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(600.f, 1000.f, 1600.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

	//Script 설정
	GateHouseObject3->AddComponent(new CGateHouse);
	// AddGameObject
	m_pCurScene->AddGameObject(L"Monster", GateHouseObject3, false);

	
	// 성
	//CGameObject* Castle = nullptr;
	//Castle = new CGameObject;

	////pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Castle.mdat", L"MeshData\\Castle.mdat");
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\65.fbx");
	////pMeshData->Save(pMeshData->GetPath());

	//Castle = pMeshData->Instantiate();
	//Castle->SetName(L"Castle");
	//Castle->FrustumCheck(false);

	//// Transform 설정
	//Castle->Transform()->SetLocalPos(Vec3(0.f, 200.f, 1200.f));
	//Castle->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	////Castle->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	////Script 설정
	//Castle->AddComponent(new CCastle);
	//// AddGameObject
	//m_pCurScene->AddGameObject(L"Default", Castle, false);


	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-94.f, 200.f, 3550.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(5000.f, 600.f, 200.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-94.f, 200.f, -1455.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(5000.f, 600.f, 200.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);



	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(-2465.f, 200.f, 774.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(200.f, 600.f, 5000.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);



	// 더미
	pObject = new CGameObject;
	pObject->SetName(L"Map Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(2465.f, 200.f, 774.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CCollider2D);
	pObject->Collider2D()->SetColliderType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec3(200.f, 600.f, 5000.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject);

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

