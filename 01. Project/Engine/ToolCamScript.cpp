#include "stdafx.h"
#include "ToolCamScript.h"

#include "Camera.h"

#include "RenderMgr.h"
#include"PlayerScript.h"


CToolCamScript::CToolCamScript()
	: CScript(0)
	, m_fSpeed(200.f)
	, m_fScaleSpeed(1.f)
{

	m_pPlayer = nullptr;
	m_VecObj.clear();
}

CToolCamScript::~CToolCamScript()
{
}

void CToolCamScript::update()
{
	CameraMode();

	Vec3 vPos = Transform()->GetLocalPos();
	
	Mouse_Move();
	if (true == m_bMouseFix)
	{
		Fix_Mouse();
				
	}

	


	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
	{
		fSpeed *= 5.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vFront * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vBack * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	{
		Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vLeft * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	{
		Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vRight * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_1))
	{
		fScale -= m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);



	}
	if (KEY_AWAY(KEY_TYPE::KEY_1))
	{
		//집결지앞의 충돌체와 충돌시 시네마틱으로
		m_CameraMode = CENEMATIC_0;
		m_bCine1 = true;

	}
	if (KEY_HOLD(KEY_TYPE::KEY_2))
	{
		fScale += m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);
	}

	
		
		//Vec3 temp = CSceneMgr::GetInst()->m_vSaveRot;
		////Vec2 vDrag = GetDragDir(); //CKeyMgr::GetInst()->GetDragDir();
		//Vec3 vRot = Transform()->GetLocalRot();
		//vRot = temp;
		//Transform()->SetLocalRot(vRot);
		////cout << "dirx:" << vRot.x << "diry:" << vRot.y << endl;
		//cout << "vRot:" << vRot.x <<","<< vRot.y<< endl;
	
	
}


void CToolCamScript::CameraMode()
{
	switch (m_CameraMode)
	{
	case PLAYER_IDLE:
		Player_Mode();
		break;
	case CENEMATIC_0:
		Cenematic0_Mode();
		break;

	default:
		break;
	}
}

void CToolCamScript::Player_Mode()
{
	Vec3 vtemp = { 0.f,0.f,0.f };
	Transform()->SetLocalPos(vtemp);

	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;
	
	Vec3 vRot = Transform()->GetLocalRot();
	Vec3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3 vLook = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vBackLook = -vLook;
	Vec3 vRightLook = vLook + vRight;
	Vec3 vLeftLook = vLook - vRight;
	vRight = Vector3::Normalize(vRight);
	vBackLook = Vector3::Normalize(vBackLook);
	vRightLook = Vector3::Normalize(vRightLook);
	vLeftLook = Vector3::Normalize(vLeftLook);
	vLook = Vector3::Normalize(vLook);


	
		CSceneMgr::GetInst()->FindPlayerPos(L"Player Object");
		if (CSceneMgr::GetInst()->m_bfindcheck)
		{
			
			
			vPos.x = CSceneMgr::GetInst()->m_vSavePos.x;
			vPos.y = CSceneMgr::GetInst()->m_vSavePos.y+200;
			vPos.z = CSceneMgr::GetInst()->m_vSavePos.z-600;
		}
			

		//m_pPlayer= dynamic_cast<CPlayerScript*>(CSceneMgr::GetInst()->testvec.back());
		
		//m_pPlayer = GetObj()->GetScript<CPlayerScript>();
					
	//CSceneMgr::GetInst()->FindGameObjectByTag(L"Player Object", m_VecObj);
	//wstring temp = m_VecObj[0]->GetName();

	//vPos = m_VecObj.front()->Transform()->GetLocalPos();

	
	
	Transform()->SetLocalPos(vPos);

	
	cout << "posX:" << vPos.x << "posY:" << vPos.y << "posZ:" << vPos.z << endl;
}

void CToolCamScript::Cenematic0_Mode()
{

	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	//집결지 시네
	if (m_bCine1 == true && m_bCheckFin1 == false)
	{
		checktime += DT;

		//cout << checktime << endl;
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);
		vPos += vFront * fSpeed * DT;
		vPos += vUp * fSpeed * 2.f * DT;

		if (//집결지끝도착하면 180도회전으로
			checktime >= 1.f)
		{
			m_bCheckFin1 = true;
			checktime = 0.f;

		}


	}
	if (m_bCheckFin1)
	{


		Vec3 vRot = Transform()->GetLocalRot();

		vRot.y += DT * XM_PI * 1 / 5;

		Transform()->SetLocalRot(vRot);


		cout << "RotY:" << vRot.y << endl;


		////한바뀌돌면 원래 위치로
		if (vRot.y >= 6.1f)
		{
			vRot.y = 0;
			m_CameraMode = PLAYER_IDLE;

		}
		
	}

	Transform()->SetLocalPos(vPos);
}
void CToolCamScript::Fix_Mouse()
{
	POINT   ptMouse{ 800 >> 1, 600 >> 1 };

	ClientToScreen(CRenderMgr::GetInst()->GetHwnd(), &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


}
void CToolCamScript::Mouse_Move()
{
	// 마우스 좌표	


	m_ptOldMouse = m_ptMouse;
	GetCursorPos(&m_ptMouse);
	ScreenToClient(CRenderMgr::GetInst()->GetHwnd(), &m_ptMouse);

	m_vDragDir = Vec2((float)(m_ptMouse.x - m_ptOldMouse.x) * 0.05f, (float)(m_ptOldMouse.y - m_ptMouse.y) * 0.05f);


	
	

}
