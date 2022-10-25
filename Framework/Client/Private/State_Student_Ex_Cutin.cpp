#include "stdafx.h"
#include "State_Student_Ex_Cutin.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Run.h"
#include "GameInstance.h"
#include "Actor.h"
CState_Student_Ex_Cutin::CState_Student_Ex_Cutin(CActor * pStudent)
	:CStateBase(pStudent)
{
}

HRESULT CState_Student_Ex_Cutin::Initialize()
{
	char pAnimaitonStr[MAX_PATH];
	char pExCutinAnimaitonStr[MAX_PATH];
	

	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);
	
	strcpy_s(pExCutinAnimaitonStr, pAnimaitonStr);

	strcat_s(pExCutinAnimaitonStr, "_Exs_Cutin");

	m_pExCutin = m_pModel->Get_AnimationFromName(pExCutinAnimaitonStr);
	


	if (nullptr == m_pExCutin)
		return E_FAIL;

	m_pAnimation = m_pExCutin;

	m_pOwner->Set_StageState(CActor::STATE_STATE_EX);

	return S_OK;
}

void CState_Student_Ex_Cutin::Enter()
{
	m_pAnimation->Play();
}

_bool CState_Student_Ex_Cutin::Loop(_float fTimeDelta)
{
	if (m_pExCutin->IsFinished())
	{
		m_pExCutin->Reset();
		return true;
	}
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();

	return false;
}

CStateBase * CState_Student_Ex_Cutin::Exit()
{
	return nullptr;
}

CState_Student_Ex_Cutin * CState_Student_Ex_Cutin::Create(CActor * pStudent)
{
	CState_Student_Ex_Cutin*	pInstance = new CState_Student_Ex_Cutin(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
