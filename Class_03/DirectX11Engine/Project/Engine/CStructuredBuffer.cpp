#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_Desc{}
    , m_ElementSize(0)
    , m_ElementCount(0)
    , m_Type(SB_TYPE::SRV_ONLY)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove, void* _InitialData)
{
    // 기존에 가리키던 버퍼 및 SRV 를 가리키지 않는다.
    m_SB_Main = nullptr;
    m_SRV = nullptr;
    m_Type = _Type;

    // 버퍼의 크기를 16의 배수로 설정해야한다.
    assert(!(_ElementSize % 16));

    m_ElementSize = _ElementSize;
    m_ElementCount = _ElementCount;

    m_Desc.ByteWidth = m_ElementSize * m_ElementCount;

    m_Desc.Usage = D3D11_USAGE_DYNAMIC;
    m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (SB_TYPE::SRV_ONLY == m_Type)
    {
        m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    }
    else
    {
        m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    }

    m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    m_Desc.StructureByteStride = _ElementSize;

    HRESULT hr = E_FAIL;

    if (_InitialData)
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = _InitialData;
        hr = DEVICE->CreateBuffer(&m_Desc, &tSub, m_SB_Main.GetAddressOf());
    }    
    else
    {
        hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_SB_Main.GetAddressOf());
    }

    if (FAILED(hr))
        return E_FAIL;

    // ShaderResourceView 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC tViewDesc = {};
    tViewDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
    tViewDesc.BufferEx.NumElements = m_ElementCount;
    hr = DEVICE->CreateShaderResourceView(m_SB_Main.Get(), &tViewDesc, m_SRV.GetAddressOf());

    // Unordered AccessView 생성
    if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC tViewDesc = {};
        tViewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        tViewDesc.Buffer.NumElements = m_ElementCount;
        DEVICE->CreateUnorderedAccessView(m_SB_Main.Get(), &tViewDesc, m_UAV.GetAddressOf());
    }

    return hr;
}

void CStructuredBuffer::SetData(void* _pData, UINT _ElementCount)
{
    if (!_ElementCount)
    {
        _ElementCount = m_ElementCount;
    }

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Main.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

    memcpy(tSub.pData, _pData, _ElementCount * m_ElementSize);

    CONTEXT->Unmap(m_SB_Main.Get(), 0);
}

void CStructuredBuffer::Binding(UINT _RegisterNum)
{    
    CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Binding_CS_SRV(UINT _RegisterNum)
{
    CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Binding_CS_UAV(UINT _RegisterNum)
{
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}
