#pragma once
#include "Script.h"
#include "Mesh.h"

struct tAnimation {
	wstring Name;
	UINT StartFrame;
	UINT EndFrame;
	bool Stop;
	bool Repeat;
};



//typedef struct tagTrailVertex
//{
//	Vec3 vPosition;
//	DWORD dwColor;
//	Vec2 vTexUV;
//}TRIALVTX;

class CSwordScript:
	public CScript
{
private:
    tAnimation* m_pCurAnimation;

    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    tMTBone* m_pTargetBone;
    CGameObject* m_pTargetObject;
    int m_iTargetBoneIdx;

public:
	void init();
	void update();

	void SetTarget(CGameObject* _target) { m_pTargetObject = _target; }
	void SetBoneIdx(int _boneIdx) { m_iTargetBoneIdx = _boneIdx; }

	/*list<TRIALVTX>	m_lstTrailVtx;
	Vec3					m_vTopPos;
	Vec3					m_vBottomPos;
	unsigned int			m_iTrailIdxCount;*/


	CSwordScript();
	virtual ~CSwordScript();

public:
	CLONE(CSwordScript);
};