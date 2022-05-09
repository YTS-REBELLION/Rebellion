#pragma once
#include "Script.h"



class CCastle :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CCastle);

public:
	CCastle();
	virtual ~CCastle();

};


