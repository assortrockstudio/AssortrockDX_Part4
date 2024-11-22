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
    CStructuredBuffer*      m_SpawnCountBuffer; // SpawnCount ���� �뵵

    Ptr<CTexture>           m_ParticleTex;      // ��ƼŬ ���� �ؽ���
    
    UINT                    m_SpawnCount;       // SpawnCount Per Tick
    float                   m_Time;             // ���� �ð�

    // Particle Data
    UINT                    m_MaxParticle;      // ��ƼŬ �ִ� ����
    UINT                    m_SpawnRate;        // �ʴ� ��ƼŬ �������� (Spawn Per Second)






public:
    void SetParticleTexture(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }


public:
    virtual void finaltick() override;
    virtual void render() override;

public:
    CParticleSystem();
    ~CParticleSystem();
};

