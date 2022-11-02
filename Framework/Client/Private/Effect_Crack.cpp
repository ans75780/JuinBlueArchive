#include "stdafx.h"
#include "..\Public\Effect_Crack.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"

#include "Camera_Free.h"
#include "Charater.h"
#include "Hod.h"


CEffect_Crack::CEffect_Crack(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

HRESULT CEffect_Crack::Initialize(void * pArg)
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
	

	m_pTransformCom->Set_Scaled(_float3(5.f, 5.f, 5.f));
	m_gVcolor.x = 40.f / 255.f;
	m_gVcolor.y = 40.f / 255.f;
	m_gVcolor.z = 40.f / 255.f;
	m_gVcolor.w = 255.f / 255.f;

	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMConvertToRadians(90.f));
	return S_OK;
}

void CEffect_Crack::Tick(_float fTimeDelta)
{
	if (m_bOff)
		return;

	m_fLifeTime	+= fTimeDelta;

	if (m_fLifeTime > 2.f)
	{
		if(m_gVcolor.w > 0.01f)
			m_gVcolor.w -= fTimeDelta * 0.5f;
	}

	if (m_fLifeTime > 4.f)
	{
		m_bOff = true;
		m_bEnable = true;
		m_bDelete = true;
	}
}

void CEffect_Crack::LateTick(_float fTimeDelta)
{
	if (m_bOff)
		return;

	if (nullptr == m_pTransformCom ||
		nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIBG, (CGameObject*)this);
}

HRESULT CEffect_Crack::Render()
{
	if (m_bOff)
		return S_OK;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(4);
	m_pVIBufferCom->Render();

	return S_OK;
}

_float3 CEffect_Crack::Get_BgPos()
{
	_float4 fPos;
	XMStoreFloat4(&fPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	return _float3(0.f,0.f, fPos.z);
}

HRESULT CEffect_Crack::SetUp_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Crack"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Crack::SetUp_ShaderResource()
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
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_gVcolor, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_Crack * CEffect_Crack::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_Crack*		pInstance = new CEffect_Crack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CEffect_Crack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Crack::Clone(void * pArg)
{
	CEffect_Crack*		pInstance = new CEffect_Crack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Crack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Crack::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
