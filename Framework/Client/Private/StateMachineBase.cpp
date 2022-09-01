#include "stdafx.h"
#include "..\Public\StateMachineBase.h"
#include "Animation.h"
#include "StateBase.h"
#include "Engine_Defines.h"
CStateMachineBase::CStateMachineBase()
{
}
CStateMachineBase::~CStateMachineBase()
{
	while (!m_States.empty())
	{

		//Safe_Delete(m_States.top());
		m_States.pop();
	}
}


HRESULT CStateMachineBase::Initialize(CStateBase * pStateBase)
{
	return S_OK;
}

HRESULT CStateMachineBase::Update(_float fTimeDelta)
{
	if (m_States.empty())
		return E_FAIL;

	if (!m_States.top()->Loop(fTimeDelta))
	{
		CStateBase*	pState = m_States.top()->Exit();
		//Safe_Delete(m_States.top());
		m_States.pop();

		if (nullptr != pState)
		{
			pState->Enter();
			m_States.push(pState);
		}
	}
	return S_OK;
}

CStateBase * const CStateMachineBase::Get_CurrentState()
{
	return m_States.top();
}

CStateMachineBase * CStateMachineBase::Create(CStateBase * pStateBase)
{
	CStateMachineBase*		pInstance = new CStateMachineBase();

	if (FAILED(pInstance->Initialize(pStateBase)))
	{
		MSG_BOX("StateMachine Error : Initialize");
		//Safe_Delete(pInstance);
		return nullptr;
	}
	
	return pInstance;
}
