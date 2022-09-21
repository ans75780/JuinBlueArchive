#include "stdafx.h"
#include "Student.h"

#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "State_Student_Idle.h"
#include "State_Student_Run.h"
#include "Collider.h"


CStudent::CStudent(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _tchar*	pStudentTag)
	: CGameObject(pDevice, pContext)
{
	lstrcpy(m_szStudentName, pStudentTag);
}


CStudent::CStudent(const CStudent & rhs)
	: CGameObject(rhs)
{
	lstrcpy(m_szStudentName, rhs.m_szStudentName);
}

HRESULT CStudent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStudent::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.00f, 1.00f, 1.00f));
	//m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));

	m_pStateMachine = CStateMachineBase::Create(this);

	CState_Student_Idle* state = CState_Student_Idle::Create(this);

	m_pStateMachine->Setup_StateMachine(state);
	//m_pModelCom->Set_CurrentAnimation(3);
	return S_OK;
}

void CStudent::Tick(_float fTimeDelta)
{
	m_pStateMachine->Update(fTimeDelta);
	

	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	//m_pModelCom->Play_Animation(fTimeDelta);

}

void CStudent::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

#ifdef _DEBUG
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_CurrentLevelID() == LEVEL_FORMATION)
	{
		m_pRendererCom->Add_DebugRenderGroup(m_pAABBCom);
	}
#endif // _DEBUG


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
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		return E_FAIL;*/
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}

#ifdef _DEBUG
	
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_CurrentLevelID() == LEVEL_FORMATION)
		//m_pAABBCom->Render();
	//m_pOBBCom->Render();
	//m_pSphereCom->Render();
#endif // _DEBUG


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CStudent::Render_MeshPart(CMeshContainer * pMesh)
{
	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", pMesh, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pModelCom->Render(pMesh, m_pShaderCom, "g_Bones");

	return S_OK;
}

void CStudent::Set_Transform(_vector vPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

_bool CStudent::Collision_AABB(RAYDESC & ray, _float & distance)
{
	return m_pAABBCom->Collision_AABB(ray, distance);
}

HRESULT CStudent::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	_tchar szModelTag[MAX_PATH] = L"Prototype_Component_Model_";

	lstrcat(szModelTag, m_szStudentName);

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

CStudent * CStudent::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _tchar *pStudentTag)
{
	CStudent*		pInstance = new CStudent(pDevice, pContext, pStudentTag);

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
