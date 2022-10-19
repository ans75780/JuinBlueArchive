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

	//GameObject ���� ��ũ���� �ʱ�ȭ�ؼ� ���� �̰� �������.
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

	//idle�����ε�, �������� �޾Ҵٸ� ���� ����� ���� ���� �̵�
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
				//��ó�� Ÿ���� �����Ѵٸ� �ѱ��
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
		//���࿡ ���� ��Ʋ���´ٰ� �÷��̾ �׾��ٸ�
		���� ����� �÷��̾ ã�� �̵���.
		���� ��� �÷��̾ �׾ Ÿ���� nullptr�� �����ϸ� Idle�� ������ȯ.
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
