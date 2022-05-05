
#pragma once
#include "Script.h"

class CSomethings :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CSomethings);

public:
	CSomethings();
	virtual ~CSomethings();

};

