#pragma once
#include "Scene.h"

class CScene;
class CCamera;
class CGameObject;

class CBossMap :
	public CScene
{
public:
	virtual void init();

	void CreateMap();
};

#pragma once
#include "Scene.h"
