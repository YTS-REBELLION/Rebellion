#pragma once

class CScene;
class CCamera;
class CGameObject;


class CSceneMgr
{
	SINGLE(CSceneMgr);
private:
	CScene*		m_pCurScene;	
	CScene*		m_arrScene[(UINT)SCENE_TYPE::END];
	CGameObject* m_pObject;

public:
	void init();

	void update();	 // Player Mod �϶�
	void update_tool(); 

public:
	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void ChangeScene(SCENE_TYPE _Type);

	void FindGameObjectByTag( const wstring& _strTag, vector<CGameObject*>& _vecFindObj);

	void FindPlayerPos(const wstring& _strTag);
	
	Vec3 m_vSavePos;
	Vec3 m_vSaveRot;
	bool m_bfindcheck = false;
	vector<CGameObject*> testvec;

public:
	void CreateTargetUI();
	void CreateMap();
};

