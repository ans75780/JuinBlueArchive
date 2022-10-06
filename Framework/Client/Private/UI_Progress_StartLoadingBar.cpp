#include "stdafx.h"
#include "..\Public\UI_Progress_StartLoadingBar.h"
#include "Level_Loading.h"
#include "VIBuffer_Point.h"
#include "Key_Manager.h"
#include "Engine_Defines.h"
#include "GameInstance.h"

CUI_Progress_StartLoadingBar::CUI_Progress_StartLoadingBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Progress_StartLoadingBar::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_BUTTTON;
	lstrcpy(m_szUIClass, TEXT("CUI_Progress_StartLoadingBar"));
	lstrcpy(m_szUIName, TEXT("Start_LoadingBar"));

	m_fPos = { -500.f,-280.f,0.f };
	m_fSize = { 1.f,1.f,1.f };

	m_fFrontBarSize = { 1.f,15.f};
	
	m_fFrontColor = { 75.f / 255.f, 206.f / 255.f, 255.f / 255.f, 1.f };


	return S_OK;
}

HRESULT CUI_Progress_StartLoadingBar::Initialization()
{
	m_fOriginSize = m_fSize;

	return S_OK;
}

void CUI_Progress_StartLoadingBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (0.f < m_fPlusValue)
	{
		if (!m_bLoadingSuccess)
		{
			m_fFrontBarSize.x += fTimeDelta * 20.f;
			m_fPlusValue -= fTimeDelta * 20.f;
		}
		else
		{
			m_fFrontBarSize.x += 1.f;
			m_fPlusValue -= 1.f;
		}

		if (1000.f < m_fFrontBarSize.x)
		{
			m_fFrontBarSize.x = 0.f;
			m_fPlusValue = 0.f;
		}
	}

}

void CUI_Progress_StartLoadingBar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Progress_StartLoadingBar::Render()
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

void CUI_Progress_StartLoadingBar::OnLButtonDown()
{
	
}

void CUI_Progress_StartLoadingBar::OnLButtonUp()
{
	
}

void CUI_Progress_StartLoadingBar::OnLButtonClicked()
{

}

void CUI_Progress_StartLoadingBar::Set_Size(_float3 _fSize)
{
	m_fOriginSize = _fSize;
	m_fSize = _fSize;
}

void CUI_Progress_StartLoadingBar::Set_ProgressWidth(_float fWidth)
{
}

void CUI_Progress_StartLoadingBar::Set_ProgressHeight(_float fHeight)
{

}

HRESULT CUI_Progress_StartLoadingBar::SetUp_Component()
{
	//UI들은 기본적으로 렌더러를 달고 생성된다.	
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_StartLoadingBar"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Point"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Progress_StartLoadingBar::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProgressBarSize", &m_fFrontBarSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_fFrontColor, sizeof(_float4))))
		return E_FAIL;

//	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
//		return E_FAIL;
	return S_OK;
}

CUI_Progress_StartLoadingBar * CUI_Progress_StartLoadingBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Progress_StartLoadingBar*		pInstance = new CUI_Progress_StartLoadingBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Progress_StartLoadingBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Progress_StartLoadingBar::Free()
{
	__super::Free();
}
