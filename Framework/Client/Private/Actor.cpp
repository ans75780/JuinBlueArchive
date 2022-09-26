#include "stdafx.h"
#include "Actor.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "State_Student_Idle.h"
#include "State_Student_Run.h"
#include "Collider.h"
#include "State_Student_Formation_Idle.h"
#include "Animation.h"
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

	memcpy(&TransformDesc, pArg, sizeof(CTransform::TRANSFORMDESC));

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;
	return S_OK;
}

void CActor::Tick(_float fTimeDelta)
{
	m_pStateMachine->Update(fTimeDelta);


	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CActor::LateTick(_float fTimeDelta)
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

HRESULT CActor::SetUp_Components()
{
	return S_OK;
}

HRESULT CActor::SetUp_ShaderResource()
{
	return S_OK;
}

HRESULT CActor::SetUp_StateMachine()
{
	
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
