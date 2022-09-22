#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Student_Ex : public CStateBase
{
private:
	CState_Student_Ex(class CStudent* pStudent);
	virtual ~CState_Student_Ex() = default;
public:
	// CStateBase��(��) ���� ��ӵ�
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Student_Ex * Create(class CStudent * pStudent);

};

END
