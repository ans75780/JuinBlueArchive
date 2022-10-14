#include "stdafx.h"
#include "State_Student_Run.h"
#include "Student.h"
#include "Animation.h"
#include "GameObject.h"
#include "StateMachineBase.h"
#include "GameInstance.h"
#include "Layer.h"
#include "State_Student_Jump.h"
#include "State_Attack.h"
#include "Transform_Utils.h"
#include "Actor.h"
CState_Student_Run::CState_Student_Run(CActor * pActor)
	:CState_Run(pActor)
{
}

HRESULT CState_Student_Run::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	m_pBodyCollider = (CCollider*)m_pOwner->Get_Component(L"Com_BodyCollider");
	m_pAttackRangeCollider = (CCollider*)m_pOwner->Get_Component(L"Com_AttackRangeCollider");
	
	
	return S_OK;
}

void CState_Student_Run::Enter()
{
	__super::Enter();
	m_pOwner->Get_Transform()->Rotation(XMVectorSet(0,1.f,0.f,1.f),XMConvertToRadians(0.f));
	m_pOwner->Set_StageState(CActor::STATE_STATE_MOVE);

}

_bool CState_Student_Run::Loop(_float fTimeDelta)
{
	if (__super::Loop(fTimeDelta))
		return true;

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
