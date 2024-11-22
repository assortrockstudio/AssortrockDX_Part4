#include "pch.h"
#include "CParticleSystem.h"

#include "CTransform.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_Time(0.f)
	, m_MaxParticle(1000)
{
	// Particle Tick �뵵 ComputeShader 
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleCS").Get();

	// ParticleSystem �� ����� �޽ÿ� ��Ƽ���� ����
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl"));

	// ��ƼŬ ������ ������ ����ȭ ���� ����	
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, nullptr);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true);

	// ��ƼŬ ���(���) ���� ����
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.SpawnRate = 20;
	m_Module.vSpawnColor = Vec3(1.f, 0.f, 0.f);
	m_Module.MinLife = 1.f;
	m_Module.MaxLife = 4.f;
	m_Module.vSpawnMinScale = Vec3(1.f, 1.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(20.f, 20.f, 1.f);

	m_Module.SpawnShape = 1;
	m_Module.SpawnShapeScale = Vec3(50.f, 50.f, 50.f);




	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	m_Module.SpawnBurstRepeat = true;
	m_Module.SpawnBurstCount = 100;
	m_Module.SpawnBurstRepeatTime = 3.f;



	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule) + (16 - sizeof(tParticleModule) % 16), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::~CParticleSystem()
{
	delete m_ParticleBuffer;
	delete m_SpawnCountBuffer;
	delete m_ModuleBuffer;
}


void CParticleSystem::finaltick()
{	
	// SpawnCount ���
	CaculateSpawnCount();

	// ComputeShader Excecute
	m_TickCS->SetParticleWorldPos(Transform()->GetWorldPos());
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	

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



void CParticleSystem::CaculateSpawnCount()
{
	m_Time += DT;
	tSpawnCount count = {  };

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		// SpawnRate �� ���� �̹� Tick ���� ������ų ��ƼŬ�� ��
		float Term = 1.f / (float)m_Module.SpawnRate;
		UINT SpawnCount = 0;

		if (Term < m_Time)
		{
			float Value = m_Time / Term;
			SpawnCount = (UINT)Value;
			m_Time -= (float)SpawnCount * Term;
		}

		count.SpawnCount = SpawnCount;
	}
	
	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{	
		UINT BurstCount = 0;

		if (0.f == m_BurstTime)
		{
			BurstCount = m_Module.SpawnBurstCount;
		}		
		
		m_BurstTime += DT;

		if (m_Module.SpawnBurstRepeat && m_Module.SpawnBurstRepeatTime <= m_BurstTime)
		{
			m_BurstTime -= m_Module.SpawnBurstRepeatTime;
			BurstCount += m_Module.SpawnBurstCount;
		}

		count.SpawnCount += BurstCount;
	}	

	// SpawnCount �� Buffer �� ����	
	m_SpawnCountBuffer->SetData(&count);
}