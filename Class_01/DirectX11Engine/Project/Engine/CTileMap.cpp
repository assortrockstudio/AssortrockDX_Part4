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
	// ��ġ���� 
	Transform()->Binding();

	// ��������
	GetMaterial()->SetTexParam(TEX_0, m_Atlas);
	GetMaterial()->SetScalarParam(VEC2_0, m_AtlasResolution);
	GetMaterial()->SetScalarParam(VEC2_1, m_AtlasTileEachSize);
	GetMaterial()->SetScalarParam(VEC2_2, Vec2(m_Col, m_Row));
	GetMaterial()->SetScalarParam(INT_0, m_AtlasMaxRow);
	GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxCol);
	GetMaterial()->SetScalarParam(INT_2, 0);
	GetMaterial()->Binding();

	// Ÿ������ ���ε�
	if (m_TileBuffer->GetElementCount() < m_vecTileInfo.size())
		m_TileBuffer->Create(sizeof(tTileInfo), m_vecTileInfo.size(), m_vecTileInfo.data());	
	else
		m_TileBuffer->SetData(m_vecTileInfo.data(), m_vecTileInfo.size());

	m_TileBuffer->Binding(20);


	// �޽� ���ε� �� ������
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

	// ��Ʋ�� �ؽ�ó �ȿ��� Ÿ�� 1���� �����ϴ� ũ��
	m_AtlasTileEachSize = _TileSize;

	// ��Ʋ�� �ؽ��� �ػ󵵸� Ÿ��1�� ������� ����� 
	// ��Ʋ��Ʈ �ؽ��Ŀ� Ÿ���� ���� � �����ϴ��� Ȯ��
	m_AtlasMaxCol = m_Atlas->GetWidth() / m_AtlasTileEachSize.x;
	m_AtlasMaxRow = m_Atlas->GetHeight() / m_AtlasTileEachSize.y;
}
