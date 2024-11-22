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
    uint    SpawnRate;   // �ʴ� ��ƼŬ ���� ����
    float4  vSpawnColor; // ���� ���� ����
    float4  vSpawnMinScale; // ���� �� �ּ� ũ��
    float4  vSpawnMaxScale; // ���� �� �ִ� ũ��

    float   MinLife; // �ּ� ����
    float   MaxLife; // �ִ� ����

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
    // ������ ID == ��ƼŬ Index
    // ������ ID �� ��ƼŬ ������ �ʰ��ϸ� ����
    if (ParticleBufferSize <= _ID.x)
        return;
            
    // ��Ȱ���� ��ƼŬ�� Ȱ��ȭ
    if (0 == Particle.Active)
    {
        if (Module[0].Module[0])
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