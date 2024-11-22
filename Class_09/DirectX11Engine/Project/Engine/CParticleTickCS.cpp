#include "pch.h"
#include "CParticleTickCS.h"

CParticleTickCS::CParticleTickCS()
    : CComputeShader(32, 1, 1)
{
}

CParticleTickCS::~CParticleTickCS()
{
}

int CParticleTickCS::Binding()
{

    return S_OK;
}

void CParticleTickCS::CalculateGroupNum()
{
}

void CParticleTickCS::Clear()
{
}
