#include "stdafx.h"
#include "State_Student_Formation_Pickup.h"
#include "Student.h"
#include "Transform.h"
#include "Model.h"
#include "Animation.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Key_Manager.h"
CState_Student_Formation_Pickup::CState_Student_Formation_Pickup(CStudent * pStudent)
	:CStateBase(pStudent)
{

}

HRESULT CState_Student_Formation_Pickup::Initialize()
{
	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, m_pStudent->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Original_Formation_Pickup");

	m_pAnimation = m_pModel->Get_AnimationFromName(pAnimaitonStr);

	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_pAnimation->Play();
	m_pTransform = m_pStudent->Get_Transform();


	return S_OK;
}

void CState_Student_Formation_Pickup::Enter()
{

}

_bool CState_Student_Formation_Pickup::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	
	if (m_bCalledExit)
		return true;

	return false;
}

CStateBase * CState_Student_Formation_Pickup::Exit()
{
	return nullptr;
}

CState_Student_Formation_Pickup * CState_Student_Formation_Pickup::Create(CStudent * pStudent)
{
	CState_Student_Formation_Pickup*	pInstance = new CState_Student_Formation_Pickup(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
