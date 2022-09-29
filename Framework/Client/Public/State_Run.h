#include "StateBase.h"


namespace Engine
{
	class CTransform;
}


BEGIN(Client)

class CState_Run : public CStateBase
{
private:
	CState_Run(class CActor* pStudent);
	virtual ~CState_Run() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Run * Create(class CActor * pStudent);

private:
	class CTransform*	m_pTransform = nullptr;
};

END