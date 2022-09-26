#include "StateBase.h"


namespace Engine
{
	class CTransform;
}


BEGIN(Client)

class CState_Student_Default: public CStateBase
{
private:
	CState_Student_Default(class CActor* pStudent, _tchar*	AnimationName);
	virtual ~CState_Student_Default() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Student_Default * Create(class CActor * pStudent, _tchar*	AnimationName);

private:
	class CTransform*	m_pTransform = nullptr;
};

END