#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Idle : public CStateBase
{
protected:
	CState_Idle(class CActor* pStudent);
	virtual ~CState_Idle() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Idle * Create(class CActor * pActor);

protected:
	class CCollider*	m_pBodyCollider = nullptr;
	class CCollider*	m_pAttackRangeCollider = nullptr;

};

END
