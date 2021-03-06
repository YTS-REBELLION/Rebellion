#pragma once

class CScene;
class CCamera;
class CGameObject;


class CSceneMgr
{
	SINGLE(CSceneMgr);
private:
	CScene*		m_pCurScene;	

public:
	void init();

	void update();	 // Player Mod ?϶?
	void update_tool(); 

public:
	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void FindGameObjectByTag( const wstring& _strTag, vector<CGameObject*>& _vecFindObj);
	//void FindGameObjectByPoint(POINT _point, vector<CGameObject*>& _vecFindObj, CCamera* _pToolCam = nullptr);
	void FindPlayerPos(const wstring& _strTag);
	
	Vec3 m_vSavePos;
	Vec3 m_vSaveRot;
	bool m_bfindcheck = false;
	vector<CGameObject*> testvec;


public:
	void CreateTargetUI();
};

