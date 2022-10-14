#include "stdafx.h"
#include "Student.h"
#include "Actor.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "Collider.h"
#include "Animation.h"
#include "HpBar.h"
#include "Transform_Utils.h"
#include "Layer.h"
#include "State_Headers.h"

CStudent::CStudent(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CActor(pDevice, pContext)
{
}


CStudent::CStudent(const CStudent & rhs)
	: CActor(rhs)
{

}

HRESULT CStudent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStudent::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	//GameObject 에서 디스크립션 초기화해서 먼저 이거 해줘야함.
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	memcpy(&m_desc, pArg, sizeof(OBJ_DESC));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.00f, 1.00f, 1.00f));

	return S_OK;
}

HRESULT CStudent::StartLevel(_uint iLevel)
{
	if (FAILED(SetUp_StateMachine(iLevel)))
		return E_FAIL;
	LEVEL eLevel;

	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (iLevel == LEVEL::LEVEL_GAMEPLAY)
	{
		if (FAILED(pInstance->Add_GameObject(iLevel, L"Layer_HpBar", L"Prototype_HpBar", this, ((CGameObject**)&m_pHpBar))))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CStudent::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_eStageState != CActor::STAGE_STATE::STAGE_STATE_DEAD)
		CheckState();

	if (m_eStageState == CActor::STAGE_STATE::STAGE_STATE_DEAD)
	{
		m_pHpBar->Set_Delete(true);
	}
}

void CStudent::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

//#ifdef _DEBUG
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_CurrentLevelID() == LEVEL_FORMATION)
	{
		m_pRendererCom->Add_DebugRenderGroup(m_pAABBCom);
	}
	if (pInstance->Get_CurrentLevelID() == LEVEL_GAMEPLAY)
	{
		m_pRendererCom->Add_DebugRenderGroup(m_pAttackRangeCollider);
		m_pRendererCom->Add_DebugRenderGroup(m_pBodyCollider);

	}
	RELEASE_INSTANCE(CGameInstance);
//#endif // _DEBUG


}

HRESULT CStudent::Render()
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
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			continue;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
	
		m_pShaderCom->Begin(1);
		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}

	return S_OK;
}


HRESULT CStudent::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CStudent::SetUp_StateMachine(_uint iClonedLevel)
{
	//학생정보에서 레벨을 받아와서 맞는 기초상태로 세팅한다.

	m_pStateMachine = CStateMachineBase::Create(this);
	CStateBase* state = nullptr;
	switch (iClonedLevel)
	{
	case Client::LEVEL_GAMEPLAY:
		state = CState_Idle::Create(this);
		break;
	case Client::LEVEL_FORMATION:
		state = CState_Student_Formation_Idle::Create(this);
		break;
	case Client::LEVEL_MAPTOOL:
		break;
	case Client::LEVEL_END:
		break;
	default:
		break;
	}
	if (nullptr == state)
		return E_FAIL;

	m_pStateMachine->Setup_StateMachine(state);
	return S_OK;
}

void CStudent::CheckState()
{
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_CurrentLevelID() != LEVEL_GAMEPLAY)
		return;
	if (m_eStageState == STATE_STATE_MOVE)
	{
		map<const _tchar*, class CLayer*>				m_pLayers;
		m_pLayers = pInstance->Get_Layer(LEVEL_GAMEPLAY);

		list<CGameObject*>	Baricades;

		Baricades = find_if(m_pLayers.begin(), m_pLayers.end(), CTag_Finder(L"Layer_Baricade"))->second->Get_GameObjects();

		for (auto& elem : Baricades)
		{
			if (m_pBodyCollider->Collision((CCollider*)elem->Get_Component(L"Com_SPHERE")))
			{
				if (m_desc.eClass != UNIT_CLASS_BACK)
				{
					m_pStateMachine->Get_CurrentState()->Get_Animation()->Reset();
					m_pStateMachine->Add_State(CState_Student_Jump::Create(this));
				}
			}
		}
		list<CGameObject*>	Enemies;


		Enemies = find_if(m_pLayers.begin(), m_pLayers.end(), CTag_Finder(L"Layer_Enemy"))->second->Get_GameObjects();

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
			m_pStateMachine->Get_CurrentState()->Get_Animation()->Reset();
			m_pStateMachine->Add_State(CState_Attack::Create(this, (CActor*)pTarget));
		}
	}
	RELEASE_INSTANCE(CGameInstance);

}




CStudent * CStudent::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CStudent*		pInstance = new CStudent(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStudent");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStudent::Clone(void * pArg)
{
	CStudent*		pInstance = new CStudent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStudent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStudent::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pStateMachine);

}
