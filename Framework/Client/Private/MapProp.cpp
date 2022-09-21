#include "stdafx.h"
#include "..\Public\MapProp.h"

#include "GameInstance.h"

CMapProp::CMapProp(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CMapProp::CMapProp(const CMapProp & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMapProp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapProp::Initialize(void * pArg)
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

	lstrcpy(m_desc.sz_Name, TEXT("MapProp"));

	if (pArg)
	{
		CGameObject::OBJ_DESC* tempDesc = static_cast<CGameObject::OBJ_DESC*>(pArg);

		lstrcpy(m_desc.sz_Name, tempDesc->sz_Name);
	}

	return S_OK;
}

void CMapProp::Tick(_float fTimeDelta)
{

}

void CMapProp::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMapProp::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (i == 3)
		{

		}
		else if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		return E_FAIL;*/

		m_pShaderCom->Begin(0);

		m_pModelCom->NonAnimRender(i);
	}

	return S_OK;
}


HRESULT CMapProp::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	//if (FAILED(__super::Add_Component(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_City_Building_01"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CMapProp::SetUp_ShaderResource()
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

CMapProp * CMapProp::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMapProp*		pInstance = new CMapProp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapProp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMapProp::Clone(void * pArg)
{
	CMapProp*		pInstance = new CMapProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMapProp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapProp::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}
