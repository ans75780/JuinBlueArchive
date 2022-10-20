#include "stdafx.h"
#include "..\Public\UI_GachaCard.h"
#include "Level_Loading.h"
#include <random>
#include "ImguiMgr.h"



CUI_GachaCard::CUI_GachaCard(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_GachaCard::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_BACKGROUND;
	lstrcpy(m_szUIClass, TEXT("CUI_GachaCard"));

	static	_uint	CardShuffle[10] = { 1, 1, 2, 2, 2, 3, 3, 3, 3, 3 };
	static _bool Shuffle = true;
	if (Shuffle)
	{
		random_device _rd;
		mt19937_64 _random(_rd());

		uniform_int_distribution<__int64> _range(0, 9);

		for (int i = 0; i < 10; ++i)
		{
			_uint R = (_uint)(_range(_random));

			uint  temp = CardShuffle[i];
			CardShuffle[i] = CardShuffle[R];
			CardShuffle[R] = temp;
		}
		Shuffle = false;
	}

	for (int i = 0; i < 10; i++)
	{
		if (0 == CardShuffle[i])
			continue;
		
		if (1 == CardShuffle[i])
			m_fCardNum = 2.f;
		else if (2 == CardShuffle[i])
			m_fCardNum = 1.f;
		else if (3 == CardShuffle[i])
			m_fCardNum = 0.f;

		CardShuffle[i] = 0;
		break;
	}

	m_pTransformCom->Set_Rotate(0.1f);
	m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), 1.f);

	return S_OK;
}

HRESULT CUI_GachaCard::Initialization()
{
	__super::Initialization();

	return S_OK;
}

void CUI_GachaCard::Tick(_float fTimeDelta)
{
	if (m_bTickStop)
		return;

	__super::Tick(fTimeDelta * 3.f);
}

void CUI_GachaCard::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_GachaCard::Render()
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

void CUI_GachaCard::OnLButtonDown()
{
}

void CUI_GachaCard::OnLButtonUp()
{
}

void CUI_GachaCard::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);

	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}


HRESULT CUI_GachaCard::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fCardNum)))
		return E_FAIL;

	return S_OK;
}

CUI_GachaCard * CUI_GachaCard::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_GachaCard*		pInstance = new CUI_GachaCard(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_GachaCard");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_GachaCard::Free()
{
	__super::Free();
}
