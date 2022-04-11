#pragma once
#include "Script.h"

class CToolCamScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;
	CGameObject* m_pPlayer;

public:	
	virtual void update();
	void SetCameraToPlayer(CGameObject* Player) {
		m_pPlayer = Player;
	}

public:
	CLONE(CToolCamScript);

public:
	CToolCamScript();
	virtual ~CToolCamScript();
};

