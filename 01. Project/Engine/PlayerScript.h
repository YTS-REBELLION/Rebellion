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

	Vec3				m_vecPlayerDir;
	float				m_fSpeed = PLAYER_SPEED;
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

public:
	bool				m_bIsActive = false;
	bool GetIsHide() { return m_bIsActive; }
	Vec3 GetPlayerDir() { return m_vecPlayerDir; }
public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
};

