#include "stdafx.h"
#include "State_Run.h"
#include "Student.h"
#include "Transform.h"
#include "Model.h"
#include "Animation.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Key_Manager.h"
#include "Actor.h"

CState_Run::CState_Run(CActor * pStudent)
	:CStateBase(pStudent)
{

}

HRESULT CState_Run::Initialize()
{
	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Move_Ing");

	m_pAnimation = m_pModel->Get_AnimationFromName(pAnimaitonStr);

	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_pTransform = m_pOwner->Get_Transform();
	return S_OK;
}

void CState_Run::Enter()
{
	__super::Enter();
	m_pOwner->Set_StageState(CActor::STATE_STATE_MOVE);


}

_bool CState_Run::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();


	m_pTransform->Go_Straight(fTimeDelta);

	return false;
}

CStateBase * CState_Run::Exit()
{
	return nullptr;
}

CState_Run * CState_Run::Create(CActor * pStudent)
{
	CState_Run*	pInstance = new CState_Run(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
