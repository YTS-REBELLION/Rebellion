#include "stdafx.h"
#include "Fire.h"
#include "TimeMgr.h"
#include"PlayerScript.h"
#include"Camera.h"
#include"ToolCamScript.h"


void CFire::init()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_NoiseBuffer.frameTime);
	pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_NoiseBuffer.padding);
	pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_NoiseBuffer.scrollSpeeds);
	pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_NoiseBuffer.scales);

	pMtrl->SetData(SHADER_PARAM::VEC2_0, &m_DistortionBuffer.distortion1);
	pMtrl->SetData(SHADER_PARAM::VEC2_1, &m_DistortionBuffer.distortion2);
	pMtrl->SetData(SHADER_PARAM::VEC2_2, &m_DistortionBuffer.distortion3);
	pMtrl->SetData(SHADER_PARAM::FLOAT_2, &m_DistortionBuffer.distortionScale);
	pMtrl->SetData(SHADER_PARAM::FLOAT_3, &m_DistortionBuffer.distortionBias);

}

void CFire::update()
{
	float angle=0.f;
	float rotation = 0.f;
	//	frametime ConstBuffer Parameter √÷Ω≈»≠
	m_NoiseBuffer.frameTime += DT;
	//cout << m_NoiseBuffer.frameTime << endl;
	if (m_NoiseBuffer.frameTime > 2000.f)
		m_NoiseBuffer.frameTime = 0.f;
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_NoiseBuffer.frameTime);

	for (auto player : CSceneMgr::GetInst()->GetCurScene()->GetLayer(1)->GetParentObj())
	{
		m_pPlayer = player;

	}
	Vec3 PlayerRot = m_pPlayer->Transform()->GetLocalRot();
	Vec3 WorldDir = /*m_pPlayer->Transform()->GetLocalPos() - this->Transform()->GetLocalPos();*/ Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 localPos = Transform()->GetLocalPos();

	m_fcreate_time += DT;

	if (m_fcreate_time >= 1.5f) WorldDir *= -1.f;
	if (m_fcreate_time >= 3.0f) WorldDir *= -1.f;
	if (m_fcreate_time >= 4.5f) WorldDir *= -1.f;
	if (m_fcreate_time >= 6.f) GetObj()->SetDead();



	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	float Temp = 500 * DT;

	localPos += -WorldDir * Temp;

	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(PlayerRot+Vec3(XMConvertToRadians(90.f),XM_PI,0.f));
}



CFire::CFire()
	:CScript((UINT)COMPONENT_TYPE::SCRIPT)
{
	m_NoiseBuffer.scrollSpeeds = Vec4(1.3f, 2.1f, 2.3f, 1.f);
	m_NoiseBuffer.scales = Vec4(1.f, 2.f, 3.f, 1.f);
	m_NoiseBuffer.padding = 0.0f;
	m_NoiseBuffer.frameTime = 0.f;
	//	
	m_NoiseBuffer.frameTime = CTimeMgr::GetInst()->GetAccTime();

	m_DistortionBuffer.distortion1 = Vec2(0.1f, 0.2f);
	m_DistortionBuffer.distortion2 = Vec2(0.1f, 0.3f);
	m_DistortionBuffer.distortion3 = Vec2(0.1f, 0.1f);
	m_DistortionBuffer.distortionBias = 0.8f;
	m_DistortionBuffer.distortionScale = 0.5f;
}

CFire::~CFire()
{
}
