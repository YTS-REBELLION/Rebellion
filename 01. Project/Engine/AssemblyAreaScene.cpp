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
#include "SwordScript.h"
#include "ToolCamScript.h"


void CAssemblyAreaScene::init()
{
	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Player");
	GetLayer(2)->SetName(L"House");
	GetLayer(3)->SetName(L"House");
	GetLayer(4)->SetName(L"Portal");
}