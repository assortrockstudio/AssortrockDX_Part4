#include "pch.h"
#include "CSetColorShader.h"


CSetColorShader::CSetColorShader()
	: CComputeShader(32, 32, 1)
{
}

CSetColorShader::~CSetColorShader()
{
}

int CSetColorShader::Binding()
{
	if (nullptr == m_TargetTex || nullptr == m_TargetTex->GetUAV())
		return E_FAIL;

	m_TargetTex->Binding_CS_UAV(0);

	m_Const.v4Arr[0] = m_ClearColor;
	m_Const.iArr[0] = (UINT)m_TargetTex->GetWidth();
	m_Const.iArr[1] = (UINT)m_TargetTex->GetHeight();

	return S_OK;
}

void CSetColorShader::CalculateGroupNum()
{
	m_GroupX = (UINT)m_TargetTex->GetWidth() / m_ThreadPerGroupX;
	(UINT)m_TargetTex->GetWidth() % m_ThreadPerGroupX ? m_GroupX += 1 : m_GroupX;

	m_GroupY = (UINT)m_TargetTex->GetHeight() / m_ThreadPerGroupY;
	(UINT)m_TargetTex->GetHeight() % m_ThreadPerGroupY ? m_GroupY += 1 : m_GroupY;

	m_GroupZ = 1;	
}

void CSetColorShader::Clear()
{
	m_TargetTex->Clear_CS_UAV(0);
}
