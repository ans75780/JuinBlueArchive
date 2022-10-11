#include "stdafx.h"
#include "..\Public\UI_Loading.h"
#include "Level_Loading.h"

#include "ImguiMgr.h"

CUI_Loading::CUI_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Loading::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_BACKGROUND;
	lstrcpy(m_szUIClass, TEXT("CUI_Loading"));


	return S_OK;
}

HRESULT CUI_Loading::Initialization()
{
	__super::Initialization();

	return S_OK;
}

void CUI_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Loading::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Loading::Render()
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

void CUI_Loading::OnLButtonDown()
{
}

void CUI_Loading::OnLButtonUp()
{
}

void CUI_Loading::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);
	
	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}


HRESULT CUI_Loading::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

CUI_Loading * CUI_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Loading*		pInstance = new CUI_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Loading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Loading::Free()
{
	__super::Free();
}
