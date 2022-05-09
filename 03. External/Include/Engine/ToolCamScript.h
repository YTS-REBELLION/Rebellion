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
	float			m_fSpeed;
	float			m_fScaleSpeed;
	CGameObject*	m_pPlayer;

public:	
	bool			m_bCine1 = false;
	bool			m_bCheckFin1 = false;
	float			checktime = 0.f;
	float			checktime2 = 0.f;
	bool			m_bMouseFix = false;

	CAMERASTATE		m_eCamState = CAMERASTATE::PLAYER;
public:
	virtual void update();
	void SetCameraToPlayer(CGameObject* Player) {
		m_pPlayer = Player;
	}

	void SetFreeCamera();
	void SetPlayerFixedCamera();

	//void CameraMode();
	//void Player_Mode();
	//void Cenematic0_Mode();
	//void Fix_Mouse();
	//void Mouse_Move();

public:
	CLONE(CToolCamScript);

public:
	CToolCamScript();
	virtual ~CToolCamScript();

public:
	//CPlayerScript* m_pPlayer;
	CameraState m_CameraMode = PLAYER_IDLE;
	vector<CGameObject*> m_VecObj;
	Vec2 GetDragDir() { return m_vDragDir; }

private:
	
	POINT				m_ptMouse;
	POINT				m_ptOldMouse;

	Vec2				m_vDragDir;
	Vec3				m_vMouseRot = { 0.f,0.f,0.f };
};

