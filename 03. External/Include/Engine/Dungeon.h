#pragma once
#include "Scene.h"

class CScene;
class CCamera;
class CGameObject;

class CDungeonScene:
	public CScene
{
public:
	virtual void init();
	void CreateMap();
};