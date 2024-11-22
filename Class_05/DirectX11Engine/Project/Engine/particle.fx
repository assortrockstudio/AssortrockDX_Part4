#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"

StructuredBuffer<tParticle> ParticleBuffer : register(t17);

struct VS_PARTICLE_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_PARTICLE_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_PARTICLE_OUT VS_Particle(VS_PARTICLE_IN _in)
{
    VS_PARTICLE_OUT output = (VS_PARTICLE_OUT) 0.f;
        
    float3 vWorldPos = (_in.vPos * ParticleBuffer[g_int_0].vWorldScale) + ParticleBuffer[g_int_0].vWorldPos;
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
    
    output.vPosition = vProjPos;
    output.vUV = _in.vUV;    
    
    return output;
}

float4 PS_Particle(VS_PARTICLE_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vOutColor;
}






#endif