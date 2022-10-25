#pragma once
#include "StateBase.h"

namespace Engine
{
	class CTransform;
	class CCollider;
}


BEGIN(Client)

class CState_Run : public CStateBase
{
protected:
	CState_Run(class CActor* pStudent);
	virtual ~CState_Run() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() ;
	virtual void Enter() ;
	virtual _bool Loop(_float fTimeDelta) ;
	virtual CStateBase * Exit() ;
	static  CState_Run * Create(class CActor * pStudent);

protected:
	class CTransform*	m_pTransform = nullptr;
};

END