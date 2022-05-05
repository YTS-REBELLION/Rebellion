#include "stdafx.h"
#include "Sword.h"


CSword::CSword()
	: CScript((UINT)SCRIPT_TYPE::SWORDSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CSword::~CSword()
{
}

void CSword::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CSword::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	
	vRot.y -= DT * 5.f;



	/* TRIALVTX vertex[100];
	for (_uint i = 0; i < m_iTrailIdxCount; ++i)
		vertex[i].dwColor = D3DCOLOR_ARGB(255, 255, 0, 255);
	for (_uint i = 0; i < m_iTrailIdxCount / 2; ++i)
	{
		vertex[2 * i].vTexUV = { ((m_iTrailIdxCount / 2.f - i) / (m_iTrailIdxCount / 2 - 1.f)), 0.f };
		vertex[(2 * i) + 1].vTexUV = { ((m_iTrailIdxCount / 2.f - i) / (m_iTrailIdxCount / 2 - 1.f)), 1.f };
	}

	int iIdx = 0;
	for (auto& iter = m_lstTrailVtx.rbegin(); iter != m_lstTrailVtx.rend(); ++iter)
	{
		vertex[iIdx].vPosition = iter->vPosition;
		++iIdx;
		if (iIdx >= m_iTrailIdxCount)
			break;
	}

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);


	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		pEffect = m_pSwordTrailShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);

		iPassMax = 0;
		

		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(0);

		m_pGraphicDev->SetFVF(Engine::FVF_TRAIL);
		m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iTrailIdxCount - 2, vertex, sizeof(Engine::TRIALVTX));

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}*/









	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}