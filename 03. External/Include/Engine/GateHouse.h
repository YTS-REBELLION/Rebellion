
#pragma once
#include "Script.h"

class CGateHouse :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CGateHouse);

public:
	CGateHouse();
	virtual ~CGateHouse();

};


