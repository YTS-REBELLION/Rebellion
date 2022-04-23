#pragma once
#include "Script.h"

class CSword :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CSword);

public:
	CSword();
	virtual ~CSword();


};

