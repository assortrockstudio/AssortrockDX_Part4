#pragma once
#include "CScript.h"
class CCameraMoveScript :
    public CScript
{
private:
    float       m_Speed;

public:
    virtual void tick() override;

private:
    void MoveByPerspective();
    void MoveByOrthographic();

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

