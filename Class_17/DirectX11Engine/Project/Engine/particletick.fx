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



[numthreads(32, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{    
    // ������ ID == ��ƼŬ Index
    // ������ ID �� ��ƼŬ ������ �ʰ��ϸ� ����
    if (ParticleBufferSize <= _ID.x)
        return;
            
    // ��Ȱ���� ��ƼŬ�� Ȱ��ȭ
    if (0 == Particle.Active)
    {
        // SpawnModule �� ����������
        if (SpawnModule || SpawnBurstModule)
        {
            // �̹��� Ȱ��ȭ ������ ��ƼŬ ��
            int CurSpawnCount = SpawnCount;
        
            // ��ƼŬ Ȱ��ȭ ���� ���ڰ� 0���� ũ��
            while (0 < CurSpawnCount)
            {
                // SpawnCount �� ����ִ� ���� �޾� �� ����
                int OriginValue = 0;
                
                // ��ü�� �õ��Ҷ�, SpawnCount �� ����ִ� ���� ���󰪰� ������ ��츸 ���� ��ü�Ѵ�.
                //InterlockedExchange(SpawnCount, SpawnCount - 1, OriginValue);
                InterlockedCompareExchange(SpawnCount, CurSpawnCount, SpawnCount - 1, OriginValue);
            
                // ��ü�� ���� ���� �����ϴ� ���� ��ġ�ϸ� Ȱ��ȭ ����
                // ���������� �ٽ� �ݺ����� ���ؼ� ���� �õ��� �����Ѵ�.
                if (CurSpawnCount == OriginValue)
                {
                    float3 vRandom = GetRandom(NoiseTex, ((float) _ID.x / (float)(ParticleBufferSize - 1)));
                    float3 vRandom1 = GetRandom(NoiseTex, ((float) (_ID.x + 1) / (float) (ParticleBufferSize - 1)));
                    float3 vRandom2 = GetRandom(NoiseTex, ((float) (_ID.x + 2) / (float) (ParticleBufferSize - 1)));
                    
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
                    Particle.Life = (Module[0].MaxLife - Module[0].MinLife) * vRandom1.y + Module[0].MinLife;
                    Particle.Active = 1;
                
                    break;
                }
            
                CurSpawnCount = SpawnCount;
            }
        }
        
        
      
    } 
    
    // �����尡 ����ϴ� ��ƼŬ�� Ȱ��ȭ ���¸� Tick �� �����Ѵ�.
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