#include "stdafx.h"
#include "Enemy.h"
#include "Actor.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "Collider.h"
#include "Animation.h"
#include "HpBar.h"
#include "State_Headers.h"
#include "Layer.h"
#include "Transform_Utils.h"

CEnemy::CEnemy(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CActor(pDevice, pContext)
{
}


CEnemy::CEnemy(const CEnemy & rhs)
	: CActor(rhs)
{

}

HRESULT CEnemy::StartLevel(_uint iLevel)
{
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);


	if (iLevel == LEVEL::LEVEL_GAMEPLAY)
	{
		if (FAILED(pInstance->Add_GameObject(iLevel, L"Layer_HpBar", L"Prototype_HpBar", this, ((CGameObject**)&m_pHpBar))))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEnemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	//GameObject 에서 디스크립션 초기화해서 먼저 이거 해줘야함.
	if (FAILED(CGameObject::Initialize(&TransformDesc)))
		return E_FAIL;

	memcpy(&m_desc, pArg, sizeof(OBJ_DESC));

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (FAILED(SetUp_StateMachine(0)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.00f, 1.00f, 1.00f));
	
	return S_OK;
}

void CEnemy::Tick(_float fTimeDelta)
{
	if (m_bEnable == false)
		return;

	__super::Tick(fTimeDelta);

		CheckState();

	if (m_eStageState == CActor::STAGE_STATE::STAGE_STATE_DEAD)
	{
		m_pHpBar->Set_Delete(true);
	}
}

HRESULT CEnemy::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CEnemy::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CEnemy::Damaged(_float fAtk)
{
	__super::Damaged(fAtk);

	//idle상태인데, 데미지를 받았다면 가장 가까운 적을 향해 이동
	if (m_eStageState != CActor::STAGE_STATE_DEAD && m_eStageState == CActor::STATE_STATE_IDLE)
	{
		CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

		map<const _tchar*, class CLayer*>				m_pLayers;

		m_pLayers = pInstance->Get_Layer(LEVEL_GAMEPLAY);
		list<CGameObject*>	Enemies;

		Enemies = find_if(m_pLayers.begin(), m_pLayers.end(), CTag_Finder(L"Layer_Student"))->second->Get_GameObjects();
		if (m_pLayers.size() < 1)
			return;

		if (Enemies.size() < 1)
			return;

		CGameObject*	pTarget = nullptr;
		_float			fPrevDistance = 999.f;
		for (auto& elem : Enemies)
		{
			if (elem == this || ((CActor*)elem)->Get_StageState() == CActor::STAGE_STATE_DEAD)
				continue;
			float fDistance = CTransform_Utils::Get_Range(Get_Transform(), elem->Get_Transform());
			if (fPrevDistance > fDistance)
			{
				pTarget = elem;
				fPrevDistance = fDistance;
			}
		}
		if (pTarget != nullptr)
		{
			m_pStateMachine->Get_CurrentState()->Get_Animation()->Reset();
			m_pTransformCom->LookAt(pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			m_pStateMachine->Add_State(CState_Run::Create(this));
		}
		RELEASE_INSTANCE(CGameInstance);
	}


}

void CEnemy::CheckState() 
{


	if (m_eStageState == STATE_STATE_MOVE || m_eStageState == STATE_STATE_IDLE)
	{
		CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

		map<const _tchar*, class CLayer*>				m_pLayers;

		m_pLayers = pInstance->Get_Layer(LEVEL_GAMEPLAY);

		list<CGameObject*>	Enemies;

		Enemies = find_if(m_pLayers.begin(), m_pLayers.end(), CTag_Finder(L"Layer_Student"))->second->Get_GameObjects();
		if (m_pLayers.size() < 1)
			return;

		if (Enemies.size()  < 1)
			return;

		CGameObject*	pTarget = nullptr;
		_float			fPrevDistance = 999.f;
		for (auto& elem : Enemies)
		{
			if (elem == this || ((CActor*)elem)->Get_StageState() == CActor::STAGE_STATE_DEAD)
				continue;
			float fDistance = CTransform_Utils::Get_Range(Get_Transform(), elem->Get_Transform());
			if (m_desc.fRange >= fDistance)
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
			m_pTarget = (CActor*)pTarget;
			m_pStateMachine->Get_CurrentState()->Get_Animation()->Reset();
			m_pStateMachine->Add_State(CState_Attack::Create(this, (CActor*)pTarget));
		}
		RELEASE_INSTANCE(CGameInstance);
	}

		/*
		//만약에 적이 배틀상태다가 플레이어가 죽었다면
		가장 가까운 플레이어를 찾아 이동함.
		만약 모든 플레이어가 죽어서 타겟이 nullptr로 존재하면 Idle로 상태전환.
		*/
	if (m_pTarget)
	{
		if (m_pTarget->Get_StageState() == STAGE_STATE_DEAD)
		{
			CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

			map<const _tchar*, class CLayer*>				m_pLayers;

			m_pLayers = pInstance->Get_Layer(LEVEL_GAMEPLAY);
			list<CGameObject*>	Enemies;

			Enemies = find_if(m_pLayers.begin(), m_pLayers.end(), CTag_Finder(L"Layer_Student"))->second->Get_GameObjects();
			if (m_pLayers.size() < 1)
				return;

			if (Enemies.size() < 1)
				return;

			CGameObject*	pTarget = nullptr;
			_float			fPrevDistance = 999.f;
			for (auto& elem : Enemies)
			{
				if (elem == this || ((CActor*)elem)->Get_StageState() == CActor::STAGE_STATE_DEAD)
					continue;
				float fDistance = CTransform_Utils::Get_Range(Get_Transform(), elem->Get_Transform());
				if (fPrevDistance > fDistance)
				{
					pTarget = elem;
					fPrevDistance = fDistance;
				}
			}
			if (pTarget != nullptr)
			{
				m_pStateMachine->Get_CurrentState()->Get_Animation()->Reset();
				m_pTransformCom->LookAt(pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				m_pStateMachine->Add_State(CState_Run::Create(this));
			}
			else
			{
				m_pStateMachine->Get_CurrentState()->Get_Animation()->Reset();
				m_pStateMachine->Add_State(CState_Idle::Create(this));
			}
			RELEASE_INSTANCE(CGameInstance);
		}
	}
		

}


HRESULT CEnemy::SetUp_StateMachine(_uint iClonedLevel)
{
	m_pStateMachine = CStateMachineBase::Create(this);
	m_pStateMachine->Setup_StateMachine(CState_Idle::Create(this));

	return S_OK;
}

CEnemy * CEnemy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnemy*		pInstance = new CEnemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEnemy::Clone(void * pArg)
{
	CEnemy*		pInstance = new CEnemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy::Free()
{
	__super::Free();

}
