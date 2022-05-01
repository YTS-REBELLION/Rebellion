
#pragma once
#include "Script.h"

class CPortal :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CPortal);

public:
	CPortal();
	virtual ~CPortal();

};


