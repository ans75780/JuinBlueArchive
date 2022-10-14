#include "stdafx.h"
#include "..\Public\State_Attack.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Student_Run.h"
#include "GameInstance.h"
#include "Actor.h"
#include "GameObject.h"

CState_Attack::CState_Attack(CActor * pStudent, CActor * pTarget)
	:CStateBase(pStudent)
{
}

HRESULT CState_Attack::Initialize()
{
	char pAnimaitonStr[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, m_pOwner->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);


	char	AttackStart[MAX_PATH] = "";
	char	AttackIng[MAX_PATH] = "";

	char	AttackDelay[MAX_PATH] = "";
	char	AttackEnd[MAX_PATH] = "";
	char	AttackReload[MAX_PATH] = "";

	strcpy_s(AttackStart, pAnimaitonStr);
	strcat_s(AttackStart, "_Normal_Attack_Start");

	strcpy_s(AttackIng, pAnimaitonStr);
	strcat_s(AttackIng, "_Normal_Attack_Ing");

	strcpy_s(AttackDelay, pAnimaitonStr);
	strcat_s(AttackDelay, "_Normal_Attack_Delay");
	
	strcpy_s(AttackReload, pAnimaitonStr);
	strcat_s(AttackReload, "_Normal_Reload");

	strcpy_s(AttackEnd, pAnimaitonStr);
	strcat_s(AttackEnd, "_Normal_Attack_End");

	if (nullptr == (m_pAnims[ATTACK_STATE_START] = m_pOwner->Get_Animation(AttackStart)))
		return E_FAIL;

	if (nullptr == (m_pAnims[ATTACK_STATE_ING] = m_pOwner->Get_Animation(AttackIng)))
		return E_FAIL;

	if (nullptr == (m_pAnims[ATTACK_STATE_DELAY] = m_pOwner->Get_Animation(AttackDelay)))
		return E_FAIL;

	if (nullptr == (m_pAnims[ATTACK_STATE_RELOAD] = m_pOwner->Get_Animation(AttackReload)))
		return E_FAIL;
	
	if (nullptr == (m_pAnims[ATTACK_STATE_END] = m_pOwner->Get_Animation(AttackEnd)))
		return E_FAIL;

	m_eCurrentState = ATTACK_STATE_START;
	m_pOwner->Set_StageState(CActor::STAGE_STATE_BATTLE);
	return S_OK;
}

void CState_Attack::Enter()
{
	m_pOwner->Set_StageState(CActor::STAGE_STATE_BATTLE);
	m_pAnims[m_eCurrentState]->Play();
}

_bool CState_Attack::Loop(_float fTimeDelta)
{
	if (m_pOwner == nullptr ||  m_pOwner->Get_StageState() == CActor::STAGE_STATE_DEAD)
		return true;
	if (m_pTarget == nullptr || m_pTarget->Get_StageState() == CActor::STAGE_STATE_DEAD)
		return true;

	if(m_pTarget->Get_StageState() != CActor::STAGE_STATE_DEAD)
		m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	//애니메이션이 끝났다면
	if (m_pAnims[m_eCurrentState]->IsFinished())
	{
		//끝난 애니메이션이 무엇인지 검사해서 돌림.
		// 타겟이 죽은상태고, 현재 상태가 죽은게 아니라면 -> AttackEnd 애니메이션 발동.
		switch (m_eCurrentState)
		{
		case Client::CState_Attack::ATTACK_STATE_START:
				Change_Animation(ATTACK_STATE_ING);
			break;
			//탄창 분기.(레퍼런스로 빼온 탄환이 마지막이면 재장전, 아니면 딜레이 줌)
		case Client::CState_Attack::ATTACK_STATE_ING:
			m_pTarget->Damaged(m_pOwner->Get_Desc().fDamage);
			m_pOwner->Get_OBJ_DESC_Reference().iMagazine--;
			if (m_pOwner->Get_OBJ_DESC_Reference().iMagazine == 0)
				Change_Animation(ATTACK_STATE_RELOAD);
			else
				Change_Animation(ATTACK_STATE_DELAY);
			break;
		case Client::CState_Attack::ATTACK_STATE_DELAY:
			Change_Animation(ATTACK_STATE_ING);
			break;
		case Client::CState_Attack::ATTACK_STATE_RELOAD:
			m_pOwner->Get_OBJ_DESC_Reference().iMagazine = m_pOwner->Get_OBJ_DESC_Reference().iMaxMagazine;
			Change_Animation(ATTACK_STATE_START);
		case Client::CState_Attack::ATTACK_STATE_END:
			return true;
			break;
		case Client::CState_Attack::ATTACK_STATE_ENUM_END:
			break;
		default:
			break;
		}	
		if (m_pTarget->Get_StageState() == CActor::STAGE_STATE_DEAD &&
			m_eCurrentState != ATTACK_STATE_END)
		{
			Change_Animation(ATTACK_STATE_END);
		}
	}
	
	m_pAnims[m_eCurrentState]->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	return false;
}

CStateBase * CState_Attack::Exit()
{
	for (_uint i = 0; i < ATTACK_STATE_ENUM_END; i++)
		m_pAnims[i]->Reset();
	return nullptr;
}

CAnimation * CState_Attack::Get_Animation()
{
	return m_pAnims[m_eCurrentState];
}

//애니메이션 분기 함수. 전 애니메이션 초기화 하고 현재 애니메이션 돌림
void CState_Attack::Change_Animation(ATTACK_STATE eState)
{
	m_pAnims[m_eCurrentState]->Reset();
	m_eCurrentState = eState;
	m_pAnims[m_eCurrentState]->Play();
}

CState_Attack * CState_Attack::Create(CActor * pActor, class CActor* pTarget)
{
 	CState_Attack*	pInstance = new CState_Attack(pActor, pTarget);
	pInstance->m_pTarget = pTarget;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
