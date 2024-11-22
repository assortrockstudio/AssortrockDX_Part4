#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*  m_ParticleBuffer;

    UINT                m_MaxParticle;  // 파티클 최대 개수



public:
    virtual void finaltick() override;
    virtual void render() override;

public:
    CParticleSystem();
    ~CParticleSystem();
};

