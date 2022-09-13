#pragma once
#include "Entity.h"

class CLayer;
class CGameObject;
class CCamera;

class CScene :
	public CEntity
{
private:
	CLayer*				m_arrLayer[MAX_LAYER];	
	SCENE_TYPE			m_eSceneType;

public:
	virtual void init() {};
	void awake();
	void start();
	virtual void update();
	void lateupdate();
	void finalupdate();	

public:
	void SetLayerName(int _iIdx, const wstring& _strName);
	void AddGameObject(const wstring& _strLayerName, CGameObject* _pObject, bool _bMoveAll);	
	void AddGameObject(int _iLayerIdx, CGameObject* _pObject, bool _bMoveAll);
	CLayer* FindLayer(const wstring& _strLayerName);
	CLayer* GetLayer(int _iIdx) { return m_arrLayer[_iIdx]; };

	void SetType(SCENE_TYPE _eType) { m_eSceneType = _eType; }
	SCENE_TYPE GetType() { return m_eSceneType; }

private:
	void AddLayer(const wstring& _strLayerName, int _iIdx = -1);



public:
	CScene();
	virtual ~CScene();
};

