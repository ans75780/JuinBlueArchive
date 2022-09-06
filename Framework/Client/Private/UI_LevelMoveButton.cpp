#include "stdafx.h"
#include "..\Public\UI_LevelMoveButton.h"

CUI_LevelMoveButton::CUI_LevelMoveButton(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_LevelMoveButton::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_eUIType = UI_BUTTTON;
	lstrcpy(m_szUIClass, TEXT("CUI_LevelMoveButton"));

	m_fPos = _float3(0.f, 0.f, 0.f);
	m_fSize = _float3(100.f, 100.f, 1.f);

	return S_OK;
}

void CUI_LevelMoveButton::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_LevelMoveButton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_LevelMoveButton::Render()
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

void CUI_LevelMoveButton::OnLButtonDown()
{
}

void CUI_LevelMoveButton::OnLButtonUp()
{
}

void CUI_LevelMoveButton::OnLButtonClicked()
{
}

HRESULT CUI_LevelMoveButton::SetUp_Components()
{
	return S_OK;
}

HRESULT CUI_LevelMoveButton::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

CUI_LevelMoveButton * CUI_LevelMoveButton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_LevelMoveButton*		pInstance = new CUI_LevelMoveButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LevelMoveButton::Free()
{
	__super::Free();
}
