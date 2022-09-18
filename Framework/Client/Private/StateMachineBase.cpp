#include "stdafx.h"
#include "..\Public\StateMachineBase.h"
#include "Animation.h"
#include "StateBase.h"
#include "Engine_Defines.h"
#include "Student.h"

CStateMachineBase::CStateMachineBase(CStudent * pStudent)
	:m_pStudent(pStudent)
{
	Safe_AddRef(m_pStudent);
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
	}
	return S_OK;
}

HRESULT CStateMachineBase::Setup_StateMachine(CStateBase * pState)
{
	if (nullptr == pState)
		return E_FAIL;
	Clear();

	m_States.push(pState);

	return S_OK;
}

HRESULT CStateMachineBase::Add_State(CStateBase * pState)
{
	if (nullptr == pState)
		return E_FAIL;

	m_States.push(pState);

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

CStateMachineBase * CStateMachineBase::Create(CStudent * pStudent)
{
	CStateMachineBase*		pInstance = new CStateMachineBase(pStudent);

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
	Safe_Release(m_pStudent);
}
