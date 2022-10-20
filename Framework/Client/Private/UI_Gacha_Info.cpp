#include "stdafx.h"
#include "..\Public\UI_Gacha_Info.h"
#include "Level_Loading.h"
#include <random>
#include "ImguiMgr.h"

CUI_Gacha_Info::CUI_Gacha_Info(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Gacha_Info::Initialize(void * pArg)
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

	m_eUIType = UI_TYPE::UI_BUTTTON;
	lstrcpy(m_szUIClass, TEXT("CUI_Gacha_Info"));

	m_fAlpha = 1.f;
	m_bAlpha = true;



	return S_OK;
}

HRESULT CUI_Gacha_Info::Initialization()
{
	__super::Initialization();

	if (!lstrcmp(m_szUIName, TEXT("SubInfo")))
	{
		m_pTransformCom->Set_Rotate(0.1f);
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), 0.3f);
	}

	return S_OK;
}

void CUI_Gacha_Info::Tick(_float fTimeDelta)
{
	if (m_bTickStop)
		return;

	__super::Tick(fTimeDelta);

	//if (m_bAlpha)
	//{
	//	if (1.f > m_fAlpha)
	//		m_fAlpha += fTimeDelta;
	//	else
	//		m_bFullAlpha = true;
	//}
	//else
	//{
	//	if (0.f < m_fAlpha)
	//		m_fAlpha -= fTimeDelta;
	//	else
	//		m_bFullAlpha = false;
	//}

	if (KEY(UP, HOLD))
	{
		if (1.f > m_fAlpha)
			m_fAlpha += fTimeDelta;
	}

	if (KEY(DOWN, HOLD))
	{
		if (0.f < m_fAlpha)
			m_fAlpha -= fTimeDelta;
	}

}

void CUI_Gacha_Info::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Gacha_Info::Render()
{
	if (!m_bRender)
		return S_OK;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* ���̴� ���������� ���� ������. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Gacha_Info::OnLButtonDown()
{
}

void CUI_Gacha_Info::OnLButtonUp()
{
}

void CUI_Gacha_Info::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);

	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}


HRESULT CUI_Gacha_Info::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fCharaNum)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_AlphaValue", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Gacha_Info::SetUp_Component()
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

CUI_Gacha_Info * CUI_Gacha_Info::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Gacha_Info*		pInstance = new CUI_Gacha_Info(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Gacha_Info");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Gacha_Info::Free()
{
	__super::Free();
}
