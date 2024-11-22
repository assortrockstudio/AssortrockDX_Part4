#pragma once
#include "CRenderComponent.h"

#include "CParticleTickCS.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleTickCS>    m_TickCS;
    CStructuredBuffer*      m_ParticleBuffer;    
    UINT                    m_MaxParticle;  // 파티클 최대 개수
    Ptr<CTexture>           m_ParticleTex;  // 파티클 입자 텍스쳐

public:
    void SetParticleTexture(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }


public:
    virtual void finaltick() override;
    virtual void render() override;

public:
    CParticleSystem();
    ~CParticleSystem();
};

