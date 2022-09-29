#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Idle : public CStateBase
{
private:
	CState_Idle(class CActor* pStudent);
	virtual ~CState_Idle() = default;
public:
	// CStateBase��(��) ���� ��ӵ�
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Idle * Create(class CActor * pActor);

};

END
