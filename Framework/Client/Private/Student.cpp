#include "stdafx.h"
#include "Student.h"

#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "State_Student_Idle.h"

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

	CState_Student_Idle* state = (CState_Student_Idle*)CState_Student_Idle::Create(this);

	m_pStateMachine->Setup_StateMachine(state);
	//m_pModelCom->Set_CurrentAnimation(3);
	return S_OK;
}

void CStudent::Tick(_float fTimeDelta)
{
	m_pStateMachine->Update(fTimeDelta);
	
	//m_pModelCom->Play_Animation(fTimeDelta);

}

void CStudent::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
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

	
	Render_MeshPart(m_pHair);
	Render_MeshPart(m_pBody);
	Render_MeshPart(m_pFace);
	Render_MeshPart(m_pHead);
	Render_MeshPart(m_pWeapon);
	Render_MeshPart(m_pHalo);
	

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


HRESULT CStudent::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	_tchar szModelTag[MAX_PATH] = L"Test_Prototype_Component_Model_";

	lstrcat(szModelTag, m_szStudentName);

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, szModelTag, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	m_pHair = m_pModelCom->Get_MeshContainers(0);
	m_pBody = m_pModelCom->Get_MeshContainers(1);
	m_pFace = m_pModelCom->Get_MeshContainers(2);
	m_pHead = m_pModelCom->Get_MeshContainers(4);
	m_pWeapon = m_pModelCom->Get_MeshContainers(5);
	m_pHalo = m_pModelCom->Get_MeshContainers(6);


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


	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
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
