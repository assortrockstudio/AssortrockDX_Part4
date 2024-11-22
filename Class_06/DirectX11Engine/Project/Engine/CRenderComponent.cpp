#include "pch.h"
#include "CRenderComponent.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	// 동적 재질을 세팅받을 수 없다.
	assert(!_Mtrl->IsDynamic());

	m_CurMtrl = m_SharedMtrl = _Mtrl;	
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// 재질 자체를 참조한적이 없다면 nullptr 을 준다.
	if (nullptr == m_SharedMtrl)
		return nullptr;

	// 동적재질을 이전에 생성한 적이 있으면 그걸 준다.
	if (nullptr != m_DynamicMtrl)
		return m_DynamicMtrl;

	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->GetDynamicMaterial();

	return m_DynamicMtrl;
}

void CRenderComponent::RestoreMaterial()
{
	m_CurMtrl = m_SharedMtrl;
	m_DynamicMtrl = nullptr;
}