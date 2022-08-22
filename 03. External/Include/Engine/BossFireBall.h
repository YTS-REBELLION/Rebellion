
#pragma once
#include "Script.h"

class CBossFireBall :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CBossFireBall);

public:
	CBossFireBall();
	virtual ~CBossFireBall();
public:
	Vec3 vPos = { 0.f,0.f,0.f };
	float m_fcreate_time = 0.f;

};