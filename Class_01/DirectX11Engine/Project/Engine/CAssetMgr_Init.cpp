#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"

void CAssetMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}

void CAssetMgr::CreateDefaultMesh()
{
	Ptr<CMesh> pMesh = nullptr;

	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// =========
	// Rect Mesh
	// =========
	// 0 -- 1
	// | \  |
	// 3 -- 2
	v.vPos = Vec3(-0.5, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	// Index Buffer
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"RectMesh", pMesh);
		
	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"RectMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Circle Mesh
	// ===========
	// 원점을 vector 에 넣는다.
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Radius = 0.5f;
	UINT Slice = 60;
	float AngleStep = (2 * XM_PI) / Slice;

	float Angle = 0.f;
	for (UINT i = 0; i <= Slice; ++i)
	{
		v.vPos = Vec3(cosf(Angle) * Radius, sinf(Angle) * Radius, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Angle += AngleStep;
	}

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"CircleMesh", pMesh);
	

	vecIdx.clear();
	for (int i = 0; i <= (int)Slice; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CAssetMgr::CreateDefaultTexture()
{
	Load<CTexture>(L"texture\\Character.png", L"texture\\Character.png");
	Load<CTexture>(L"texture\\Fighter.bmp", L"texture\\Fighter.bmp");	
}


void CAssetMgr::CreateDefaultGraphicShader()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	Ptr<CGraphicShader> pShader = nullptr;
	
	// ===========
	// Std2DShader
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(strPath + L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset<CGraphicShader>(L"Std2DShader", pShader);


	// =======================
	// Std2D AlphaBlend Shader
	// =======================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(strPath + L"shader\\std2d.fx", "PS_Std2D_AB");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset<CGraphicShader>(L"Std2DAlphaBlendShader", pShader);

	// =============
	// TileMapShader
	// =============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(strPath + L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);	
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset<CGraphicShader>(L"TileMapShader", pShader);

	// ================
	// DebugShapeShader
	// ================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\debug_shape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(strPath + L"shader\\debug_shape.fx", "PS_DebugShape");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddAsset<CGraphicShader>(L"DebugShapeShader", pShader);
}


#include "CSetColorShader.h"

void CAssetMgr::CreateDefaultComputeShader()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();

	Ptr<CComputeShader> pComputeShader = nullptr;

	pComputeShader = new CSetColorShader;
	pComputeShader->CreateComputeShader(strPath + L"shader\\compute.fx", "CS_Test");
	AddAsset<CComputeShader>(L"SetColorCS", pComputeShader);
}



void CAssetMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// BackgroundMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"BackgroundMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"TileMapMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// DebugShapeMtrl
	pMtrl = new CMaterial;
	pMtrl->SetName(L"DebugShapeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);	
}