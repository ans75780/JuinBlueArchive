#include "..\Public\UI_Canvas.h"
#include "GameInstance.h"
#include "UI.h"
#include "Key_Manager.h"
#include "Engine_Macro.h"

CUI_Canvas::CUI_Canvas(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CUI_Canvas * CUI_Canvas::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Canvas*	pInstance = new CUI_Canvas(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Initialize Error : CUI_Canvas");
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CUI_Canvas::Free()
{
	for (auto& vecUI : m_vecUI)
	{
		for (auto& UI : vecUI)
		{
			Safe_Release(UI);
		}
		vecUI.clear();
	}
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}

void CUI_Canvas::Add_UI(CUI * pUI)
{
	UI_TYPE	 eUIType = pUI->Get_UIType();
	m_vecUI[eUIType].push_back(pUI);
}

void CUI_Canvas::Clear_UIVec()
{
	for (auto& vecUI : m_vecUI)
	{
		if (vecUI.empty())
			continue;
		for (auto& UI : vecUI)
		{
			Safe_Release(UI);
		}
		vecUI.clear();
	}
}

void CUI_Canvas::AllUICanvas_Initialization()
{
	for (auto& vecUI : m_vecUI)
	{
		for (auto& UI : vecUI)
		{
			UI->initialization();
		}
	}
}

HRESULT CUI_Canvas::Initialize()
{
	m_bOnDialog = false;


	return S_OK;
}

void CUI_Canvas::Tick(_float fTimeDelta)
{
	m_bEventCurFrame = false;

	_uint uCnt;
	for (auto& vecUI : m_vecUI)
	{
		uCnt = 0;
		for (auto& UI : vecUI)
		{
			if (!UI->Get_Enable())
				continue;
			if (UI->m_bDead)
			{
				UI->Free();
				vecUI.erase(vecUI.begin() + uCnt);
				break;
			}

			UI->Tick(fTimeDelta);
			if (m_bEventCurFrame == false)
			{
				Check_UI(UI);
			}
			uCnt++;
		}
	}
}

void CUI_Canvas::LateTick(_float fTimeDelta)
{
	for (auto& vecUI : m_vecUI)
	{
		for (auto& UI : vecUI)
		{
			if (!UI->Get_Enable())
				continue;
			UI->LateTick(fTimeDelta);
			UI->Compute_Transform();
		}
	}
}

HRESULT CUI_Canvas::Render()
{

	return S_OK;
}

void CUI_Canvas::Check_UI(CUI * pUI)
{
	POINT pt = GETMOUSEPOS;

	CUI*	pMouseOveredUI = nullptr;

	if (KEY(LBUTTON, TAP))
	{
		pMouseOveredUI = pUI->Get_MouseOveredUI(pt);
		if (pMouseOveredUI == nullptr)
			return;
		if (nullptr != pMouseOveredUI)
		{
			pMouseOveredUI->m_bMouseClicked = true;
			pMouseOveredUI->OnLButtonDown();
			m_bEventCurFrame = true;
		}
	}
	else if (KEY(LBUTTON, AWAY))
	{
		pMouseOveredUI = pUI->Get_MouseOveredUI(pt);
		if (pMouseOveredUI == nullptr)
			return;
		m_bEventCurFrame = true;
		pMouseOveredUI->m_bMouseClicked = false;
		pMouseOveredUI->OnLButtonUp();
		if (nullptr != pMouseOveredUI)
		{
			pMouseOveredUI->OnLButtonClicked();
			pMouseOveredUI->m_bClickTogle = !pMouseOveredUI->m_bClickTogle;//클릭 토글
		}
	}
}
