#include "stdafx.h"
#include "..\Public\UI_Frame.h"
#include "Level_Loading.h"

#include "ImguiMgr.h"

CUI_Frame::CUI_Frame(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Frame::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_BACKGROUND;
	lstrcpy(m_szUIClass, TEXT("CUI_Frame"));

	m_fFrame = 0.f;
	m_fMaxFrame = 0.f;
	m_fDelay = 0.f;

	return S_OK;
}

HRESULT CUI_Frame::Initialization()
{
	__super::Initialization();

	return S_OK;
}

void CUI_Frame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_fMaxFrame == 0.f)
		return;

	m_fFrame += m_fMaxFrame * (fTimeDelta * m_fDelay);

	if (m_fFrame >= m_fMaxFrame)
		m_fFrame = 0.f;

}

void CUI_Frame::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Frame::Render()
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

void CUI_Frame::OnLButtonDown()
{
}

void CUI_Frame::OnLButtonUp()
{
}

void CUI_Frame::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);
	
	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}


HRESULT CUI_Frame::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;

	return S_OK;
}

CUI_Frame * CUI_Frame::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Frame*		pInstance = new CUI_Frame(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Frame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Frame::Free()
{
	__super::Free();
}
