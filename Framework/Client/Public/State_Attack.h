#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Attack: public CStateBase
{
private:
	CState_Attack(class CActor* pStudent);
	virtual ~CState_Attack() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Attack * Create(class CActor * pActor);

};

END
