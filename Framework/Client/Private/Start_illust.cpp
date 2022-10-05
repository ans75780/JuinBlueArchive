#include "stdafx.h"
#include "..\Public\Start_illust.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"

CStart_illust::CStart_illust(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

HRESULT CStart_illust::Initialize(void * pArg)
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

	m_fSize.x = 1440.f;
	m_fSize.y = 810.f;
	m_fSize.z = 1.f;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	_vector vPos;
	m_pTransformCom->Set_Scaled(m_fSize);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		vPos = XMVectorSet(m_fPos.x, m_fPos.y, 0.91f, 1.f));

	XMStoreFloat3(&m_fPos, vPos);

	return S_OK;
}

HRESULT CStart_illust::initialization()
{
	return S_OK;
}

void CStart_illust::Tick(_float fTimeDelta)
{
	static _float num = 0.f;

	num += fTimeDelta;

	FrameCheck(fTimeDelta);

	if (num > 8.f)
	{
		m_fPos.x = 0.f;
		m_fPos.y = 0.f;
		m_fSize.x = 1440.f;
		m_fSize.y = 810.f;

		m_fFrame++;

		num = 0.f;

		if (16.f <= m_fFrame)
			m_fFrame = 0.f;
	}
}

void CStart_illust::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return;

	_vector vPos;
	m_pTransformCom->Set_Scaled(m_fSize);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos = XMVectorSet(m_fPos.x, m_fPos.y, 0.91f, 1.f));
	XMStoreFloat3(&m_fPos, vPos);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, (CGameObject*)this);
}

HRESULT CStart_illust::Render()
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

HRESULT CStart_illust::SetUp_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_LOADING_START, TEXT("Prototype_Component_Texture_Startillust"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStart_illust::SetUp_ShaderResource()
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

void CStart_illust::FrameCheck(_float & _Delta)
{
	if (m_fFrame < 1.f)			//0
		Move_Right(_Delta);
	else if (m_fFrame < 2.f)	//1
		Move_ZoomOut(_Delta);
	else if (m_fFrame < 3.f)	//2
		Move_Right(_Delta);
	else if (m_fFrame < 4.f)
		Move_ZoomIn(_Delta);
	else if (m_fFrame < 5.f)
		Move_Left(_Delta);
	else if (m_fFrame < 6.f)
		Move_Right(_Delta);
	else if (m_fFrame < 7.f)
		Move_ZoomOut(_Delta);
	else if (m_fFrame < 8.f)
		Move_Down(_Delta);
	else if (m_fFrame < 9.f)
		Move_Right(_Delta);
	else if (m_fFrame < 10.f)
		Move_ZoomOut(_Delta);
	else if (m_fFrame < 11.f)
		Move_Left(_Delta);
	else if (m_fFrame < 12.f)
		Move_Right(_Delta);
	else if (m_fFrame < 13.f)
		Move_Left(_Delta);
	else if (m_fFrame < 14.f)
		Move_Right(_Delta);
	else if (m_fFrame < 15.f)
		Move_Right(_Delta);
	else if (m_fFrame < 16.f)
		Move_Down(_Delta);
}

void CStart_illust::Move_Right(_float & _Delta)
{
	m_fPos.x += _Delta * 3.f;
}

void CStart_illust::Move_Left(_float & _Delta)
{
	m_fPos.x -= _Delta * 3.f;
}

void CStart_illust::Move_Down(_float & _Delta)
{
	m_fPos.y -= _Delta * 2.5f;
}

void CStart_illust::Move_ZoomIn(_float & _Delta)
{
	m_fSize.x += _Delta * 10.f;
	m_fSize.y += _Delta * 5.625f;
}

void CStart_illust::Move_ZoomOut(_float & _Delta)
{
	m_fSize.x -= _Delta * 10.f;
	m_fSize.y -= _Delta * 5.625f;
}

CStart_illust * CStart_illust::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CStart_illust*		pInstance = new CStart_illust(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CStart_illust");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStart_illust::Clone(void * pArg)
{
	CStart_illust*		pInstance = new CStart_illust(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStart_illust");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStart_illust::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
