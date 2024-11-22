#include "pch.h"
#include "CTileMap.h"

#include "CAssetMgr.h"
#include "components.h"

#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_Row(1)
	, m_Col(1)
	, m_TileEachSize(Vec2(32.f, 32.f))
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));
	
	m_TileBuffer = new CStructuredBuffer;
}

CTileMap::~CTileMap()
{
	delete m_TileBuffer;
}

void CTileMap::begin()
{
	SetRowCol(m_Row, m_Col);
}

void CTileMap::finaltick()
{

}

void CTileMap::render()
{
	// 위치정보 
	Transform()->Binding();

	// 재질정보
	GetMaterial()->SetTexParam(TEX_0, m_Atlas);
	GetMaterial()->SetScalarParam(VEC2_0, m_AtlasResolution);
	GetMaterial()->SetScalarParam(VEC2_1, m_AtlasTileEachSize);
	GetMaterial()->SetScalarParam(VEC2_2, Vec2(m_Col, m_Row));
	GetMaterial()->SetScalarParam(INT_0, m_AtlasMaxRow);
	GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxCol);
	GetMaterial()->SetScalarParam(INT_2, 0);
	GetMaterial()->Binding();

	// 타일정보 바인딩
	if (m_TileBuffer->GetElementCount() < m_vecTileInfo.size())
		m_TileBuffer->Create(sizeof(tTileInfo), m_vecTileInfo.size(), SB_TYPE::SRV_ONLY, true, m_vecTileInfo.data());
	else
		m_TileBuffer->SetData(m_vecTileInfo.data(), m_vecTileInfo.size());

	m_TileBuffer->Binding(20);


	// 메시 바인딩 및 렌더링
	GetMesh()->render();
}

void CTileMap::SetRowCol(UINT _Row, UINT _Col)
{
	m_Row = _Row;
	m_Col = _Col;

	Transform()->SetRelativeScale(Vec3(m_TileEachSize.x * m_Col, m_TileEachSize.y * m_Row, 1.f));

	m_vecTileInfo.clear();
	m_vecTileInfo.resize(m_Row * m_Col);

	for (int i = 0; i < m_vecTileInfo.size(); ++i)
	{
		//m_vecTileInfo[i].ImgIdx = i % (m_AtlasMaxRow * m_AtlasMaxCol);
		m_vecTileInfo[i].ImgIdx = 7;
	}
}

void CTileMap::SetTileEachSize(Vec2 _vSize)
{
	m_TileEachSize = _vSize;
	Transform()->SetRelativeScale(Vec3(m_TileEachSize.x * m_Col, m_TileEachSize.y * m_Row, 1.f));
}

void CTileMap::SetAltasTexture(Ptr<CTexture> _Tex)
{
	m_Atlas = _Tex;
	m_AtlasResolution = Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void CTileMap::SetAltasTileSize(Vec2 _TileSize)
{
	assert(m_Atlas.Get());	

	// 아틀라스 텍스처 안에서 타일 1개가 차지하는 크기
	m_AtlasTileEachSize = _TileSize;

	// 아틀라스 텍스쳐 해상도를 타일1개 사이즈로 나누어서 
	// 아틀라트 텍스쳐에 타일이 몇행 몇열 존재하는지 확인
	m_AtlasMaxCol = m_Atlas->GetWidth() / m_AtlasTileEachSize.x;
	m_AtlasMaxRow = m_Atlas->GetHeight() / m_AtlasTileEachSize.y;
}

