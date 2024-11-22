#pragma once
#include "CEntity.h"

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;           // ����
    D3D11_BUFFER_DESC                   m_Desc;         // �������� ����ü
    ComPtr<ID3D11ShaderResourceView>    m_SRV;          // Shader Resource View

    UINT                                m_ElementSize;  // ���� �� ��� ũ��
    UINT                                m_ElementCount; // ������ ��� ����

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

