
#pragma once
#include "Script.h"

class CMiro :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CMiro);

public:
	CMiro();
	virtual ~CMiro();

};


