#include "stdafx.h"
#include "..\Public\Arona_Cam.h"

#include "GameInstance.h"

CArona_Cam::CArona_Cam(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CArona_Cam::CArona_Cam(const CArona_Cam & rhs)
	: CGameObject(rhs)
{
}

HRESULT CArona_Cam::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArona_Cam::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimation(0);



	return S_OK;
}

void CArona_Cam::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta * 0.5f);

}

void CArona_Cam::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

//#ifdef _DEBUG
//	m_pRendererCom->Add_DebugRenderGroup(m_pAABBCom);
//	m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
//	m_pRendererCom->Add_DebugRenderGroup(m_pSphereCom);
//#endif // _DEBUG
}

HRESULT CArona_Cam::Render()
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
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		return E_FAIL;*/


		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}

#ifdef _DEBUG
	/*m_pAABBCom->Render();
	m_pOBBCom->Render();
	m_pSphereCom->Render();
	m_pNaviCom->Render();*/
#endif // _DEBUG



	return S_OK;
}

HRESULT CArona_Cam::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Model_Arona_Cam"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CArona_Cam::SetUp_ShaderResource()
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

CArona_Cam * CArona_Cam::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CArona_Cam*		pInstance = new CArona_Cam(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArona_Cam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArona_Cam::Clone(void * pArg)
{
	CArona_Cam*		pInstance = new CArona_Cam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CArona_Cam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArona_Cam::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}
