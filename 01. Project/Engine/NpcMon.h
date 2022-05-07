
#include "Script.h"

class CNpcMon :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CNpcMon);

public:
	CNpcMon();
	virtual ~CNpcMon();

};

