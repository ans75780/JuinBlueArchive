#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Attack: public CStateBase
{
	enum ATTACK_STATE
	{
		ATTACK_START, ATTACK_ING, ATTACK_DELAY, ATTACK_END, ATTACK_RELOAD, ATTACK_END
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


protected:
	class CGameObject*	m_pTarget = nullptr;
	class CAnimation*	m_pAnimation_Attack_Start = nullptr;
	class CAnimation*	m_pAnimation_Attack_Ing = nullptr;
	class CAnimation*	m_pAnimation_Attack_Delay = nullptr;
	class CAnimation*	m_pAnimation_Attack_End = nullptr;
	class CAnimation*	m_pAnimation_Attack_Reload= nullptr;


	vector<CAnimation*>	m_vecAnimVector;
	_uint				m_iVectorIndex;
public:
	static  CState_Attack * Create(class CActor * pActor, class CActor* pTarget);

};

END
