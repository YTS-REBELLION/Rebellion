#pragma once
#include "Script.h"
#include "Mesh.h"

class CPlayerColScript :
    public CScript
{
private:
    Vec3 m_vCalPos;
    Vec3 m_vCalRot;
    CGameObject* m_pPlayer;
public:

    virtual void update();
    CPlayerColScript();
    virtual ~CPlayerColScript();
    void SetPlayer(CGameObject* _pObj) { m_pPlayer = _pObj; }
    CGameObject* GetPlayer() { return m_pPlayer; }
    void SetPos(const Vec3& _Pos) { m_vCalPos = _Pos; }
    void SetRot(const Vec3& _Rot) { m_vCalRot = _Rot; }

    virtual void OnCollisionEnter(CCollider2D* _pOther);
    virtual void OnCollision(CCollider2D* _pOther);
    virtual void OnCollisionExit(CCollider2D* _pOther);

public:
    CLONE(CPlayerColScript);
};