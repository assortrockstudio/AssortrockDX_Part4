#pragma once
#include "CScript.h"

class CMissileScript :
    public CScript
{
private:
    float       m_Speed;


public:
    virtual void begin() override;
    virtual void tick() override;


public:
    CMissileScript();
    ~CMissileScript();
};

