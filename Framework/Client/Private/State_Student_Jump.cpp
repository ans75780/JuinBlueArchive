#include "stdafx.h"
#include "State_Student_Jump.h"
#include "..\Public\State_Student_Jump.h"
#include "State_Student_Run.h"
#include "Student.h"
#include "Animation.h"
#include "GameObject.h"
#include "StateMachineBase.h"
#include "GameInstance.h"
#include "Layer.h"


CState_Student_Jump::CState_Student_Jump(CActor * pActor)
	:CStateBase(pActor)
{

}

HRESULT CState_Student_Jump::Initialize()
{

	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Move_Jump");

	m_pAnimation = m_pModel->Get_AnimationFromName(pAnimaitonStr);

	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_fOriginSpeed = m_pOwner->Get_Transform()->Get_Desc().fSpeedPerSec;
	m_pOwner->Get_Transform()->Set_Speed(m_fOriginSpeed * 0.8f);	

	m_pAnimation->Play();
	return S_OK;

}

void CState_Student_Jump::Enter()
{
	__super::Enter();
}

_bool CState_Student_Jump::Loop(_float fTimeDelta)
{
	if (m_pAnimation->IsFinished())
	{
		m_pAnimation->Reset();
		m_pOwner->Get_Transform()->Set_Speed(m_fOriginSpeed);
		return true;
	}

	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	return false;
}

CStateBase * CState_Student_Jump::Exit()
{
	return nullptr;
}

CStateBase * CState_Student_Jump::Create(CActor * pStudent)
{
	CState_Student_Jump*	pInstance = new CState_Student_Jump(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
