#pragma once
#include "CComponent.h"

#include "assets.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;

    Ptr<CMaterial>      m_CurMtrl;      // 현재 사용중인 재질을 참조
    Ptr<CMaterial>      m_SharedMtrl;   // 에셋 매니저에 등록된 재질을 참조
    Ptr<CMaterial>      m_DynamicMtrl;  // 원본 재질의 사본
    
public:
    virtual void render() = 0;

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    Ptr<CMesh> GetMesh() { return m_Mesh; }

    void SetMaterial(Ptr<CMaterial> _Mtrl);
    Ptr<CMaterial> GetMaterial() { return m_CurMtrl; }

    Ptr<CMaterial> GetDynamicMaterial();
    void RestoreMaterial();

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    ~CRenderComponent();
};

