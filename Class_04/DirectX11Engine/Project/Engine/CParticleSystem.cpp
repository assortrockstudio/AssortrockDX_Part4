#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticle(10)
{
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), 1, SB_TYPE::SRV_UAV, true, nullptr);
		
	tParticle particle = {};
	particle.Life = 10.f;
	particle.vWorldPos = Vec3(100.f, 100.f, 100.f);
	m_ParticleBuffer->SetData(&particle);

	tParticle particle1 = {};
	m_ParticleBuffer->GetData(&particle1);
}

CParticleSystem::~CParticleSystem()
{
	delete m_ParticleBuffer;
}

void CParticleSystem::finaltick()
{
	m_ParticleBuffer->Binding_CS_UAV(0);
}

void CParticleSystem::render()
{
}