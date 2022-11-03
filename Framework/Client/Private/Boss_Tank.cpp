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
#include "UI_BossHpBar.h"
#include "Layer.h"
#include "Transform_Utils.h"
#include "Boss_Tank.h"
#include "State_Tank_Destroy.h"
#include "Boss_Tank.h"
CBoss_Tank::CBoss_Tank(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEnemy(pDevice, pContext)
{
}


CBoss_Tank::CBoss_Tank(const CBoss_Tank & rhs)
	: CEnemy(rhs)
{

}

HRESULT CBoss_Tank::StartLevel(_uint iLevel)
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

HRESULT CBoss_Tank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Tank::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	//GameObject 에서 디스크립션 초기화해서 먼저 이거 해줘야함.
	if (FAILED(CGameObject::Initialize(&TransformDesc)))
		return E_FAIL;

	m_desc.eClass = UNIT_CLASS_FRONT;
	m_desc.eType = UNIT_TYPE_ENEMY;
	m_desc.fHp = 50.f;
	m_desc.fMaxHp = 50.f;
	m_desc.iMagazine = 1000;
	m_desc.iMaxMagazine = 1000;
	m_desc.fDamage = 50.f;
	m_desc.fRange = 4.f;	

	lstrcpy(m_desc.sz_Name, L"EnemyTSS_PowerLoader_PMC");

	//memcpy(&m_desc, pArg, sizeof(OBJ_DESC));

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (FAILED(SetUp_StateMachine(0)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EnemyTSS_PowerLoader_PMC_Diffuse"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;
	
	m_pTransformCom->Set_Scaled(_float3(1.00f, 1.00f, 1.00f));
	
	m_pHpBar->Set_Offset(_float3(0.f, 1.8f, 0.f));

	return S_OK;
}

void CBoss_Tank::Tick(_float fTimeDelta)
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

HRESULT CBoss_Tank::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; i++)
	{
		//if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			//continue;

		if (FAILED(m_pTexture->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
			continue;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}
	return S_OK;
}

void CBoss_Tank::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CBoss_Tank::Damaged(_float fAtk)
{
	m_desc.fHp -= fAtk;
	if (0 >= m_desc.fHp)
		m_pStateMachine->Add_State(CState_Tank_Destroy::Create(this));

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

void CBoss_Tank::CheckState()
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

		if (Enemies.size() < 1)
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
				m_pTarget = (CActor*)pTarget;
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

HRESULT CBoss_Tank::SetUp_StateMachine(_uint iClonedLevel)
{
	m_pStateMachine = CStateMachineBase::Create(this);
	m_pStateMachine->Setup_StateMachine(CState_Idle::Create(this));

	return S_OK;
}

CBoss_Tank * CBoss_Tank::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Tank*		pInstance = new CBoss_Tank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss_Tank");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Tank::Clone(void * pArg)
{
	CBoss_Tank*		pInstance = new CBoss_Tank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_Tank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Tank::Free()
{
	__super::Free();

}
