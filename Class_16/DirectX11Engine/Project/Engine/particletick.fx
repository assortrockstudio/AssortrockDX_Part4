#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "func.fx"

struct tSpawnCount
{
    int spawncount;
    uint3 padding;
};

struct tParticleModule
{
    // Spawn
    uint    SpawnRate;   // 초당 파티클 생성 개수
    float4  vSpawnColor; // 생성 시점 색상
    float4  vSpawnMinScale; // 생성 시 최소 크기
    float4  vSpawnMaxScale; // 생성 시 최대 크기

    float   MinLife; // 최소 수명
    float   MaxLife; // 최대 수명

	// Module On / Off
    int     Module[1];
    
    float4 padding;
};



RWStructuredBuffer<tParticle>       ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount>     SpawnCountBuffer : register(u1);

Texture2D                           NoiseTex : register(t20);
StructuredBuffer<tParticleModule>   Module : register(t21);


#define ParticleBufferSize  g_int_0
#define ParticleObjectPos   g_vec4_0
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
        if (Module[0].Module[0])
        {
            // 이번에 활성화 가능한 파티클 수
            int CurSpawnCount = SpawnCount;
        
            // 파티클 활성화 가능 숫자가 0보다 크면
            while (0 < CurSpawnCount)
            {
                // SpawnCount 에 들어있던 값을 받아 볼 변수
                int OriginValue = 0;
            
                // 교체를 시도할때, SpawnCount 에 들어있는 값이 예상값과 동일한 경우만 값을 교체한다.
                //InterlockedExchange(SpawnCount, SpawnCount - 1, OriginValue);
                InterlockedCompareExchange(SpawnCount, CurSpawnCount, SpawnCount - 1, OriginValue);
            
                // 교체한 값이 원래 예상하던 값과 일치하면 활성화 성공
                // 실패했으면 다시 반복문을 통해서 다음 시도로 진입한다.
                if (CurSpawnCount == OriginValue)
                {
                    float3 vRandom = GetRandom(NoiseTex, _ID.x / (ParticleBufferSize - 1));
                
                    float BoxScale = 300.f;
                    float3 vRandomPos = (float3) 0.f;
                
                    vRandomPos.x = vRandom.x * BoxScale - (BoxScale / 2.f);
                    vRandomPos.y = vRandom.y * BoxScale - (BoxScale / 2.f);
                    vRandomPos.z = vRandom.z * BoxScale - (BoxScale / 2.f);
                
                    Particle.vLocalPos = vRandomPos;
                    Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos.xyz;
                    Particle.vWorldScale = (Module[0].vSpawnMaxScale - Module[0].vSpawnMinScale) * vRandom.x + Module[0].vSpawnMinScale;
                                        
                    Particle.vColor = Module[0].vSpawnColor;
                    
                    Particle.Age = 0.f;
                    Particle.Life = (Module[0].MaxLife - Module[0].MinLife) * vRandom.y + Module[0].MinLife;
                    Particle.Active = 1;
                
                    break;
                }
            
                CurSpawnCount = SpawnCount;
            }
        }
        
        
      
    } 
    
    // 스레드가 담당하는 파티클이 활성화 상태면 Tick 을 수행한다.
    else
    {
        Particle.Age += DT;
        if (Particle.Life <= Particle.Age)
        {
            Particle.Active = 0;
        }
    }
}

#endif