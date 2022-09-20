#include "stdafx.h"
#include "..\Public\UI_Text.h"
#include "Level_Loading.h"

#include "ImguiMgr.h"

CUI_Text::CUI_Text(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Text::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	lstrcpy(m_szUIClass, TEXT("CUI_Text"));
	ZeroMemory(m_szText, MAX_PATH);
	m_fColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_fScale = 1.f;

	return S_OK;
}

HRESULT CUI_Text::initialization()
{
	return S_OK;
}

void CUI_Text::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Text::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Text::Render()
{
	//if (nullptr == m_pShaderCom ||
	//	nullptr == m_pVIBufferCom)
	//	return E_FAIL;

	///* 셰이더 전역변수에 값을 던진다. */
	//if (FAILED(SetUp_ShaderResource()))
	//	return E_FAIL;

	//m_pShaderCom->Begin(0);
	//m_pVIBufferCom->Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float2 Pos;
	Pos.x = m_fPos.x + (g_iWinCX * 0.5f);
	Pos.y = m_fPos.y + (g_iWinCY * 0.5f);
	
	pGameInstance->Render_Font(TEXT("Font_Default"), m_szText, Pos, XMLoadFloat4(&m_fColor), m_fScale);
	//m_pGameInstance->Render_Font(TEXT("Font_Default"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.f);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CUI_Text::OnLButtonDown()
{
}

void CUI_Text::OnLButtonUp()
{

}

void CUI_Text::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);
	
	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}

HRESULT CUI_Text::SetUp_Components()
{
	return S_OK;
}

HRESULT CUI_Text::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

CUI_Text * CUI_Text::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Text*		pInstance = new CUI_Text(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Text");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Text::Free()
{
	__super::Free();
}
