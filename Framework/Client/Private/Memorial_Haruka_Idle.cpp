#include "stdafx.h"
#include "..\Public\Memorial_Haruka_Idle.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"

CMemorial_Haruka_Idle::CMemorial_Haruka_Idle(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

HRESULT CMemorial_Haruka_Idle::Initialize(void * pArg)
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


	m_fPos.x = 0.f;
	m_fPos.y = 0.f;
	m_fPos.z = 0.f;

	m_fSize.x = 1280.f;
	m_fSize.y = 720.f;
	m_fSize.z = 1.f;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	_vector vPos;
	m_pTransformCom->Set_Scaled(m_fSize);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		vPos = XMVectorSet(m_fPos.x, m_fPos.y, 0.91f, 1.f));

	XMStoreFloat3(&m_fPos, vPos);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Set_DisableUI(false);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMemorial_Haruka_Idle::Initialization()
{
	return S_OK;
}

void CMemorial_Haruka_Idle::Tick(_float fTimeDelta)
{
	m_fFrame += 209.f * (fTimeDelta * 0.08f);

	if (m_fFrame >= 209.f)
		m_fFrame = 0.f;
}

void CMemorial_Haruka_Idle::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, (CGameObject*)this);
}

HRESULT CMemorial_Haruka_Idle::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMemorial_Haruka_Idle::SetUp_Components()
{
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Memorial_Haruka_Idle"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMemorial_Haruka_Idle::SetUp_ShaderResource()
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

CMemorial_Haruka_Idle * CMemorial_Haruka_Idle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMemorial_Haruka_Idle*		pInstance = new CMemorial_Haruka_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CMemorial_Haruka_Idle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMemorial_Haruka_Idle::Clone(void * pArg)
{
	CMemorial_Haruka_Idle*		pInstance = new CMemorial_Haruka_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMemorial_Haruka_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMemorial_Haruka_Idle::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
