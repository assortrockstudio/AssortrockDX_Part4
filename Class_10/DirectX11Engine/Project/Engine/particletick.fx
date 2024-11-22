#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

#define ParticleBufferSize  g_int_0
#define Particle            ParticleBuffer[_ID.x]




[numthreads(32, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{    
    // ������ ID == ��ƼŬ Index
    // ������ ID �� ��ƼŬ ������ �ʰ��ϸ� ����
    if (ParticleBufferSize <= _ID.x)
        return;
        
    Particle.vWorldPos.y += DT * 100.f;
}

#endif