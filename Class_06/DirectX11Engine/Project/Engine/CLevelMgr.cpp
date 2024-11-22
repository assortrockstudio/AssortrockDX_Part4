#include "pch.h"
#include "CLevelMgr.h"

#include "CAssetMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"
#include "CAnim2D.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CCollisionMgr.h"
#include "CSetColorShader.h"

#include "CStructuredBuffer.h"


CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
	{
		delete m_CurLevel;
	}	
}

void CLevelMgr::init()
{
	// 텍스쳐 생성하기
	Ptr<CTexture> pTestTex = 
		CAssetMgr::GetInst()->CreateTexture(L"TestTex"
			, 1000, 1000, DXGI_FORMAT_R8G8B8A8_UNORM
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	// 컴퓨트 쉐이더 테스트하기
	Ptr<CSetColorShader> pCS = (CSetColorShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"SetColorCS").Get();
	pCS->SetTargetTexture(pTestTex);
	pCS->SetClearColor(Vec3(0.f, 0.f, 1.f));
	pCS->Execute();

	// 구조화버퍼 데이터 이동 테스트
	CStructuredBuffer* pTestBuffer = new CStructuredBuffer;
	pTestBuffer->Create(sizeof(tParticle), 1, SB_TYPE::SRV_UAV, true, nullptr);

	tParticle particle = {};
	particle.Life = 10.f;
	particle.vWorldPos = Vec3(100.f, 100.f, 100.f);
	pTestBuffer->SetData(&particle);

	tParticle particle1 = {};
	pTestBuffer->GetData(&particle1);

	delete pTestBuffer;


	// 임시 레벨 설정
	m_CurLevel = new CLevel;
	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Player");
	m_CurLevel->GetLayer(2)->SetName(L"Monster");

	// Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->SetCameraPriority(0); // 메인 카메라로 설정
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	
	m_CurLevel->AddObject(0, pCamObj);

	// 광원 오브젝트 생성
	CGameObject* pLightObj = new CGameObject;
	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	
	m_CurLevel->AddObject(0, pLightObj);

	// Background Object
	CGameObject* pBackground = new CGameObject;
	pBackground->AddComponent(new CTransform);
	pBackground->AddComponent(new CMeshRender);

	pBackground->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1000.f));
	pBackground->Transform()->SetRelativeScale(Vec3(1600.f, 900.f, 1.f));

	pBackground->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pBackground->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMtrl"));
	pBackground->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.jpg", L"texture\\Background.jpg"));

	m_CurLevel->AddObject(0, pBackground, false);


	// Particle Object
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"Particle");
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));

	m_CurLevel->AddObject(0, pParticleObj, false);


	// Level 의 충돌설정
	CCollisionMgr::GetInst()->LayerCheck(1, 1);
	CCollisionMgr::GetInst()->LayerCheck(2, 1);	

	// 레벨 시작
	m_CurLevel->begin();
}

void CLevelMgr::tick()
{
	if(nullptr != m_CurLevel)	
	{
		m_CurLevel->tick();
		m_CurLevel->finaltick();
	}
}