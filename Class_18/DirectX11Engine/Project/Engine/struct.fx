#ifndef _STRUCT
#define _STRUCT

struct tLight
{
    float4 vDiffuse; // ���ݻ� (���� ����)
    float4 vAmbient; // ȯ�汤 (������ �߻��ϴ� �ּ����� ���)
    float4 vMaxSpecular; // �ݻ籤 (������ �߻��ϴ� �ݻ籤�� �ִ� ���)
};

struct tLightInfo
{
    tLight  Light;  // ���� ���� ����

    uint    LightType; // 0 : DirectionalLight, 1 : PointLight, 2 : SpotLight
    float3  WorldDir; // WorldSpace ���� ������ ����
    float3  WorldPos; // WorldSpace ���� ������ ��ġ   
    float   Range; // SpotLight or PointLight �� ������ ���� ����
    float   Angle; // Spot Light �� ��� ������ ��������
    float3  Padding;
};


struct tParticle
{
    float4  vColor;
    float3  vLocalPos;
    float3  vWorldPos;
    float3  vWorldScale;
    float3  vWorldRotation;
    
    float3  vForce;
    float3  vVelocity;

    float   Life;
    float   Age;
    float   NomarlizedAge;
    int     Active;
    
    float2  padding;
};

struct tSpawnCount
{
    int spawncount;
    uint3 padding;
};

struct tParticleModule
{
    // Spawn
    uint    SpawnRate; // �ʴ� ��ƼŬ ���� ����
    float4  vSpawnColor; // ���� ���� ����
    float4  vSpawnMinScale; // ���� �� �ּ� ũ��
    float4  vSpawnMaxScale; // ���� �� �ִ� ũ��

    float   MinLife;            // �ּ� ����
    float   MaxLife;            // �ִ� ����

    uint    SpawnShape;         // 0 : Box,  1: Sphere
    float3  SpawnShapeScale;    // SpawnShapeScale.x == Radius

    uint    BlockSpawnShape;    // 0 : Box,  1: Sphere
    float3  BlockSpawnShapeScale; // SpawnShapeScale.x == Radius    
    
    // Spawn Burst
    uint    SpawnBurstCount;    // �ѹ��� �߻���Ű�� Particle ��
    uint    SpawnBurstRepeat;
    float   SpawnBurstRepeatTime;
    
    // Add Velocity
    uint    AddVelocityType;    // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
    float3  AddVelocityFixedDir;
    float   AddSpeed;
    
	// Module On / Off
    int     Module[3];
    
    float4  padding;
};


#endif