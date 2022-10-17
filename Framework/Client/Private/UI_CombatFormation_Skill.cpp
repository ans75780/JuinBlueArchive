#include "stdafx.h"
#include "UI_CombatFormation_Skill.h"
#include "UI_Progress.h"
#include "GameInstance.h"
#include "UI_Default.h"
CUI_CombatFormation_Skill::CUI_CombatFormation_Skill(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext)
	:m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	


}

HRESULT CUI_CombatFormation_Skill::Initialize(void * pArg)
{
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	m_pUICostFront = CUI_Default::Create(m_pDevice, m_pContext);
	m_pUICostBack = CUI_Default::Create(m_pDevice, m_pContext);
	
	m_fCost = 0.f;
	m_fCostMax = 10.f;

	m_pUICostFront->Set_Pos(_float3(350.f, -300.f, 0.f));
	m_pUICostFront->Set_Size(_float3(368.f, 18.f, 1.f));

	m_pUICostBack->Set_Pos(_float3(350.f, -300.f, 0.f));
	m_pUICostBack->Set_Size(_float3(368.f, 18.f, 1.f));


	m_pUICostFront->Set_UIType(UI_TYPE::UI_BUTTTON);
	m_pUICostBack->Set_UIType(UI_TYPE::UI_BACKGROUND);

	m_pUICostFront->SetPath(1);
	if (FAILED(m_pUICostFront->LoadUIImage(L"Prototype_Component_Texture_Combat_Cost_Gauge")))
	{
		return E_FAIL;

	}
	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUICostFront, nullptr)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pUICostBack->LoadUIImage(L"Prototype_Component_Texture_Combat_Cost_Gauge_Back")))
	{
		return E_FAIL;

	}
	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUICostBack, nullptr)))
	{
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CUI_CombatFormation_Skill::Tick(_float fTimeDelta)
{

	m_fCost += fTimeDelta * 0.1f;
	if (m_fCost >= m_fCostMax)
	{
		m_fCost = m_fCostMax;
	}
	m_pUICostFront->SetRatioWidth(m_fCost);

	if (KEY(A, TAP))
	{
		m_fCost -= 0.1f;
	}
	if (KEY(S, TAP))
	{
		m_fCost += 0.1f;
	}
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

	//Safe_Release(m_pUICostBack);
	//Safe_Release(m_pUICostFront);

}
