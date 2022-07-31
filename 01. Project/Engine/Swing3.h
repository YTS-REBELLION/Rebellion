


#pragma once
#include "Script.h"

class CSwing3 :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CSwing3);

public:
	CSwing3();
	virtual ~CSwing3();
public:
	Vec3 vPos = { 0.f,0.f,0.f };
	float m_fcreate_time = 0.f;
	CGameObject* m_pPlayer;
	int m_count = 0.f;
};
