#pragma once
#include "CScript.h"

class CStructuredBuffer;

class CPlayerScript :
    public CScript
{
private:
    float               m_Speed;
    CGameObject*        m_TargetMon;   

public:
    void SetTarget(CGameObject* _Target){ m_TargetMon = _Target; }


public:
    virtual void begin() override;
    virtual void tick() override;   
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;

public:
    CPlayerScript();
    ~CPlayerScript();
};

