#include "pch.h"
#include "CParticleSystem.h"

#include "CTransform.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticle(100)
{
	// Particle Tick �뵵 ComputeShader 
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleCS").Get();


	// ParticleSystem �� ����� �޽ÿ� ��Ƽ���� ����
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl"));


	// ��ƼŬ ������ ������ ����ȭ ���� ����
	// ��ƼŬ 10���� ������ ����ȭ ���ۿ� ���� ����
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	float fTerm = vResolution.x / (m_MaxParticle + 1);

	tParticle arrParticle[100] = {};

	for (int i = 0; i < m_MaxParticle; ++i)
	{
		arrParticle[i].vWorldPos = Vec3(-(vResolution.x / 2.f) + (i + 1) * fTerm, 0.f, 100.f);
		arrParticle[i].vWorldScale = Vec3(10.f, 10.f, 1.f);
		arrParticle[i].Active = 1;

		/*if (0 == i)
			arrParticle[i].Active = 1;*/
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, arrParticle);
}

CParticleSystem::~CParticleSystem()
{
	delete m_ParticleBuffer;
}

void CParticleSystem::finaltick()
{	
	// ComputeShader Excecute
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);

	if (FAILED(m_TickCS->Execute()))
	{
		assert(nullptr);
	}
}

void CParticleSystem::render()
{
	m_ParticleBuffer->Binding(17);
	Transform()->Binding();

	GetMaterial()->SetTexParam(TEX_1, m_ParticleTex);
	GetMaterial()->Binding();

	// ��� ���ڸ� �ѹ��� �ν��Ͻ����� ��������
	GetMesh()->render_particle(m_MaxParticle);

	m_ParticleBuffer->Clear_SRV();
}