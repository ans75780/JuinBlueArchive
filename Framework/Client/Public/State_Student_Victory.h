#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Student_Victory : public CStateBase
{
protected:
	CState_Student_Victory(class CActor* pStudent);
	virtual ~CState_Student_Victory() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Student_Victory * Create(class CActor * pActor);

protected:
	CAnimation*		m_pVictoryStart = nullptr;
	CAnimation*		m_pVictoryEnd = nullptr;


};

END
