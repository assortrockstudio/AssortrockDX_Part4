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
Texture2D                       NoiseTex : register(t20);


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
                // ��ƼŬ�� �������� ������ ��ġ�� �����ϱ�
                float2 vUV = (float2) 0.f;
                //                                         x�� ��ǥ�� �������� �̵���
                vUV.x = _ID.x / (ParticleBufferSize - 1) + (Time * 0.1f);
                
                //                                  Frequency     ����    y������ ��ǥ �̵���
                vUV.y = (sin((vUV.x - Time) * PI *    20.f    ) * 0.5f ) + (Time * 0.2f);
                
                float4 vNoise = NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                
                float BoxScale = 300.f;                
                float3 vRandomPos = (float3) 0.f;
                
                vRandomPos.x = vNoise.x * BoxScale - (BoxScale / 2.f);
                vRandomPos.y = vNoise.y * BoxScale - (BoxScale / 2.f);
                vRandomPos.z = vNoise.z * BoxScale - (BoxScale / 2.f);                
                
                Particle.vLocalPos = vRandomPos;
                Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos.xyz;
                Particle.vWorldScale = float3(10.f, 10.f, 1.f);
                Particle.Age = 0.f;
                Particle.Life = 2.f;                
                Particle.Active = 1;
                
                break;
            }
            
            CurSpawnCount = SpawnCount;            
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