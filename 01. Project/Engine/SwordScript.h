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

	PERSON_OBJ_TYPE m_eType;

public:

	void init(PERSON_OBJ_TYPE type, CGameObject* _target, int _boneIdx);

	void update();

	void Set_Sword_To_Fbx();

	void Set_FM_Player();

	CSwordScript();
	virtual ~CSwordScript();
	CGameObject* m_pPlayer;
public:
	CLONE(CSwordScript);

public:
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollision(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
};