#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Dead: public CStateBase
{
private:
	CState_Dead(class CActor* pStudent);
	virtual ~CState_Dead() = default;
public:
	// CStateBase��(��) ���� ��ӵ�
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Dead * Create(class CActor * pActor);

};

END
