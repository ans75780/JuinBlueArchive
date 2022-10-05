#pragma once
#include "StateBase.h"


BEGIN(Client)

class CState_Student_Jump : public CStateBase
{
protected:
	CState_Student_Jump(class CActor* pActor);
	virtual ~CState_Student_Jump() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize();
	virtual void Enter();
	virtual _bool Loop(_float fTimeDelta);
	virtual CStateBase * Exit();
	static  CStateBase * Create(class CActor * pStudent);

private:
	_float			m_fOriginSpeed;
};

END

