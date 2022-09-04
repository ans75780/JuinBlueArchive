#include "stdafx.h"
#include "State_Student_Idle.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
CState_Student_Idle::CState_Student_Idle(CStudent * pStudent)
	:CStateBase(pStudent)
{
}

HRESULT CState_Student_Idle::Initialize()
{


	char pAnimaitonStr[MAX_PATH];
	
	WideCharToMultiByte(CP_ACP, 0, m_pStudent->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Original_Stand_Idle");

	 m_pAnimation =  m_pModel->Get_AnimationFromName(pAnimaitonStr);

	 if (nullptr == m_pAnimation)
		 return E_FAIL;


	 m_pAnimation->Play();

	return S_OK;
}

void CState_Student_Idle::Enter()
{
}

_bool CState_Student_Idle::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	return false;
}

CStateBase * CState_Student_Idle::Exit()
{
	return nullptr;
}

CStateBase * CState_Student_Idle::Create(CStudent * pStudent)
{
	CState_Student_Idle*	pInstance = new CState_Student_Idle(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
