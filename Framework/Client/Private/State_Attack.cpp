#include "stdafx.h"
#include "..\Public\State_Attack.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Student_Run.h"
#include "GameInstance.h"
#include "Actor.h"



CState_Attack::CState_Attack(CActor * pStudent, CActor * pTarget)
	:CStateBase(pStudent)
{
}

HRESULT CState_Attack::Initialize()
{
	char pAnimaitonStr[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);


	char	AttackStart[MAX_PATH] = "";
	char	AttackIng[MAX_PATH] = "";

	char	AttackDelay[MAX_PATH] = "";
	char	AttackEnd[MAX_PATH] = "";
	char	AttackReload[MAX_PATH] = "";


	strcpy_s(AttackStart, pAnimaitonStr);
	strcat_s(AttackStart, "_Normal_Attack_Start");

	strcpy_s(AttackStart, pAnimaitonStr);
	strcat_s(AttackStart, "_Normal_Attack_Ing");

	strcpy_s(AttackDelay, pAnimaitonStr);
	strcat_s(AttackDelay, "_Normal_Attack_Delay");
	
	strcpy_s(AttackReload, pAnimaitonStr);
	strcat_s(AttackReload, "_Normal_Attack_Reload");

	strcpy_s(AttackEnd, pAnimaitonStr);
	strcat_s(AttackEnd, "_Normal_Attack_End");

	
	if (nullptr == (m_pAnimation_Attack_Start = m_pOwner->Get_Animation(AttackStart)))
		return E_FAIL;

	if (nullptr == (m_pAnimation_Attack_Ing= m_pOwner->Get_Animation(AttackIng)))
		return E_FAIL;

	if (nullptr == (m_pAnimation_Attack_Delay= m_pOwner->Get_Animation(AttackDelay)))
		return E_FAIL;

	if (nullptr == (m_pAnimation_Attack_Reload= m_pOwner->Get_Animation(AttackReload)))
		return E_FAIL;
	
	if (nullptr == (m_pAnimation_Attack_End = m_pOwner->Get_Animation(AttackEnd)))
		return E_FAIL;


	return S_OK;
}

void CState_Attack::Enter()
{
	__super::Enter();
}

_bool CState_Attack::Loop(_float fTimeDelta)
{








	return false;
}

CStateBase * CState_Attack::Exit()
{
	return nullptr;
}

CState_Attack * CState_Attack::Create(CActor * pActor, class CActor* pTarget)
{
	CState_Attack*	pInstance = new CState_Attack(pActor, pTarget);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
