#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include <stack>

namespace Engine
{
	class CAnimation;
}
BEGIN(Client)

class CStateMachineBase
{
private:
	CStateMachineBase();
	virtual ~CStateMachineBase();

public:
	HRESULT		Initialize(class CStateBase* pStateBase);
	HRESULT		Update(_float fTimeDelta);

public:
	class CStateBase* const	Get_CurrentState();

private:
	stack<class CStateBase*>	m_States;

public:
	CStateMachineBase*		Create(class CStateBase* pStateBase);

};

END
