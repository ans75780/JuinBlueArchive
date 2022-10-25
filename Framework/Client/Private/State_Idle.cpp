#include "stdafx.h"
#include "State_Idle.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Student_Run.h"
#include "GameInstance.h"
#include "Actor.h"



CState_Idle::CState_Idle(CActor * pStudent)
	:CStateBase(pStudent)
{
}

HRESULT CState_Idle::Initialize()
{

	char pAnimaitonStr[MAX_PATH];
	
	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Normal_Idle");

	 m_pAnimation =  m_pModel->Get_AnimationFromName(pAnimaitonStr);

	 if (nullptr == m_pAnimation)
		 return E_FAIL;

	 m_pBodyCollider = (CCollider*)m_pOwner->Get_Component(L"Com_BodyCollider");
	 m_pAttackRangeCollider = (CCollider*)m_pOwner->Get_Component(L"Com_AttackRangeCollider");

	return S_OK;
}

void CState_Idle::Enter()
{
	m_pAnimation->Play();
	m_pOwner->Set_StageState(CActor::STATE_STATE_IDLE);
}

_bool CState_Idle::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	

	return false;
}

CStateBase * CState_Idle::Exit()
{
	return CState_Student_Run::Create(m_pOwner);
}

CState_Idle * CState_Idle::Create(CActor * pStudent)
{
	CState_Idle*	pInstance = new CState_Idle(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
