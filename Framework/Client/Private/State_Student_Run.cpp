#include "stdafx.h"
#include "State_Student_Run.h"
#include "Student.h"
#include "Transform.h"
#include "Model.h"
#include "Animation.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Key_Manager.h"
#include "Actor.h"


CState_Student_Run::CState_Student_Run(CActor * pStudent)
	:CStateBase(pStudent)
{

}

HRESULT CState_Student_Run::Initialize()
{
	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Original_Move_Ing");

	m_pAnimation = m_pModel->Get_AnimationFromName(pAnimaitonStr);

	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_pAnimation->Play();
	m_pTransform = m_pOwner->Get_Transform();
	return S_OK;
}

void CState_Student_Run::Enter()
{

}

_bool CState_Student_Run::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	if (KEY(Q, AWAY))
	{
		return true;
	}
	m_pTransform->Go_Backward(fTimeDelta);

	return false;
}

CStateBase * CState_Student_Run::Exit()
{
	return nullptr;
}

CState_Student_Run * CState_Student_Run::Create(CActor * pStudent)
{
	CState_Student_Run*	pInstance = new CState_Student_Run(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
