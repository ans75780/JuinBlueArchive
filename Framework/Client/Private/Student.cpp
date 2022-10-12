#include "stdafx.h"
#include "Student.h"
#include "Actor.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "State_Idle.h"
#include "State_Run.h"
#include "Collider.h"
#include "State_Student_Formation_Idle.h"
#include "Animation.h"
#include "HpBar.h"
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


	return S_OK;
}

void CStudent::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pTransformCom->Get_WorldMatrix();

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
