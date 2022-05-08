#include "stdafx.h"
#include "Miro.h"
CMiro::CMiro()
	: CScript((UINT)SCRIPT_TYPE::WALLSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CMiro::~CMiro()
{
}

void CMiro::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

}

void CMiro::update()
{
	Vec3 WorldDir;
	Vec3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetLocalRot();
	//vRot.y +=1*DT;
	//cout << "roty:"<<vRot.y << endl;
	Transform()->SetLocalPos(localPos);
	Transform()->SetLocalRot(vRot);



}