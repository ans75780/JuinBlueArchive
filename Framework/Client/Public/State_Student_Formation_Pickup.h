#include "StateBase.h"


namespace Engine
{
	class CTransform;
}


BEGIN(Client)

class CState_Student_Formation_Pickup : public CStateBase
{
private:
	CState_Student_Formation_Pickup(class CActor* pStudent);
	virtual ~CState_Student_Formation_Pickup() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Student_Formation_Pickup * Create(class CActor * pStudent);

private:
	class CTransform*	m_pTransform = nullptr;


};

END