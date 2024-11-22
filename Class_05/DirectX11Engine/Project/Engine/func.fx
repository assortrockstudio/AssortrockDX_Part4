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



#endif