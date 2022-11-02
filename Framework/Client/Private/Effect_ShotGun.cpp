#include "stdafx.h"
#include "..\Public\Effect_ShotGun.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"

#include "Camera_Free.h"
#include "Charater.h"
#include "Hod.h"


CEffect_ShotGun::CEffect_ShotGun(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

HRESULT CEffect_ShotGun::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(0.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_matrix temp = *(_matrix*)pArg;

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, temp.r[3]);
	}


	m_pTransformCom->Set_Scaled(_float3(2.f, 2.f, 2.f));
	m_gVcolor.x = 255.f / 255.f;
	m_gVcolor.y = 255.f / 255.f;
	m_gVcolor.z = 0.f / 255.f;
	m_gVcolor.w = 255.f / 255.f;
	return S_OK;
}

void CEffect_ShotGun::Tick(_float fTimeDelta)
{
	if (m_bOff)
		return;

	m_fFrame += 9.f * (fTimeDelta * 4.f);

	if (m_fFrame > 9)
	{
		m_bOff = true;
		m_bEnable = true;
		m_bDelete = true;
	}
}

void CEffect_ShotGun::LateTick(_float fTimeDelta)
{
	if (m_bOff)
		return;

	if (nullptr == m_pTransformCom ||
		nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, (CGameObject*)this);
}

HRESULT CEffect_ShotGun::Render()
{
	if (m_bOff)
		return S_OK;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_ShotGun::SetUp_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_ShotGun"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_ShotGun::SetUp_ShaderResource()
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
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_gVcolor, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_ShotGun * CEffect_ShotGun::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_ShotGun*		pInstance = new CEffect_ShotGun(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CEffect_ShotGun");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_ShotGun::Clone(void * pArg)
{
	CEffect_ShotGun*		pInstance = new CEffect_ShotGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_ShotGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_ShotGun::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
