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
    float3  vLocalPos;
    float3  vWorldPos;
    float3  vWorldScale;
    float4  vColor;
    float3  vVelocity;

    float   Life;
    float   Age;
    float   NomarlizedAge;
    int     Active;
};


#endif