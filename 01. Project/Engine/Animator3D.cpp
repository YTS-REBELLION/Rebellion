#include "stdafx.h"
#include "Animator3D.h"

#include "TimeMgr.h"
#include "MeshRender.h"
#include "Material.h"

CAnimator3D::CAnimator3D()
	: m_iCurClip(0)
	, m_dCurTime(0.)
	, m_iFrameCount(30)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{	
}

CAnimator3D::~CAnimator3D()
{
}

void CAnimator3D::update()
{	
}

void CAnimator3D::lateupdate()
{	
}

void CAnimator3D::finalupdate()
{
	m_dCurTime = 0.f;
	// ���� ������� Clip �� �ð��� �����Ѵ�.
	m_vecClipUpdateTime[m_iCurClip] += DT;

	if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
	{
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}

	m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	// �� ������ŭ �ݺ��ϸ� ���� �ð��� �°� ��� �� ����� ��� �������ش�.
	for (size_t i = 0; i < m_pVecBones->size(); ++i)
	{
		if (m_pVecBones->at(i).vecKeyFrame.empty())
		{
			// Ű������ �� ����� ���� ���� ���
			m_vecFinalBoneMat[i] = m_pVecBones->at(i).matBone;
			continue;
		}
			   
		// �ð��� ���Ͽ� �ε����� ���Ѵ�.
		int	iFrameIndex = (int)(m_dCurTime * (float)m_iFrameCount);
		int	iFrameNextIndex = 0;

		/*{
			m_vecFinalBoneMat[i] = m_pVecBones->at(i).matOffset *
				XMMatrixAffineTransformation(m_pVecBones->at(i).vecKeyFrame[iFrameIndex].vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f)
					, m_pVecBones->at(i).vecKeyFrame[iFrameIndex].qRot
					, m_pVecBones->at(i).vecKeyFrame[iFrameIndex].vTranslate);
			continue;
		}*/

		if (iFrameIndex >= m_pVecClip->at(0).iFrameLength - 1)
		{
			iFrameNextIndex = iFrameIndex;
		}
		else
		{
			iFrameNextIndex = iFrameIndex + 1;
		}

		const tMTKeyFrame& tKeyFrame = m_pVecBones->at(i).vecKeyFrame[iFrameIndex];
		const tMTKeyFrame& tNextKeyFrame = m_pVecBones->at(i).vecKeyFrame[iFrameNextIndex];

		double	fFrameTime = tKeyFrame.dTime;
		double	fNextFrameTime = tNextKeyFrame.dTime;

		// �����Ӱ��� �ð��� ���� ������ �����ش�.
		double	fPercent = (m_dCurTime - fFrameTime) / (1.f / m_iFrameCount);

		XMVECTOR vS1 = tKeyFrame.vScale;
		XMVECTOR vS2 = tNextKeyFrame.vScale;

		XMVECTOR vT1 = tKeyFrame.vTranslate;
		XMVECTOR vT2 = tNextKeyFrame.vTranslate;

		XMVECTOR vQ1 = XMLoadFloat4(&tKeyFrame.qRot);
		XMVECTOR vQ2 = XMLoadFloat4(&tNextKeyFrame.qRot);

		XMVECTOR vS = XMVectorLerp(vS1, vS2, (float)fPercent);
		XMVECTOR vT = XMVectorLerp(vT1, vT2, (float)fPercent);
		XMVECTOR vQ = XMQuaternionSlerp(vQ1, vQ2, (float)fPercent);

		XMVECTOR vQZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		// ������ ����� ���Ͽ� ���� ������� ������.				
		m_vecFinalBoneMat[i] = m_pVecBones->at(i).matOffset * XMMatrixAffineTransformation(vS, vQZero, vQ, vT);
	}	
}

void CAnimator3D::SaveToScene(FILE * _pFile)
{
}

void CAnimator3D::LoadFromScene(FILE * _pFile)
{
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}

void CAnimator3D::UpdateData()
{		
	// Bone Texture Update
	//m_pBoneTex->SetData(&m_vecFinalBoneMat[0], sizeof(Matrix) * m_vecFinalBoneMat.size());

	/*UINT iMtrlCount = MeshRender()->GetMaterialCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetData(SHADER_PARAM::TEX_3, &m_pBoneTex);
	}*/
}