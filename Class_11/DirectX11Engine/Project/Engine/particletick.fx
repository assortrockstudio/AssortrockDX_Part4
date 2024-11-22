#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"

struct tSpawnCount
{
    uint spawncount;
    uint3 padding;
};

RWStructuredBuffer<tParticle>   ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> SpawnCountBuffer : register(u1);


#define ParticleBufferSize  g_int_0
#define Particle            ParticleBuffer[_ID.x]
#define SpawnCount          SpawnCountBuffer[0].spawncount



[numthreads(32, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{    
    // 스레드 ID == 파티클 Index
    // 스레드 ID 가 파티클 개수를 초과하면 리턴
    if (ParticleBufferSize <= _ID.x)
        return;
                
    InterlockedAdd(SpawnCount, 1);
    
    // InterlockedExchange();
    
    
    //// 비활성된 파티클을 활성화
    //if (0 < SpawnCount && !Particle.Active)
    //{
    //    SpawnCount -= 1;
        
    //    Particle.Active = 1;
    //}
    
    //if(Particle.Active)
    //{
    //    Particle.vWorldPos.y += DT * 100.f;
    //}
}

#endif