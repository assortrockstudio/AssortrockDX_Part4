#pragma once
#include "CRenderComponent.h"

#include "CParticleTickCS.h"

struct tSpawnCount
{
    int  SpawnCount;
    UINT padding[3];
};


class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleTickCS>    m_TickCS;
    CStructuredBuffer*      m_ParticleBuffer;   // ParticleBuffer
    CStructuredBuffer*      m_SpawnCountBuffer; // SpawnCount 전달 용도

    Ptr<CTexture>           m_ParticleTex;      // 파티클 입자 텍스쳐
    
    UINT                    m_SpawnCount;       // SpawnCount Per Tick
    float                   m_Time;             // 누적 시간

    // Particle Data
    UINT                    m_MaxParticle;      // 파티클 최대 개수
    UINT                    m_SpawnRate;        // 초당 파티클 생성개수 (Spawn Per Second)






public:
    void SetParticleTexture(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }


public:
    virtual void finaltick() override;
    virtual void render() override;

public:
    CParticleSystem();
    ~CParticleSystem();
};

