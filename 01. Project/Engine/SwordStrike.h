
#pragma once
#include "Script.h"

class CSwordStrike :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

	CGameObject* m_pPlayer;
	float m_fspeed = 1000.f;
public:
	virtual void awake();
	virtual void update();

	void Set_Player(CGameObject* _player) { m_pPlayer = _player; }

public:
	CLONE(CSwordStrike);

public:
	CSwordStrike();
	virtual ~CSwordStrike();
public:
	Vec3 vPos = { 0.f,0.f,0.f };
	float m_fcreate_time = 0.f;

};
