#include "..\Public\UI.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
_float4x4	CUI::g_UIMatProj;


CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext),m_bMouseClicked(false), m_bMouseOver(false),
	m_pRendererCom(CRenderer::Create(pDevice, pDeviceContext))
{

}

HRESULT CUI::Initialize(void * arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	//UI들은 기본적으로 렌더러를 달고 생성된다.
	m_Components.emplace(L"Com_Renderer", m_pRendererCom);
	Safe_AddRef(m_pRendererCom);

	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{

}

void CUI::LateTick(_float fTimeDelta)
{
}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
	
}

void CUI::OnLButtonDown(void * pArg)
{
}

void CUI::OnLButtonUp(void * pArg)
{
}

void CUI::OnLButtonClicked(void * pArg)
{
}

void CUI::OnEvent(_uint iEventNum)
{
}


HRESULT CUI::LoadUIImage(const _tchar * TextureTag, _uint iLevel)
{

	CGameInstance* instance = GET_INSTANCE(CGameInstance);
	if (FAILED(__super::Add_Component(iLevel, TextureTag, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CUI::OnMouseOver(void * pArg)
{
}

void CUI::OnMouseOut(void * pArg)
{
}


