#include "stdafx.h"
#include "..\Public\UI_UpWall.h"
#include "Level_Loading.h"
#include <random>
#include "ImguiMgr.h"

CUI_UpWall::CUI_UpWall(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_UpWall::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	ZeroMemory(m_szUIName, sizeof(_tchar) * MAX_PATH);
	ZeroMemory(m_szUITextureTag, sizeof(_tchar) * MAX_PATH);
	ZeroMemory(m_szUIClass, sizeof(_tchar) * MAX_PATH);

	if (pArg == nullptr)
	{
		TransformDesc.fSpeedPerSec = 0.f;
		TransformDesc.fRotationPerSec = XMConvertToRadians(0.0f);

		m_Components.emplace(m_pTransformTag, m_pTransformCom);

		ZeroMemory(&m_desc, sizeof(OBJ_DESC));

		Safe_AddRef(m_pTransformCom);

	}

	if (FAILED(this->SetUp_Component()))
		return E_FAIL;

	m_eUIType = UI_POST;
	lstrcpy(m_szUIClass, TEXT("CUI_UpWall"));

	m_fAlpha = 1.f;
	return S_OK;
}

HRESULT CUI_UpWall::Initialization()
{
	__super::Initialization();

	return S_OK;
}

void CUI_UpWall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if ( -836.f < m_fPos.x || 924.f > m_fPos.y )
	{
		m_fPos.x -= fTimeDelta * 2000.f;
		m_fPos.y += fTimeDelta * 2000.f;
	}


}

void CUI_UpWall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_UpWall::Render()
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

void CUI_UpWall::OnLButtonDown()
{
}

void CUI_UpWall::OnLButtonUp()
{
}

void CUI_UpWall::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);

	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}


HRESULT CUI_UpWall::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_AlphaValue", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_UpWall::SetUp_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxTex_Test"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

CUI_UpWall * CUI_UpWall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_UpWall*		pInstance = new CUI_UpWall(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_UpWall");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_UpWall::Free()
{
	__super::Free();
}
