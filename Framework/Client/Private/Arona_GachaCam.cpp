#include "stdafx.h"
#include "..\Public\Arona_GachaCam.h"

#include "GameInstance.h"

CArona_GachaCam::CArona_GachaCam(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CArona_GachaCam::CArona_GachaCam(const CArona_GachaCam & rhs)
	: CGameObject(rhs)
{
}

HRESULT CArona_GachaCam::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArona_GachaCam::Initialize(void * pArg)
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

void CArona_GachaCam::Tick(_float fTimeDelta)
{
	//m_pModelCom->Play_Animation(fTimeDelta * 0.5f);

}

void CArona_GachaCam::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CArona_GachaCam::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;




	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;


		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}

	return S_OK;
}

CAnimation * CArona_GachaCam::Get_Animation(const char * pAnimationName)
{
	CAnimation*	m_pAnimation = nullptr;

	m_pAnimation = m_pModelCom->Get_AnimationFromName(pAnimationName);

	return m_pAnimation;
}

HRESULT CArona_GachaCam::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Model_Arona_Gacha_Cam"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CArona_GachaCam::SetUp_ShaderResource()
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

CArona_GachaCam * CArona_GachaCam::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CArona_GachaCam*		pInstance = new CArona_GachaCam(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArona_GachaCam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArona_GachaCam::Clone(void * pArg)
{
	CArona_GachaCam*		pInstance = new CArona_GachaCam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CArona_GachaCam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArona_GachaCam::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}