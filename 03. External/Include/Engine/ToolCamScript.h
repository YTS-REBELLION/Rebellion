#pragma once
#include "Script.h"

class CPlayerScript;

enum CameraState
{
	PLAYER_IDLE,
	CENEMATIC_0,
	CAMERASTATE_END
};

class CToolCamScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;
	bool		m_bCine1 = false;
	bool		m_bCheckFin1 = false;
	float checktime = 0.f;
	bool		m_bMouseFix = false;
public:
	virtual void update();
	void CameraMode();
	void Player_Mode();
	void Cenematic0_Mode();
	void Fix_Mouse();
	void Mouse_Move();
public:
	CLONE(CToolCamScript);

public:
	CToolCamScript();
	virtual ~CToolCamScript();

public:
	CPlayerScript* m_pPlayer;
	CameraState m_CameraMode = PLAYER_IDLE;
	vector<CGameObject*> m_VecObj;
};

