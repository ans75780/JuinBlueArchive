#include "stdafx.h"
#include "State_Student_Ex.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Student_Run.h"
#include "GameInstance.h"
CState_Student_Ex::CState_Student_Ex(CStudent * pStudent)
	:CStateBase(pStudent)
{
}

HRESULT CState_Student_Ex::Initialize()
{
	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, m_pStudent->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Original_Exs_Cutin");

	m_pAnimation = m_pModel->Get_AnimationFromName(pAnimaitonStr);

	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_pAnimation->Play();

	return S_OK;
}

void CState_Student_Ex::Enter()
{
}

_bool CState_Student_Ex::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();

	if (m_pAnimation->IsFinished())
		return true;

	return false;
}

CStateBase * CState_Student_Ex::Exit()
{
	return nullptr;
}

CState_Student_Ex * CState_Student_Ex::Create(CStudent * pStudent)
{
	CState_Student_Ex*	pInstance = new CState_Student_Ex(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
