#ifndef _STRUCT
#define _STRUCT

struct tLight
{
    float4 vDiffuse; // 난반사 (빛의 색상)
    float4 vAmbient; // 환경광 (빛으로 발생하는 최소한의 밝기)
    float4 vMaxSpecular; // 반사광 (빛으로 발생하는 반사광의 최대 밝기)
};

struct tLightInfo
{
    tLight  Light;  // 광원 색상 정보

    uint    LightType; // 0 : DirectionalLight, 1 : PointLight, 2 : SpotLight
    float3  WorldDir; // WorldSpace 에서 광원의 방향
    float3  WorldPos; // WorldSpace 에서 광원의 위치   
    float   Range; // SpotLight or PointLight 에 광원의 영향 범위
    float   Angle; // Spot Light 인 경우 광원의 각도범위
    float3  Padding;
};



#endif