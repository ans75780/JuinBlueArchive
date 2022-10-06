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

}

_bool CState_Student_Run::Loop(_float fTimeDelta)
{
	list<CGameObject*>	Baricades;
	list<CGameObject*>	Enemies;


	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	Baricades = pInstance->Get_Layer(pInstance->Get_CurrentLevelID())[L"Layer_Baricade"]->Get_GameObjects();

	for (auto& elem : Baricades)
	{
		if (m_pBodyCollider->Collision((CCollider*)elem->Get_Component(L"Com_SPHERE")))
		{
			if (m_pOwner->Get_Desc().eClass != UNIT_CLASS_BACK)
			{
				m_pAnimation->Reset();
				m_pOwner->Get_StateMachine()->Add_State(CState_Student_Jump::Create(m_pOwner));
			}
		}
	}

	Enemies = pInstance->Get_Layer(pInstance->Get_CurrentLevelID())[L"Layer_Enemy"]->Get_GameObjects();

	CGameObject::OBJ_DESC Desc = m_pOwner->Get_OBJ_DESC();

	CGameObject*	pTarget = nullptr;
	_float			fPrevDistance = 999.f;
	for (auto& elem : Enemies)
	{
		if (elem == m_pOwner || ((CActor*)elem)->Get_StageState() == CActor::STAGE_STATE_DEAD)
			continue;
	
		float fDistance = CTransform_Utils::Get_Range(m_pOwner->Get_Transform(), elem->Get_Transform());
		if (Desc.fRange >= fDistance)
		{
			//맨처음 타겟이 존재한다면 넘기기
			if (pTarget == nullptr)
			{
				pTarget = elem;
				fPrevDistance = fDistance;
			}
			else
			{
				if (fPrevDistance > fDistance)
				{
					pTarget = elem;
					fPrevDistance = fDistance;
				}	
			}	
		}
	}
	if (pTarget != nullptr)
	{
		m_pAnimation->Reset();
		m_pOwner->Get_StateMachine()->Add_State(CState_Attack::Create(m_pOwner, (CActor*)pTarget));
	}
	RELEASE_INSTANCE(CGameInstance);
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
