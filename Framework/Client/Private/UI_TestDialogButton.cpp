#include "stdafx.h"
#include "UI_TestDialogButton.h"
#include "GameInstance.h"
#include "UI.h"
#include "Transform_Utils.h"
CUI_TestDialogButton ::CUI_TestDialogButton (ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_TestDialogButton ::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_eUIType = UI_DIALOG;

	m_fPos = _float3(0.f, -150.f, 0.f);
	m_fSize = _float3(200, 100, 1.f);

	//m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,

	return S_OK;
}

void CUI_TestDialogButton ::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_TestDialogButton ::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
	Compute_Transform();
}

HRESULT CUI_TestDialogButton ::Render()
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

void CUI_TestDialogButton ::OnLButtonDown()
{


}

void CUI_TestDialogButton ::OnLButtonUp()
{


}

void CUI_TestDialogButton ::OnLButtonClicked()
{
	if (m_pParent)
	{
		m_pParent->OnEvent(3);
	}
}

HRESULT CUI_TestDialogButton ::SetUp_Components()
{
	return S_OK;
}

HRESULT CUI_TestDialogButton ::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

CUI_TestDialogButton * CUI_TestDialogButton ::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_TestDialogButton *		pInstance = new CUI_TestDialogButton (pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_TestDialogButton ");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CUI_TestDialogButton ::Free()
{
	__super::Free();
}
