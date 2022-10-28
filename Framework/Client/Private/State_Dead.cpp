#include "stdafx.h"
#include "State_Idle.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Student_Run.h"
#include "GameInstance.h"
#include "Actor.h"
#include "State_Dead.h"



CState_Dead::CState_Dead(CActor * pStudent)
	:CStateBase(pStudent)
{
	strcpy_s(m_szName, "_Vital_Death");
}

HRESULT CState_Dead::Initialize()
{
	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, m_szName);

	m_pAnimation = m_pModel->Get_AnimationFromName(pAnimaitonStr);

	if (nullptr == m_pAnimation)
		return E_FAIL;

	return S_OK;
}

void CState_Dead::Enter()
{
	m_pAnimation->Play();
	m_pOwner->Set_StageState(CActor::STAGE_STATE_DEAD);
}

_bool CState_Dead::Loop(_float fTimeDelta)
{
	if (m_pAnimation->IsFinished())
	{
		m_pAnimation->Reset();
		m_pOwner->Set_Enable(false);
		m_pOwner->Set_Delete(true);
		return true;
	}
	
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	return false;
}

CStateBase * CState_Dead::Exit()
{
	return nullptr;
}

CState_Dead * CState_Dead::Create(CActor * pStudent)
{
	CState_Dead*	pInstance = new CState_Dead(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
