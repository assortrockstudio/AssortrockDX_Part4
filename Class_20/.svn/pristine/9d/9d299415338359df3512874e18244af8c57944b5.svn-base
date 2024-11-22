#ifndef _FUNC
#define _FUNC

#include "value.fx"


float3 CalLight2D(int _LightIdx, float3 _vWorldPos)
{    
    tLightInfo info = g_Light2D[_LightIdx];
  
    // 광원 처리
    float3 vLightPow = (float3) 0.f;
        
    // Directional Light
    if (0 == info.LightType)
    {
        vLightPow = info.Light.vDiffuse.rgb + info.Light.vAmbient.rgb;
    }
    
    // PointLight
    else if (1 == info.LightType)
    {
        // 픽셀의 월드스페이스
        float fDist = distance(info.WorldPos.xy, _vWorldPos.xy);
        
        float fRatio = 0.f;
       
        //fRatio = 1.f - saturate(fDist / info.Range);
        fRatio = cos((fDist / info.Range) * (PI / 2.f));        
        
        if (fDist < info.Range)
        {
            vLightPow = info.Light.vDiffuse * fRatio;
        }
    }
    
    // SpotLight
    else
    {
        
    }
        
    return vLightPow;
}

int IsBinding(in Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    _tex.GetDimensions(width, height);
    
    if (width && height)
        return 1;
    else
        return 0;
}



float3 GetRandom(in Texture2D _Noise, float _NormalizedThreadID)
{
    // 파티클의 포지션을 랜덤한 위치로 지정하기
    float2 vUV = (float2) 0.f;      
    
    vUV.x = _NormalizedThreadID + (Time * 0.1f);
    vUV.y = (sin((vUV.x - Time) * PI * 20.f) * 0.5f) + (Time * 0.2f);
                
    float3 vNoise = _Noise.SampleLevel(g_sam_0, vUV, 0).xyz;
    
    return vNoise;
}



#endif