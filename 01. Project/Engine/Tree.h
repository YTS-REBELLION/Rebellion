#pragma once
#include "Script.h"

class CTree :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CTree);

public:
	CTree();
	virtual ~CTree();

};


