#pragma once

#include <unordered_map>
#include "GameObject.h"
#include "Network.h"
#include "Sound.h"
typedef unordered_map<int, CGameObject*> OBJ;
typedef OBJ::iterator OBJITER;

extern OBJ GameObject;

extern CNetwork g_net;
extern int g_myid;
extern int targetId;
// State Description
extern D3D12_RASTERIZER_DESC g_arrRSDesc[(UINT)RS_TYPE::END];
extern D3D12_BLEND_DESC g_arrBlendDesc[(UINT)BLEND_TYPE::END];
extern D3D12_DEPTH_STENCIL_DESC g_arrDepthStencilDesc[(UINT)DEPTH_STENCIL_TYPE::END];
extern tGlobalValue g_global;
extern unordered_map<Sound_Type, CSound*> g_SoundList;