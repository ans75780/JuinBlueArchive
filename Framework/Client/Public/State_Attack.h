#pragma once

#include "StateBase.h"


namespace Engine
{
	class CAnimation;
	class CModel;
	class CGameInstance;
}

BEGIN(Client)

class CState_Attack: public CStateBase
{
	enum ATTACK_STATE
	{
		ATTACK_STATE_START, ATTACK_STATE_ING, ATTACK_STATE_DELAY, ATTACK_STATE_END, ATTACK_STATE_RELOAD, ATTACK_STATE_ENUM_END,
	};

private:
	CState_Attack(class CActor* pStudent, class CActor* pTarget);
	virtual ~CState_Attack() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;

public:
	virtual class CAnimation*	Get_Animation();



protected:
	class CActor*	m_pTarget = nullptr;
	class CGameInstance*	m_pInstance = nullptr;
private:
	void				Change_Animation(ATTACK_STATE	eState);
	void				Create_Effect();
private:
	class CAnimation*		m_pAnims[ATTACK_STATE_ENUM_END];
	ATTACK_STATE			m_eCurrentState = ATTACK_STATE_ENUM_END;


public:
	static  CState_Attack * Create(class CActor * pActor, class CActor* pTarget);

};

END
