#pragma once
#include "CScript.h"
class CBackgroundScript :
    public CScript
{
private:
    float   m_Intence;

public:
    virtual void tick() override;

    
public:
    CBackgroundScript();
    ~CBackgroundScript();
};

