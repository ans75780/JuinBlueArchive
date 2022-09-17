#include "stdafx.h"
#include "State_Student_Default.h"
#include "Student.h"
#include "Transform.h"
#include "Model.h"
#include "Animation.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Key_Manager.h"
CState_Student_Default::CState_Student_Default(CStudent * pStudent, _tchar* AnimationName)
	:CStateBase(pStudent)
{
	_tchar pTAnimaitonStr[MAX_PATH];
	char	szAnimationStr[MAX_PATH];

	lstrcpy(pTAnimaitonStr, m_pStudent->Get_Name());
	lstrcat(pTAnimaitonStr, AnimationName);
	
	WideCharToMultiByte(CP_ACP, 0, pTAnimaitonStr, MAX_PATH, szAnimationStr, MAX_PATH, NULL, NULL);

	m_pAnimation = m_pModel->Get_AnimationFromName(szAnimationStr);
}

HRESULT CState_Student_Default::Initialize()
{
	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_pAnimation->Play();
	m_pTransform = m_pStudent->Get_Transform();
	return S_OK;
}

void CState_Student_Default::Enter()
{

}

_bool CState_Student_Default::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();

	if (m_pAnimation->IsFinished())
		return true;

	return false;
}

CStateBase * CState_Student_Default::Exit()
{
	return nullptr;
}

CState_Student_Default * CState_Student_Default::Create(CStudent * pStudent, _tchar* AnimationName)
{
	CState_Student_Default*	pInstance = new CState_Student_Default(pStudent, AnimationName);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
