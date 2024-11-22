#pragma once
#include "CComputeShader.h"

#include "CTexture.h"

class CSetColorShader :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_TargetTex;


public:
    void SetTargetTexture(Ptr<CTexture> _Tex) { m_TargetTex = _Tex; }


public:
    virtual int Binding() override;
    virtual void CalculateGroupNum()override;
    virtual void Clear() override;


public:
    CSetColorShader();
    ~CSetColorShader();
};

