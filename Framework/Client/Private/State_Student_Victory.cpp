#include "stdafx.h"
#include "State_Idle.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Student_Run.h"
#include "GameInstance.h"
#include "Actor.h"
#include "State_Student_Victory.h"


CState_Student_Victory::CState_Student_Victory(CActor * pStudent)
	:CStateBase(pStudent)
{
}

HRESULT CState_Student_Victory::Initialize()
{

	char pAnimaitonStr[MAX_PATH];


	char victoryStart[MAX_PATH];
	char victoryEnd[MAX_PATH];


	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, victoryStart, MAX_PATH, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, victoryEnd, MAX_PATH, NULL, NULL);


	strcat_s(victoryStart, "_Victory_Start");
	strcat_s(victoryEnd, "_Victory_End");


	m_pVictoryStart = m_pModel->Get_AnimationFromName(victoryStart);

	if (nullptr == m_pVictoryStart)
		return E_FAIL;

	m_pVictoryEnd= m_pModel->Get_AnimationFromName(victoryEnd);

	if (nullptr == m_pVictoryEnd)
		return E_FAIL;

	m_pAnimation = m_pVictoryStart;

	return S_OK;
}

void CState_Student_Victory::Enter()
{
	m_pAnimation->Play();
	m_pOwner->Set_StageState(CActor::STAGE_STATE_EVENT);
}

_bool CState_Student_Victory::Loop(_float fTimeDelta)
{
	if (m_pVictoryStart->IsFinished())
	{
		m_pAnimation = m_pVictoryEnd;
	}
	if (m_pVictoryEnd->IsFinished())
	{
		m_pVictoryEnd->Reset();
	}

	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();

	return false;
}

CStateBase * CState_Student_Victory::Exit()
{
	return CState_Student_Run::Create(m_pOwner);
}

CState_Student_Victory * CState_Student_Victory::Create(CActor * pStudent)
{
	CState_Student_Victory*	pInstance = new CState_Student_Victory(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
