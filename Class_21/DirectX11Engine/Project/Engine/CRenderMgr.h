#pragma once

#include "CTexture.h"

class CCamera;
class CLight2D;
class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>    m_vecCam;
    vector<CLight2D*>   m_vecLight2D;
    CStructuredBuffer*  m_Light2DBuffer;

    Ptr<CTexture>       m_RenderTargetCopyTex;

public:
    void RegisterCamera(CCamera* _Cam, int _Priority);
    int RegisterLight2D(CLight2D* _Light2D)
    {
        m_vecLight2D.push_back(_Light2D);
        return (int)m_vecLight2D.size() - 1;
    }

    void CopyRenderTarget();


public:
    void init();
    void tick();
    void render();

private:
    void DataBinding();
    void DataClear();
};

