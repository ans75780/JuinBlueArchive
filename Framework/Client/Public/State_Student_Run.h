#pragma once
#include "State_Run.h"


BEGIN(Client)

class CState_Student_Run : public CState_Run
{
protected:
	CState_Student_Run(class CActor* pActor);
	virtual ~CState_Student_Run() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() ;
	virtual void Enter() ;
	virtual _bool Loop(_float fTimeDelta) ;
	virtual CStateBase * Exit() ;
	static  CStateBase * Create(class CActor * pStudent);
	
private:
	class CCollider*	m_pBodyCollider = nullptr;
	class CCollider*	m_pAttackRangeCollider = nullptr;

};

END

