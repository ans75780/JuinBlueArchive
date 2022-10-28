#pragma once

#include "StateBase.h"
#include "State_Dead.h"
BEGIN(Client)

class CState_Tank_Destroy : public CState_Dead
{
private:
	CState_Tank_Destroy(class CActor* pStudent);
	virtual ~CState_Tank_Destroy() = default;

public:
	static  CState_Tank_Destroy * Create(class CActor * pActor);
	virtual _bool Loop(_float fTimeDelta) override;


protected:
	char		m_szName[MAX_PATH];

};

END
