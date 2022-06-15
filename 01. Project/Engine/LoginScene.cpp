#include "stdafx.h"
#include "LoginScene.h"
#include "Layer.h"
#include "Transform.h"
#include "Camera.h"
#include "ResMgr.h"
#include "RenderMgr.h"
#include "KeyMgr.h"

void CLoginScene::Init()
{
	ShowCursor(true);

	GetLayer(0)->SetName(L"Default");

	CGameObject* pMainCam = nullptr;

	// Camera Object
	pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);

	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pMainCam->Camera()->SetFar(100.f);
	pMainCam->Camera()->SetLayerCheck(0, true);
	/*pMainCam->Camera()->SetWidth(CRenderMgr::GetInst()->GetResolution().fWidth);
	pMainCam->Camera()->SetHeight(CRenderMgr::GetInst()->GetResolution().fHeight);*/

	FindLayer(L"Default")->AddGameObject(pMainCam);

	awake();
	start();
}

void CLoginScene::update()
{
	if (KEY_HOLD(KEY_TYPE::KEY_ENTER))
	{

	}
}
