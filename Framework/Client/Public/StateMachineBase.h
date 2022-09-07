#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include <stack>
#include "Base.h"
namespace Engine
{
	class CAnimation;
}
BEGIN(Client)

class CStateMachineBase : public CBase
{
private:
	CStateMachineBase(class CStudent* pStudent);
	virtual ~CStateMachineBase() = default;

public:
	HRESULT		Initialize();
	HRESULT		Update(_float fTimeDelta);


public:
	HRESULT		Setup_StateMachine(class CStateBase* pState);
	void		Clear();

public:
	class CStateBase* const	Get_CurrentState();
private:
	class CStudent*				m_pStudent = nullptr;
	stack<class CStateBase*>	m_States;


public:
	static CStateMachineBase*		Create(class CStudent* pStudent);
	void							Free();
};

END
