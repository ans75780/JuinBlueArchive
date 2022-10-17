#include "stdafx.h"
#include "..\Public\UI_Progress.h"
#include "Level_Loading.h"
#include "VIBuffer_Point.h"
#include "Key_Manager.h"
#include "ImguiMgr.h"
#include "Engine_Defines.h"
#include "GameInstance.h"

CUI_Progress::CUI_Progress(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Progress::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_BUTTTON;
	lstrcpy(m_szUIClass, TEXT("CUI_Progress"));


	m_fPos = { 0.f,0.f,0.f };
	m_fSize = { 1.f,1.f,1.f };

	m_fProgressBarSize = { 100.f,100.f};
	
	m_fOriginWidth = 0.f;
	m_fOriginHeight = 0.f;

	m_fRatio = 1.f;

	return S_OK;
}

HRESULT CUI_Progress::initialization()
{
	m_fOriginSize = m_fSize;

	return S_OK;
}

void CUI_Progress::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//TestCode
		
}

void CUI_Progress::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Progress::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPathNum);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Progress::OnLButtonDown()
{
	
}

void CUI_Progress::OnLButtonUp()
{
	
}

void CUI_Progress::OnLButtonClicked()
{

}

void CUI_Progress::Set_Size(_float3 _fSize)
{
	m_fOriginSize = _fSize;
	m_fSize = _fSize;
}

void CUI_Progress::Set_ProgressWidth(_float fWidth)
{
	m_fProgressBarSize.x = fWidth;
	

	if (m_fOriginWidth == 0.f)
	{
		m_fOriginWidth = fWidth;
	}

}

void CUI_Progress::Set_ProgressHeight(_float fHeight)
{
	m_fProgressBarSize.y = fHeight;

	if (m_fOriginHeight == 0.f)
	{
		m_fOriginHeight = fHeight;
	}

}

HRESULT CUI_Progress::SetUp_Component()
{
	//UI들은 기본적으로 렌더러를 달고 생성된다.	
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_PointToProgress"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Point"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Progress::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProgressBarSize", &m_fProgressBarSize, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Ratio", &m_fRatio, sizeof(_float))))
		return E_FAIL;

	if (m_pTextureCom)
	{
		if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;
	}
	return S_OK;
}

CUI_Progress * CUI_Progress::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Progress*		pInstance = new CUI_Progress(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Progress");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Progress::Free()
{
	__super::Free();
}
