#include "stdafx.h"
#include "UI_CombatFormation_Skill.h"
#include "UI_Progress.h"
#include "GameInstance.h"
#include "UI_Default.h"
#include "UI_Text.h"
#include "UI_BossHpBar.h"
#include "Actor.h"
#include "Enemy.h"
#include "Shader.h"
CUI_BossHpBar::CUI_BossHpBar(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext)
	:m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CUI_BossHpBar::Initialize(void * pArg)
{
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

	m_pHpBarGray = CUI_Default::Create(m_pDevice, m_pContext);
	//m_pHpBarColor = CUI_Default::Create(m_pDevice, m_pContext);
	//m_pHpBarFrame = CUI_Default::Create(m_pDevice, m_pContext);

	


	m_pHpBarGray->Set_Pos(_float3(0, -150, 0.f));
	m_pHpBarGray->Set_Size(_float3(700, 40, 0.f));
	m_pHpBarGray->SetPath(2);
	m_pHpBarGray->Set_UIType(UI_TYPE::UI_BACKGROUND2);
	//m_pUIBossIcon->Set_Pos(_float3(-700, -150, 0.f));
	//m_pUIBossIcon->Set_Size(_float3(80, 80, 0.f));


	

	//m_pUIBossIcon->Set_UIType(UI_TYPE::UI_BACKGROUND);
	//m_pUICostText->Set_UIType(UI_TYPE::UI_BUTTTON);
	//m_pUICostText->Initialization();


	if (FAILED(m_pHpBarGray->LoadUIImage(L"Prototype_Component_Texture_UI_BossHpBarBack")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pHpBarGray, nullptr)))
		return E_FAIL;

//	if (FAILED(m_pUIBossIcon->LoadUIImage(L"Prototype_Component_Texture_UI_BossIcon")))
//		return E_FAIL;
//	if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUIBossIcon, nullptr)))
	//	return E_FAIL;


	//if (FAILED(pInstance->Add_UI(LEVEL_GAMEPLAY, m_pUICostText, nullptr)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CUI_BossHpBar::Tick(_float fTimeDelta)
{
	
	
}

void CUI_BossHpBar::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_BossHpBar::Render()
{
	CShader*	pShader = (CShader*)m_pHpBarGray->Get_Component(L"Com_Shader");
	pShader->Set_RawValue("g_Color", &_float4(1.f, 1.f, 1.f, 1.f), sizeof(_float4));
	pShader->Set_RawValue("g_AlphaColor", &_float4(0.4, 0.4, 0.4, 0.4f), sizeof(_float4));

	return S_OK;
}

HRESULT CUI_BossHpBar::StartGame()
{
	
	return S_OK;
}

CUI_BossHpBar * CUI_BossHpBar::Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext, CActor * pActor)
{
	CUI_BossHpBar*	pInstance = new CUI_BossHpBar(_pDevice, _pContext);
	pInstance->m_pBoss = pActor;
	if (FAILED(pInstance->Initialize(nullptr)))
	{
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

void CUI_BossHpBar::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);



	RELEASE_INSTANCE(CGameInstance);
}
