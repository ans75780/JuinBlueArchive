#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

namespace Engine
{
	class CAnimation;
}

BEGIN(Client)

class CStateBase
{
public:
	enum class STATE_TYPE{ONCE, LOOP, NONE};
private:
		CStateBase(class CAnimation* pAnimation, STATE_TYPE eType);
virtual	~CStateBase();

public:
	virtual void		Enter();
	virtual _bool		Loop(_float fTimeDelta);
	virtual CStateBase*	Exit();
public:
	class CAnimation*	Get_Animation() { return m_pAnimation; }
protected:
	_uint				m_iAnimationIndex = 0;
	class CAnimation*	m_pAnimation = nullptr;
	STATE_TYPE			m_eStateType;

public:
	CStateBase*			Create(class CAnimation* pAnimation, STATE_TYPE eType);

};
END