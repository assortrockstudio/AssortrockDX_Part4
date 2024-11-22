#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_CurAnim(nullptr)
	, m_Repeat(false)
{

}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr == m_CurAnim)
		return;

	if (m_Repeat && m_CurAnim->IsFinish())
	{
		m_CurAnim->Reset();
	}

	m_CurAnim->finaltick();
}

void CAnimator2D::Binding()
{
	if(nullptr != m_CurAnim)
		m_CurAnim->Binding();
}


void CAnimator2D::CreateAnimation(const wstring& _AnimName, Ptr<CTexture> _AltasTex
							, Vec2 _LeftTopPixelPos, Vec2 _vSlicePixelSize, Vec2 _BackgroundSize
						    , int _FrameCount, UINT _FPS)
{
	assert(!FindAnimation(_AnimName));

	CAnim2D* pAnim = new CAnim2D;
	pAnim->SetName(_AnimName);
	pAnim->Create(_AltasTex, _LeftTopPixelPos, _vSlicePixelSize, _BackgroundSize, _FrameCount, _FPS);

	pAnim->m_Animator = this;
	m_mapAnim.insert(make_pair(_AnimName, pAnim));
}

CAnim2D* CAnimator2D::FindAnimation(const wstring& _AnimName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_AnimName);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::LoadAnimation(const wstring& _strRelativePath)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Load(_strRelativePath);

	pAnim->m_Animator = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _Repeat)
{
	m_CurAnim = FindAnimation(_strAnimName);
	m_Repeat = _Repeat;
}
