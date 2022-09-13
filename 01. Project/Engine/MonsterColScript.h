#pragma once
#include "Script.h"
#include "Mesh.h"

class CMonsterColScript :
    public CScript
{
private:
    Vec3 m_vCalPos;
    Vec3 m_vCalRot;
    CGameObject* m_pMonster;
public:

    virtual void update();
    CMonsterColScript();
    virtual ~CMonsterColScript();
    void SetMonster(CGameObject* _pObj) { m_pMonster = _pObj; }
    CGameObject* GetMonster() { return m_pMonster; }
    void SetPos(const Vec3& _Pos) { m_vCalPos = _Pos; }
    void SetRot(const Vec3& _Rot) { m_vCalRot = _Rot; }

    virtual void OnCollisionEnter(CCollider2D* _pOther);
    virtual void OnCollision(CCollider2D* _pOther);
    virtual void OnCollisionExit(CCollider2D* _pOther);

public:
    CLONE(CMonsterColScript);
};