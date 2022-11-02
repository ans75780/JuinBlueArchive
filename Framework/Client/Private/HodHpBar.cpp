#include "stdafx.h"
#include "..\Public\HodHpBar.h"
#include "Level_Loading.h"
#include "VIBuffer_Point.h"
#include "Key_Manager.h"
#include "Engine_Defines.h"
#include "GameInstance.h"

CHodHpBar::CHodHpBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CHodHpBar::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_BUTTTON;
	lstrcpy(m_szUIClass, TEXT("CHodHpBar"));
	lstrcpy(m_szUIName, TEXT("Hod_Hp_Bar"));

	m_fPos = { -300.f, 300.f,0.f };
	m_fSize = { 1.f,1.f,1.f };
	m_fOriginPos = m_fPos;
	m_fFrontBarSize = { 600.f,25.f };

	m_fFrontColor = { 255.f / 255.f, 127.f / 255.f, 39.f / 255.f, 1.f };

	m_fResetPos = m_fPos;

	return S_OK;
}

HRESULT CHodHpBar::Initialization()
{
	m_fOriginSize = m_fSize;

	return S_OK;
}

void CHodHpBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (0.f < m_fMinusValue)
	{
		Shake();
		m_fFrontBarSize.x -= 100.f * fTimeDelta;
		m_fMinusValue -= 100.f * fTimeDelta;
	

		if (1.f > m_fFrontBarSize.x)
		{
			m_fFrontBarSize.x = 0.f;
			m_fMinusValue = 0.f;
		}
	}
	else
	{
		m_fPos = m_fResetPos;
		m_uShakeCount = 0;
	}

}

void CHodHpBar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CHodHpBar::Render()
{
	if (!m_bRender)
		return S_OK;

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

void CHodHpBar::OnLButtonDown()
{

}

void CHodHpBar::OnLButtonUp()
{

}

void CHodHpBar::OnLButtonClicked()
{

}

void CHodHpBar::Set_Size(_float3 _fSize)
{
	m_fOriginSize = _fSize;
	m_fSize = _fSize;
}

void CHodHpBar::Set_ProgressWidth(_float fWidth)
{
}

void CHodHpBar::Set_ProgressHeight(_float fHeight)
{

}

void CHodHpBar::Shake()
{
	if (6 < m_uShakeCount)
		m_uShakeCount = 0;

	m_uShakeCount++;



	if (3 >= m_uShakeCount)
	{
		m_fPos.x = m_fResetPos.x + 2.f;
		m_fPos.y = m_fResetPos.y - 1.f;
	}
	else
	{
		m_fPos.x = m_fResetPos.x - 2.f;
		m_fPos.y = m_fResetPos.y + 1.f;
	}

}

HRESULT CHodHpBar::SetUp_Component()
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

HRESULT CHodHpBar::SetUp_ShaderResource()
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

CHodHpBar * CHodHpBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHodHpBar*		pInstance = new CHodHpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CHodHpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHodHpBar::Free()
{
	__super::Free();
}
