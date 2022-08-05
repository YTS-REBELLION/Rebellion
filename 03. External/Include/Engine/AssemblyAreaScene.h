#pragma once
#include "Scene.h"
#include "Script.h"
#include "Mesh.h"
class CScene;
class CCamera;
class CGameObject;

class CAssemblyAreaScene : 
	public CScene
{
public:
	virtual void init();
	void CreateMap();
	Ptr<CTexture> pfFire01 = CResMgr::GetInst()->Load<CTexture>(L"Fire01", L"Texture\\Explosion\\fire01.dds");
	Ptr<CTexture> pfNoise01 = CResMgr::GetInst()->Load<CTexture>(L"Noise01", L"Texture\\Explosion\\noise01.dds");
	Ptr<CTexture> pfAlpha01 = CResMgr::GetInst()->Load<CTexture>(L"Alpha01", L"Texture\\Explosion\\alpha01.dds");
};