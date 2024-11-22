#include "pch.h"
#include "CAssetMgr.h"

#include "CTexture.h"


CAssetMgr::CAssetMgr()
{

}

CAssetMgr::~CAssetMgr()
{

}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
									 , DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture;
	pTex->Create(_Width, _Height, _Format, _BindFlag, _Usage);

	pTex->m_Key = _strKey;
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.Get()));

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture;
	pTex->Create(_Tex2D);

	pTex->m_Key = _strKey;
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.Get()));

	return pTex;
}
