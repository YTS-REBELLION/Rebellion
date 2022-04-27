#pragma once
#include "Script.h"
#include "Mesh.h"

class CPlayerScript :
	public CScript
{
public:
	vector<Ptr<CMesh>>	m_pAniData;

private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	
public:
	virtual void awake();	
	virtual void update();

	void GetPlayerAnimation(Ptr<CMesh> AniDate) { m_pAniData.push_back(AniDate); };
	void SetPlayerAnimation(const int i);

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();
};

