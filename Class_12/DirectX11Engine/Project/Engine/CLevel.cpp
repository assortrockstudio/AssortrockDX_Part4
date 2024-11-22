#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"


CLevel::CLevel()
	: m_arrLayer{}
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_LayerIdx = i;
	}
}

CLevel::~CLevel()
{
	Safe_Del_Array(m_arrLayer);
}



void CLevel::begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->begin();
	}
}

void CLevel::tick()
{
	// ���� �����ӿ� ��ϵ� ������Ʈ���� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->Clear();
	}


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->finaltick();
	}
}

void CLevel::AddObject(UINT _LayerIdx, CGameObject* _Object, bool _bChildMove)
{
	m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
}
