#include "stdafx.h"
#include "..\Public\BackGround.h"
#include "GameInstance.h"
#include "UI.h"
#include "Transform_Utils.h"
CBackGround::CBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CBackGround::Initialize(void * pArg)
{
	
	if (FAILED(__super::Initialize()))
		return E_FAIL;
		
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_eUIType = UI_BACKGROUND;

	m_fPos = _float3(0.f, 0.f, 0.f);
	m_fSize = _float3(200, 200, 1.f);
	
	//m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
	
	return S_OK;
}

void CBackGround::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBackGround::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CBackGround::Render()
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

void CBackGround::OnLButtonDown()
{
	m_fSize = { 100.f, 100.f, 1.f };

}

void CBackGround::OnLButtonUp()
{
	m_fSize = { 300.f, 300.f, 1.f };

}

void CBackGround::OnLButtonClicked()
{
	int a = 10;
}

HRESULT CBackGround::SetUp_Components()
{
	return S_OK;
}

HRESULT CBackGround::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround*		pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CBackGround");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}
void CBackGround::Free()
{
	__super::Free();
}
