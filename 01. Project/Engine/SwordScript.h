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

	void update();

	void SetTarget(CGameObject* _target) { m_pTargetObject = _target; }
	void SetBoneIdx(int _boneIdx) { m_iTargetBoneIdx = _boneIdx; }

	CSwordScript();
	virtual ~CSwordScript();

public:
	CLONE(CSwordScript);
};