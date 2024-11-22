#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "func.fx"




RWStructuredBuffer<tParticle>       ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount>     SpawnCountBuffer : register(u1);

Texture2D                           NoiseTex : register(t20);
StructuredBuffer<tParticleModule>   Module : register(t21);


#define ParticleBufferSize  g_int_0
#define ParticleObjectPos   g_vec4_0
#define Particle            ParticleBuffer[_ID.x]
#define SpawnCount          SpawnCountBuffer[0].spawncount


// Module Check
#define SpawnModule         Module[0].Module[0]
#define SpawnBurstModule    Module[0].Module[1]
#define SpawnShapeType      Module[0].SpawnShape
#define AddVelocityModule   Module[0].Module[2]
#define ScaleModule         Module[0].Module[3]
#define DragModule          Module[0].Module[4]


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
        // SpawnModule 이 켜져있으면
        if (SpawnModule || SpawnBurstModule)
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
                    float3 vRandom = GetRandom(NoiseTex, ((float) _ID.x / (float)(ParticleBufferSize - 1)));
                    float3 vRandom1 = GetRandom(NoiseTex, ((float) (_ID.x + 1) / (float) (ParticleBufferSize - 1)));
                    float3 vRandom2 = GetRandom(NoiseTex, ((float) (_ID.x + 2) / (float) (ParticleBufferSize - 1)));
                                        
                    float3 vSpawnPos = (float3) 0.f;
                    
                    // 0 : Box,  1 : Sphere
                    if (0 == SpawnShapeType)
                    {
                        vSpawnPos.x = vRandom.x * Module[0].SpawnShapeScale.x - (Module[0].SpawnShapeScale.x / 2.f);
                        vSpawnPos.y = vRandom.y * Module[0].SpawnShapeScale.y - (Module[0].SpawnShapeScale.y / 2.f);
                        vSpawnPos.z = vRandom.z * Module[0].SpawnShapeScale.z - (Module[0].SpawnShapeScale.z / 2.f);
                    }
                    else if(1 == SpawnShapeType)
                    {
                        float fRadius = Module[0].SpawnShapeScale.x;                                                
                        float fBlockRadius = Module[0].BlockSpawnShapeScale.x;                        
                        float fDifferRadius = fRadius - fBlockRadius;
                        
                        vSpawnPos = normalize(vRandom1 - 0.5f) * fDifferRadius * vRandom2.x 
                                    + normalize(vRandom1 - 0.5f) * fBlockRadius;
                    }
                                                        
                    // Add Velocity Module
                    Particle.vVelocity = (float3) 0.f;
                    if (AddVelocityModule)
                    {                        
                        float fSpeed = Module[0].AddMinSpeed + (Module[0].AddMaxSpeed - Module[0].AddMinSpeed) * vRandom2.x;
                        
                        // Random
                        if (0 == Module[0].AddVelocityType)                        
                            Particle.vVelocity = normalize(vRandom2 - 0.5f) * fSpeed;                        
                        // FromCenter
                        else if (1 == Module[0].AddVelocityType)                        
                            Particle.vVelocity = normalize(vSpawnPos) * fSpeed;
                        // ToCenter
                        else if (2 == Module[0].AddVelocityType)
                            Particle.vVelocity = -normalize(vSpawnPos) * fSpeed;
                        // Fixed
                        else
                            Particle.vVelocity = normalize(Module[0].AddVelocityFixedDir) * fSpeed;
                    }
                    
                    
                    Particle.vLocalPos = vSpawnPos;
                    Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos.xyz;
                    Particle.vWorldInitScale = (Module[0].vSpawnMaxScale - Module[0].vSpawnMinScale) * vRandom.x + Module[0].vSpawnMinScale;
                                        
                    Particle.vColor = Module[0].vSpawnColor;
                    
                    Particle.Age = 0.f;
                    Particle.NomarlizedAge = 0;
                    Particle.Life = (Module[0].MaxLife - Module[0].MinLife) * vRandom1.y + Module[0].MinLife;
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
        // Velocity 에 따른 이동 구현
        Particle.vWorldPos += Particle.vVelocity * DT;       
        
        // Scale 모듈에 따른 현재 크기 계산
        Particle.vWorldCurrentScale = Particle.vWorldInitScale;
        if(ScaleModule)
            Particle.vWorldCurrentScale = ((Module[0].EndScale - Module[0].StartScale) * Particle.NomarlizedAge + Module[0].StartScale) * Particle.vWorldInitScale;
        
        
        if(DragModule)
        {
            if (Particle.NomarlizedAge < Module[0].DestNormalizedAge)
            {
                // 기울기
                // (목적 속력 - 현재 속력) / (목적 NA - 현재 NA)
                float Gradient = (Module[0].LimitSpeed - length(Particle.vVelocity)) / (Module[0].DestNormalizedAge - Particle.NomarlizedAge);
                float NADT = DT / Particle.Life;
            
                float NewSpeed = length(Particle.vVelocity) + (Gradient * NADT);
                Particle.vVelocity = normalize(Particle.vVelocity) * NewSpeed;
            }
            
            else
            {
                if (Module[0].LimitSpeed < length(Particle.vVelocity))
                {
                    Particle.vVelocity = normalize(Particle.vVelocity) * Module[0].LimitSpeed;
                }
            }
        }
        
        
        Particle.Age += DT;
        Particle.NomarlizedAge = Particle.Age / Particle.Life;
        if (Particle.Life <= Particle.Age)
        {
            Particle.Active = 0;
        }
    }
}

#endif