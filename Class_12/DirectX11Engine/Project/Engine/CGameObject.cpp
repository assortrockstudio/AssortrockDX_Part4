#include "pch.h"
#include "CGameObject.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CComponent.h"
#include "CRenderComponent.h"
#include "CScript.h"

#include "CTaskMgr.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
{
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vector(m_vecScripts);
	Safe_Del_Vector(m_vecChild);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->begin();
	}


	// 자식 오브젝트 begin 호출
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->tick();
	}

	// 자식 오브젝트 tick 호출
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	// Component finaltick 호출
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	// 소속 Layer에 자신을 등록
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(m_LayerIdx); 
	pLayer->RegisterObject(this);


	// 자식 오브젝트 finaltick 호출
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CGameObject::render()
{
	if (m_RenderCom)
	{
		m_RenderCom->render();
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	// 입력으로 들어오는 컴포넌트의 타입을 확인한다.
	COMPONENT_TYPE type = _Component->GetComponentType();

	// 입력된 컴포넌트가 Script 종류인 경우
	if (COMPONENT_TYPE::SCRIPT == type)
	{
		m_vecScripts.push_back((CScript*)_Component);
	}

	else
	{
		// 입력으로 들어온 컴포넌트를 이미 가지고 있는 경우
		assert(!m_arrCom[(UINT)type]);

		// 입력된 컴포넌트가 렌더링 관련 컴포넌트인지 확인
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
		if (nullptr != pRenderCom)
		{
			// 이미 렌더링 관련 컴포넌트를 보유한 경우
			assert(!m_RenderCom);

			m_RenderCom = pRenderCom;
		}

		// 입력된 컴포넌트를 배열의 알맞은 인덱스 자리에 주소값을 기록한다.
		m_arrCom[(UINT)type] = _Component;
	}

	// 컴포넌트의 소유 오브젝트가 본인임을 알림
	_Component->m_Owner = this;
}


void CGameObject::AddChild(CGameObject* _Object)
{
	_Object->m_Parent = this;
	m_vecChild.push_back(_Object);
}

void CGameObject::Destroy()
{
	tTask task = {};

	task.Type = TASK_TYPE::DESTROY_OBJECT;
	task.dwParam_0 = (DWORD_PTR)this;

	CTaskMgr::GetInst()->AddTask(task);
}