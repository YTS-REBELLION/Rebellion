


#pragma once
#include "Script.h"

class CSwing4 :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	CGameObject* m_pPlayer;
public:
	virtual void awake();
	virtual void update();
	void SetPlayer(CGameObject* _pObj) { m_pPlayer = _pObj; }
	CGameObject* GetPlayer() { return m_pPlayer; }
public:
	CLONE(CSwing4);

public:
	CSwing4();
	virtual ~CSwing4();
public:
	Vec3 vPos = { 0.f,0.f,0.f };
	float m_fcreate_time = 0.f;
	
	int m_count = 0.f;
};
