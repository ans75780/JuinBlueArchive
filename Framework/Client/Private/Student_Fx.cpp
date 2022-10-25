#include "stdafx.h"
#include "Student_Fx.h"
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

CStudent_FX::CStudent_FX(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CActor(pDevice, pContext)
{
}


CStudent_FX::CStudent_FX(const CStudent_FX & rhs)
	: CActor(rhs)
{

}

HRESULT CStudent_FX::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStudent_FX::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	//GameObject 에서 디스크립션 초기화해서 먼저 이거 해줘야함.
	if (FAILED(CGameObject::Initialize(&TransformDesc)))
		return E_FAIL;

	memcpy(&m_desc, pArg, sizeof(OBJ_DESC));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.00f, 1.00f, 1.00f));

	return S_OK;
}

HRESULT CStudent_FX::StartLevel(_uint iLevel)
{
	if (FAILED(SetUp_StateMachine(iLevel)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CStudent_FX::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	_tchar szModelTag[MAX_PATH] = L"Prototype_Component_Model_";

	lstrcat(szModelTag, m_desc.sz_Name);
	lstrcat(szModelTag, L"_FX");

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, szModelTag, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	/* For.Com_AABB */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;



	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(m_desc.fRange, m_desc.fRange, m_desc.fRange);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_AttackRangeCollider"), (CComponent**)&m_pAttackRangeCollider, &ColliderDesc)))
		return E_FAIL;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_BodyCollider"), (CComponent**)&m_pBodyCollider, &ColliderDesc)))
		return E_FAIL;

	m_pBodyCollider->Set_Color(_float4(0.f, 0.f, 1.f, 1.f));
	m_pAttackRangeCollider->Set_Color(_float4(1.f, 0.f, 1.f, 1.f));

	return S_OK;
}

void CStudent_FX::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
}

void CStudent_FX::LateTick(_float fTimeDelta)
{
	if (m_bEnable)
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

HRESULT CStudent_FX::Render()
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


HRESULT CStudent_FX::SetUp_ShaderResource()
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

HRESULT CStudent_FX::SetUp_StateMachine(_uint iClonedLevel)
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

void CStudent_FX::CheckState()
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




CStudent_FX * CStudent_FX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CStudent_FX*		pInstance = new CStudent_FX(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStudent_FX");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStudent_FX::Clone(void * pArg)
{
	CStudent_FX*		pInstance = new CStudent_FX(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStudent_FX");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStudent_FX::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pStateMachine);

}
