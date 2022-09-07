#pragma once

#include "StateBase.h"



BEGIN(Client)

class CIdle : public CStateBase
{
private:
	CIdle(class CStudent* pStudent);
	~CIdle();
public:
	// CStateBase을(를) 통해 상속됨
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	virtual CStateBase * Create(CStudent * pStudent) override;


};

END
