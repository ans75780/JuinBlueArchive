#include "stdafx.h"
#include "UI_CombatFormation_Skill.h"
#include "UI_Progress.h"
#include "GameInstance.h"
#include "UI_Default.h"
#include "UI_Text.h"

CUI_CombatFormation_Skill::CUI_CombatFormation_Skill(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext)
	:m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CUI_CombatFormation_Skill::Initialize(void * pArg)
{
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	m_pUICostBarFront = CUI_Default::Create(m_pDevice, m_pContext);
	m_pUICostBarBack = CUI_Default::Create(m_pDevice, m_pContext);
	m_pUICostNumberBack = CUI_Default::Create(m_pDevice, m_pContext);
	m_pUICostText = CUI_Text::Create(m_pDevice, m_pContext);

	
	m_fCost = 0.f;
	m_fCostMax = 1.f;

	m_pUICostBarFront->Set_Pos(_float3(430.f, -320.f, 0.f));
	m_pUICostBarFront->Set_Size(_float3(368.f, 25.f, 1.f));

	m_pUICostBarBack->Set_Pos(_float3(430.f, -320.f, 0.f));
	m_pUICostBarBack->Set_Size(_float3(368.f, 25.f, 1.f));

	m_pUICostNumberBack->Set_Pos(_float3(200.f, -300.f, 0.f));
	m_pUICostNumberBack->Set_Size(_float3(80.f, 86.f, 1.f));

	//m_pUICostText->Set_Pos(_float3(175.f, -290.f, 0.f));

	m_pUICostText->Set_Pos(_float3(185.f, -290.f, 0.f));

	m_pUICostText->SetUITextColor(_float4(1.f, 1.f, 1.f, 1.f));
	m_pUICostText->SetUIScale(1.f);

	m_pUICostBarFront->Set_UIType(UI_TYPE::UI_BUTTTON);
	m_pUICostBarBack->Set_UIType(UI_TYPE::UI_BACKGROUND);
	m_pUICostBarFront->SetPath(1);

	m_pUICostNumberBack->Set_UIType(UI_TYPE::UI_BACKGROUND);
	m_pUICostText->Set_UIType(UI_TYPE::UI_BUTTTON);
	m_pUICostText->Initialization();
	if (FAILED(m_pUICostBarFront->LoadUIImage(L"Prototype_Component_Texture_Combat_Cost_Gauge")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUICostBarFront, nullptr)))
		return E_FAIL;

	if (FAILED(m_pUICostBarBack->LoadUIImage(L"Prototype_Component_Texture_Combat_Cost_Gauge_Back")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUICostBarBack, nullptr)))
		return E_FAIL;

	if (FAILED(m_pUICostNumberBack->LoadUIImage(L"Prototype_Component_Texture_Combat_CostBg")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUICostNumberBack, nullptr)))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUICostText, nullptr)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CUI_CombatFormation_Skill::Tick(_float fTimeDelta)
{
	m_fCost += fTimeDelta * 0.05f;
	
	if (m_fCost >= 1.f)
		m_fCost = 1.f;
	if (m_fCost >= m_fCostMax)
	{
		m_pUICostText->Set_Pos(_float3(175.f, -290.f, 0.f));
		m_fCost = m_fCostMax;
	}
	else
		m_pUICostText->Set_Pos(_float3(185.f, -290.f, 0.f));

	_itow_s((_uint)(m_fCost * 10.f), m_szCost, 10);
	m_pUICostText->SetUIText(m_szCost);	
	m_pUICostBarFront->SetRatioWidth(m_fCost);

}

void CUI_CombatFormation_Skill::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_CombatFormation_Skill::Render()
{
	return S_OK;
}

HRESULT CUI_CombatFormation_Skill::StartGame()
{
	m_fCost = 0.f;
	m_pUICostBarFront->SetRatioWidth(m_fCost);

	return S_OK;
}

CUI_CombatFormation_Skill * CUI_CombatFormation_Skill::Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext)
{
	CUI_CombatFormation_Skill*	pInstance = new CUI_CombatFormation_Skill(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

void CUI_CombatFormation_Skill::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	pInstance->Clear_Canvas(LEVEL_GAMEPLAY);

	RELEASE_INSTANCE(CGameInstance);
}
