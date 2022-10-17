#include "stdafx.h"
#include "Actor.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "State_Idle.h"
#include "State_Run.h"
#include "Collider.h"
#include "State_Student_Formation_Idle.h"
#include "Animation.h"
#include "State_Dead.h"
CActor::CActor(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}


CActor::CActor(const CActor & rhs)
	: CGameObject(rhs)
{

}

HRESULT CActor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CActor::Initialize(void * pArg)
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
	if (FAILED(SetUp_StateMachine(0)))
		return E_FAIL;
	return S_OK;
}

void CActor::Tick(_float fTimeDelta)
{
	m_pStateMachine->Update(fTimeDelta);


	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_pAttackRangeCollider->Update(m_pTransformCom->Get_WorldMatrix());
	m_pBodyCollider->Update(m_pTransformCom->Get_WorldMatrix());


}

void CActor::LateTick(_float fTimeDelta)
{
	if (m_bEnable == true)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

#ifdef _DEBUG
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_CurrentLevelID() == LEVEL_GAMEPLAY)
	{
		m_pRendererCom->Add_DebugRenderGroup(m_pAttackRangeCollider);
		m_pRendererCom->Add_DebugRenderGroup(m_pBodyCollider);

	}
	RELEASE_INSTANCE(CGameInstance);
#endif // _DEBUG

}

HRESULT CActor::Render()
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
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}
	return S_OK;
}

HRESULT CActor::Render_MeshPart(CMeshContainer * pMesh)
{
	if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", pMesh, aiTextureType_DIFFUSE)))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pModelCom->Render(pMesh, m_pShaderCom, "g_Bones");

	return S_OK;
}


void CActor::Set_Transform(_vector vPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

CAnimation * CActor::Get_Animation(const char * pAnimationName)
{

	CAnimation*	m_pAnimation = nullptr;

	m_pAnimation = m_pModelCom->Get_AnimationFromName(pAnimationName);

	return m_pAnimation;
}

_bool CActor::Collision_AABB(RAYDESC & ray, _float & distance)
{
	return m_pAABBCom->Collision_AABB(ray, distance);
}

void CActor::Set_Desc(OBJ_DESC & desc)
{
	memcpy(&m_desc, &desc, sizeof(OBJ_DESC));
}

void CActor::Damaged(_float fAtk)
{
	m_desc.fHp -= fAtk;
	if (0 >= m_desc.fHp)
		m_pStateMachine->Add_State(CState_Dead::Create(this));

}

void CActor::CheckState()
{
}

HRESULT CActor::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	_tchar szModelTag[MAX_PATH] = L"Prototype_Component_Model_";

	lstrcat(szModelTag, m_desc.sz_Name);

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

HRESULT CActor::SetUp_ShaderResource()
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

HRESULT CActor::SetUp_StateMachine(_uint iLevel)
{
	m_pStateMachine = CStateMachineBase::Create(this);

	return S_OK;
}

CGameObject * CActor::Clone(void * pArg)
{
	CActor*		pInstance = new CActor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CActor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CActor::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pStateMachine);

}
