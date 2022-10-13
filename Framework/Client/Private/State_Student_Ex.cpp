#include "stdafx.h"
#include "State_Student_Ex.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Run.h"
#include "GameInstance.h"
#include "Actor.h"
CState_Student_Ex::CState_Student_Ex(CActor * pStudent)
	:CStateBase(pStudent)
{
}

HRESULT CState_Student_Ex::Initialize()
{
	char pAnimaitonStr[MAX_PATH];
	char pExCutinAnimaitonStr[MAX_PATH];
	char pExAnimaitonStr[MAX_PATH];



	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcpy_s(pExAnimaitonStr, pAnimaitonStr);
	strcpy_s(pExCutinAnimaitonStr, pAnimaitonStr);


	strcat_s(pExAnimaitonStr, "_Exs");
	strcat_s(pExCutinAnimaitonStr, "_Exs_Cutin");

	m_pExCutin = m_pModel->Get_AnimationFromName(pExCutinAnimaitonStr);
	m_pEx = m_pModel->Get_AnimationFromName(pExAnimaitonStr);


	if (nullptr == m_pExCutin || nullptr == m_pEx)
		return E_FAIL;

	m_pAnimation = m_pExCutin;

	m_pOwner->Set_StageState(CActor::STATE_STATE_EX);

	return S_OK;
}

void CState_Student_Ex::Enter()
{
	m_pAnimation->Play();
}

_bool CState_Student_Ex::Loop(_float fTimeDelta)
{
	if (m_pExCutin->IsFinished() && m_pEx->IsFinished())
	{
		m_pExCutin->Reset();
		m_pEx->Reset();
		return true;
	}
	if (m_pExCutin->IsFinished())
	{
		m_pExCutin->Stop();
		m_pEx->Play();
		m_pAnimation = m_pEx;
	}
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	
	return false;
}

CStateBase * CState_Student_Ex::Exit()
{
	return nullptr;
}

CState_Student_Ex * CState_Student_Ex::Create(CActor * pStudent)
{
	CState_Student_Ex*	pInstance = new CState_Student_Ex(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
