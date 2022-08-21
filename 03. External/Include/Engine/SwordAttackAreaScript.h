#pragma once
#include "Script.h"
#include "Mesh.h"

class CSwordAttackAreaScript :
    public CScript
{
private:
    Vec3 m_vCalPos;
    Vec3 m_vCalRot;
    CGameObject* m_pObject;
public:

    virtual void update();
    CSwordAttackAreaScript();
    virtual ~CSwordAttackAreaScript();
    void Set_Object(CGameObject* _pObj) { m_pObject = _pObj; }
    CGameObject* Get_Object() { return m_pObject; }
    void SetPos(const Vec3& _Pos) { m_vCalPos = _Pos; }
    void SetRot(const Vec3& _Rot) { m_vCalRot = _Rot; }

    virtual void OnCollisionEnter(CCollider2D* _pOther);
    virtual void OnCollision(CCollider2D* _pOther);
    virtual void OnCollisionExit(CCollider2D* _pOther);

public:
    CLONE(CSwordAttackAreaScript);
};