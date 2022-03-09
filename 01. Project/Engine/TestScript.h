#include "Script.h"

class CTestScript :
	public CScript
{
private:
	int		m_iLifeCount;

public:	
	virtual void update();
public:
	CLONE(CTestScript);

public:
	CTestScript();
	virtual ~CTestScript();
};

