#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"

struct tSpawnCount
{
    int spawncount;
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
            
    // 비활성된 파티클을 활성화
    if (0 == Particle.Active)
    {          
        // 이번에 활성화 가능한 파티클 수
        int CurSpawnCount = SpawnCount;        
        
        // 파티클 활성화 가능 숫자가 0보다 크면
        while (0 < CurSpawnCount) 
        {            
            // SpawnCount 에 들어있던 값을 받아 볼 변수
            int OriginValue = 0;
            InterlockedExchange(SpawnCount, SpawnCount - 1, OriginValue);
        
            // 교체한 값이 원래 예상하던 값과 일치하면 활성화 성공
            // 실패했으면 다시 반복문을 통해서 다음 시도로 진입한다.
            if (CurSpawnCount == OriginValue)
            {
                Particle.Active = 1;
                break;
            }
            
            CurSpawnCount = SpawnCount;            
        }
    }   
    
    // 스레드가 담당하는 파티클이 활성화 상태면 Tick 을 수행한다.
    else
    {
        Particle.vWorldPos.y += DT * 100.f;
    }
}

#endif