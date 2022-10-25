#include "stdafx.h"
#include "..\Public\StateMachineBase.h"
#include "Animation.h"
#include "StateBase.h"
#include "Engine_Defines.h"
#include "Student.h"

#include "Actor.h"

CStateMachineBase::CStateMachineBase(CActor * pOwner)
	:m_pOwner(pOwner)
{
	Safe_AddRef(m_pOwner);
}

HRESULT CStateMachineBase::Initialize()
{
	return S_OK;
}

HRESULT CStateMachineBase::Update(_float fTimeDelta)
{
	if (m_States.empty())
		return E_FAIL;

	if (m_States.top()->Loop(fTimeDelta))
	{
		CStateBase*	pState = m_States.top()->Exit();
		//현재 리턴을 받았는데, 상태가 죽어있다 -> 죽은 상태가 끝났다.
		//그러면 스테이트머신을 날리고 나감.
		if (m_pOwner->Get_StageState() == CActor::STAGE_STATE_DEAD)
		{
			Clear();
			return S_OK;
		}
		//가장 마지막 상태는 지워지지않음.
		if (m_States.size() > 1)
		{ 
			Safe_Release(m_States.top());
			m_States.pop();
		}
		if (nullptr != pState)
		{
			pState->Enter();
			m_States.push(pState);
		}
		else
			m_States.top()->Enter();
	}
	return S_OK;
}

HRESULT CStateMachineBase::Setup_StateMachine(CStateBase * pState)
{
	if (nullptr == pState)
		return E_FAIL;
	Clear();

	m_States.push(pState);
	pState->Enter();
	return S_OK;
}

HRESULT CStateMachineBase::Add_State(CStateBase * pState)
{
	if (nullptr == pState)
		return E_FAIL;

	m_States.push(pState);
	pState->Enter();
	return S_OK;
}

void CStateMachineBase::Clear()
{
	while (!m_States.empty())
	{
		Safe_Release(m_States.top());
		m_States.pop();
	}
}

CStateBase * const CStateMachineBase::Get_CurrentState()
{
	return m_States.top();
}

CStateMachineBase * CStateMachineBase::Create(CActor * pOwner)
{
	CStateMachineBase*		pInstance = new CStateMachineBase(pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("StateMachine Error : Initialize");
		//Safe_Delete(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStateMachineBase::Free()
{
	while (!m_States.empty())
	{
		Safe_Release(m_States.top());
		m_States.pop();
	}
	Safe_Release(m_pOwner);
}
