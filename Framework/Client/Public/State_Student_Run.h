#include "StateBase.h"


namespace Engine
{
	class CTransform;
}


BEGIN(Client)

class CState_Student_Run : public CStateBase
{
private:
	CState_Student_Run(class CStudent* pStudent);
	virtual ~CState_Student_Run() = default;
public:
	// CStateBase��(��) ���� ��ӵ�
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Student_Run * Create(class CStudent * pStudent);

private:
	class CTransform*	m_pTransform = nullptr;
};

END