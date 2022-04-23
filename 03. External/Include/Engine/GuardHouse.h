#pragma once
#include "Script.h"

class CGuardHouse :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CGuardHouse);

public:
	CGuardHouse();
	virtual ~CGuardHouse();

};

