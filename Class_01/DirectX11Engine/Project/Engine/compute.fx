#ifndef _COMPUTE
#define _COMPUTE

// hlsl 5.0 ���� �ִ� �׷�� ���� ������ ������ ������ 1024���̴�.

// SV_GroupThreadID     : �׷� �������� �������� 3���� ��ǥ
// SV_GroupIndex        : �׷� ������ �������� 1���� ��ǥ
// SV_GroupID           : �����尡 ���� �׷��� 3���� ��ǥ
// SV_DispatchThreadID  : ��� �׷��� ������ ��ü ���� 3���� ��ǥ

//Texture2D TargetTex : register(t21);
RWTexture2D<float4> TargetTex : register(u0); // unordered register

[numthreads(32, 32, 1)]
void CS_Test(int3 _ThreadID : SV_DispatchThreadID)
{
    TargetTex[_ThreadID.xy] = float4(1.f, 0.f, 0.f, 1.f);
}

#endif