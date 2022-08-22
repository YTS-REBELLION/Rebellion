#pragma once
#pragma once
#include "Script.h"
#include "Mesh.h"

class CBossColScript :
    public CScript
{
private:
    Vec3 m_vCalPos;
    Vec3 m_vCalRot;
    CGameObject* m_pBoss;
public:

    virtual void update();
    CBossColScript();
    virtual ~CBossColScript();
    void SetBoss(CGameObject* _pObj) { m_pBoss = _pObj; }
    CGameObject* GetBoss() { return m_pBoss; }
    void SetPos(const Vec3& _Pos) { m_vCalPos = _Pos; }
    void SetRot(const Vec3& _Rot) { m_vCalRot = _Rot; }

    virtual void OnCollisionEnter(CCollider2D* _pOther);
    virtual void OnCollision(CCollider2D* _pOther);
    virtual void OnCollisionExit(CCollider2D* _pOther);

public:
    CLONE(CBossColScript);
};