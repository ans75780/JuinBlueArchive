#include "stdafx.h"
#include "UI_TestDialog.h"
#include "Transform.h"
#include "UI_TestDialogButton.h"
CUI_TestDialog::CUI_TestDialog(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CUI(pDevice, pContext)
{

}

HRESULT CUI_TestDialog::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_DIALOG;
	m_fPos = { 0,0,0.0f };
	m_fSize = { 500.f, 500.f, 1.f };

	CUI_TestDialogButton* pButton = CUI_TestDialogButton::Create(m_pDevice, m_pContext);
	pButton->LoadUIImage(L"UI_TestDialogButton");
	Add_Child(pButton);

	return S_OK;
}

void CUI_TestDialog::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto& child : m_vecChild)
		child->Tick(fTimeDelta);
}

void CUI_TestDialog::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	for (auto& child : m_vecChild)
	{
		child->LateTick(fTimeDelta);
	}
}

HRESULT CUI_TestDialog::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;

	return S_OK;
}

CUI * CUI_TestDialog::Get_MouseOveredUI(const POINT&	_pt)
{
	CUI*	pMouseOveredUI = nullptr;
	for (auto& child : m_vecChild)
	{
		pMouseOveredUI = child->Get_MouseOveredUI(_pt);
		if (pMouseOveredUI)
			return pMouseOveredUI;
	}
	pMouseOveredUI = CUI::Get_MouseOveredUI(_pt);

	return pMouseOveredUI;
}

void CUI_TestDialog::OnLButtonDown()
{
}

void CUI_TestDialog::OnLButtonUp()
{
}

void CUI_TestDialog::OnLButtonClicked()
{
	int a = 10;
}

void CUI_TestDialog::OnEvent(_uint iEventNum)
{
	if (iEventNum == 3)
		int i = 10;
}

HRESULT CUI_TestDialog::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;
	
	return S_OK;
}

CUI_TestDialog * CUI_TestDialog::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_TestDialog*	pInstance = new CUI_TestDialog(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CUI_TestDialog::Free()
{
	__super::Free();
}
