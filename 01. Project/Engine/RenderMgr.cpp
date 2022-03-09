#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "Camera.h"

#include "SceneMgr.h"
#include "ConstantBuffer.h"

#include "TimeMgr.h"
#include "EventMgr.h"

CRenderMgr::CRenderMgr()
	: m_arrRT{}
	, m_arrMRT{}		
{	
}

CRenderMgr::~CRenderMgr()
{	
}

void CRenderMgr::render()
{
	// �ʱ�ȭ
	float arrColor[4] = { 0.6f,0.6f, 0.6f, 1.f };
	CDevice::GetInst()->render_start(arrColor);

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();
	}	

	// ���
	CDevice::GetInst()->render_present();
}

void CRenderMgr::render_tool()
{
	// �ʱ�ȭ
	float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
	//Clear(arrColor);
}
