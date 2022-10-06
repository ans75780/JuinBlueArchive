#include "stdafx.h"
#include "..\Public\BG_Gacha_Video.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Texture.h"
#include "Renderer.h"

CBG_Gacha_Video::CBG_Gacha_Video(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

HRESULT CBG_Gacha_Video::Initialize(void * pArg)
{
	XMStoreFloat4x4(&m_UIMatProj,
		XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 3.f)));

	CTransform::TRANSFORMDESC		TransformDesc;

	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(0.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fPos.x = -320.f;
	m_fPos.y = 0.f;
	m_fPos.z = 0.f;

	m_fSize.x = 714.f;
	m_fSize.y = 720.f;
	m_fSize.z = 1.f;

	if (nullptr == m_pTransformCom)
		return E_FAIL;
	_vector vPos;
	m_pTransformCom->Set_Scaled(m_fSize);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos = XMVectorSet(m_fPos.x, m_fPos.y, 0.91f, 1.f));

	XMStoreFloat3(&m_fPos, vPos);

	return S_OK;
}

HRESULT CBG_Gacha_Video::Initialization()
{
	return S_OK;
}

void CBG_Gacha_Video::Tick(_float fTimeDelta)
{
	m_fFrame += 532.f * (fTimeDelta * 0.05f);

	if (m_fFrame >= 532.f)
		m_fFrame = 0.f;
}

void CBG_Gacha_Video::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIBG, (CGameObject*)this);
}

HRESULT CBG_Gacha_Video::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBG_Gacha_Video::SetUp_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GACHA, TEXT("Prototype_Component_Texture_Gacha_Pv"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBG_Gacha_Video::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;

	return S_OK;
}

CBG_Gacha_Video * CBG_Gacha_Video::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBG_Gacha_Video*		pInstance = new CBG_Gacha_Video(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CBG_Gacha_Video");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBG_Gacha_Video::Clone(void * pArg)
{
	CBG_Gacha_Video*		pInstance = new CBG_Gacha_Video(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBG_Gacha_Video");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBG_Gacha_Video::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
