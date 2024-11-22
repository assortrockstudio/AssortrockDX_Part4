#include "pch.h"
#include "CParticleTickCS.h"

#include "CStructuredBuffer.h"

CParticleTickCS::CParticleTickCS()
    : CComputeShader(32, 1, 1)
    , m_ParticleBuffer(nullptr)
{
}

CParticleTickCS::~CParticleTickCS()
{
}

int CParticleTickCS::Binding()
{
    if (nullptr == m_ParticleBuffer)
        return E_FAIL;

    m_ParticleBuffer->Binding_CS_UAV(0);

    m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();

    return S_OK;
}

void CParticleTickCS::CalculateGroupNum()
{
    m_GroupX = m_ParticleBuffer->GetElementCount() / m_ThreadPerGroupX;
    m_ParticleBuffer->GetElementCount() % m_ThreadPerGroupX ? m_GroupX += 1 : m_GroupX;

    m_GroupY = 1;
    m_GroupZ = 1;
}

void CParticleTickCS::Clear()
{
    m_ParticleBuffer->Clear_UAV();
    m_ParticleBuffer = nullptr;
}
