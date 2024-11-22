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
            InterlockedExchange(SpawnCount, SpawnCount - 1, OriginValue);
        
            // ��ü�� ���� ���� �����ϴ� ���� ��ġ�ϸ� Ȱ��ȭ ����
            // ���������� �ٽ� �ݺ����� ���ؼ� ���� �õ��� �����Ѵ�.
            if (CurSpawnCount == OriginValue)
            {
                Particle.Active = 1;
                break;
            }
            
            CurSpawnCount = SpawnCount;            
        }
    }   
    
    // �����尡 ����ϴ� ��ƼŬ�� Ȱ��ȭ ���¸� Tick �� �����Ѵ�.
    else
    {
        Particle.vWorldPos.y += DT * 100.f;
    }
}

#endif