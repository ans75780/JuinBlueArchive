#include "..\Public\UI_Warning.h"
#include "UI.h"
#include "UI_Default.h"
#include "Engine_Defines.h"
#include "GameObject.h"
#include "Renderer.h"

CUI_Warning::CUI_Warning(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

HRESULT CUI_Warning::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Warning::Initialize(void * pArg)
{
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);


	m_pWarning = CUI_Default::Create(m_pDevice, m_pContext);

	m_pWarning->Set_Pos(_float3(0.f, 0.f, 0.f));
	m_pWarning->Set_Size(_float3(512.f, 256.f, 1.f));
	m_pWarning->Set_UIType(UI_TYPE::UI_POST);
	m_pWarning->LoadUIImage(L"Prototype_Component_Texture_FX_TEX_Warning_Font_01");
	
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CUI_Warning::Tick(_float fTimeDelta)
{
	if (m_fAlphaCurrent < m_fAlphaStart)
		m_fAlphaCurrent += m_fAlphaTickForAmount * fTimeDelta;
	else if (m_fAlphaCurrent > m_fAlphaEnd)
		m_fAlphaCurrent -= m_fAlphaTickForAmount * fTimeDelta;
}

void CUI_Warning::LateTick(_float fTimeDelta)
{
}

CUI_Warning * CUI_Warning::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	return nullptr;
}

void CUI_Warning::Free()
{
}
