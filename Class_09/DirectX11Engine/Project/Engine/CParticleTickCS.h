#pragma once
#include "CComputeShader.h"

class CParticleTickCS :
    public CComputeShader
{

public:
    virtual int Binding() override;
    virtual void CalculateGroupNum()override;
    virtual void Clear() override;

public:
    CParticleTickCS();
    ~CParticleTickCS();
};

