#pragma once
#include "CEntity.h"

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;           // 버퍼
    D3D11_BUFFER_DESC                   m_Desc;         // 버퍼정보 구조체
    ComPtr<ID3D11ShaderResourceView>    m_SRV;          // Shader Resource View

    UINT                                m_ElementSize;  // 버퍼 각 요소 크기
    UINT                                m_ElementCount; // 버퍼의 요소 개수

public:
    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }

    int Create(UINT _ElementSize, UINT _ElementCount, void* _InitialData = nullptr);
    void SetData(void* _pData, UINT _ElementCount = 0);
    void Binding(UINT _RegisterNum);
   
public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

