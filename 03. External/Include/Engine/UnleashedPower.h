

#pragma once
#include "Script.h"

class CUnleashedPower :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CUnleashedPower);

public:
	CUnleashedPower();
	virtual ~CUnleashedPower();
public:
	Vec3 vPos = { 0.f,0.f,0.f };
	float m_fcreate_time = 0.f;

};
