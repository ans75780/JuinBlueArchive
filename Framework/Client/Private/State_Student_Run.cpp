#include "stdafx.h"
#include "State_Student_Run.h"
#include "Student.h"
#include "Animation.h"
#include "GameObject.h"
#include "StateMachineBase.h"
#include "GameInstance.h"
#include "Layer.h"
#include "State_Student_Jump.h"
CState_Student_Run::CState_Student_Run(CActor * pActor)
	:CState_Run(pActor)
{
}

HRESULT CState_Student_Run::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	m_pBodyCollider = (CCollider*)m_pOwner->Get_Component(L"Com_BodyCollider");
	


	return S_OK;
}

void CState_Student_Run::Enter()
{
	__super::Enter();
}

_bool CState_Student_Run::Loop(_float fTimeDelta)
{
	if (__super::Loop(fTimeDelta))
		return true;

	list<CGameObject*>	Baricades;

	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	Baricades = pInstance->Get_Layer(pInstance->Get_CurrentLevelID())[L"Layer_Baricade"]->Get_GameObjects();


	for (auto& elem : Baricades)
	{
		if (m_pBodyCollider->Collision((CCollider*)elem->Get_Component(L"Com_SPHERE")))
		{
			if (m_pOwner->Get_Desc().eClass != UNIT_CLASS_BACK)
			{
				m_pAnimation->Stop();
				m_pOwner->Get_StateMachine()->Add_State(CState_Student_Jump::Create(m_pOwner));
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return false;
}

CStateBase * CState_Student_Run::Exit()
{
	return nullptr;
}

CStateBase * CState_Student_Run::Create(CActor * pStudent)
{
	CState_Student_Run*	pInstance = new CState_Student_Run(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
