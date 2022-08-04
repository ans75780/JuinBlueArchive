#include "..\Public\UI_Canvas.h"
#include "GameInstance.h"
#include "UI.h"



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
	for (auto vecUI : m_vecUI)
	{
		for (auto UI : vecUI)
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

HRESULT CUI_Canvas::Initialize()
{
	m_bOnDialog = false;


	return S_OK;
}

void CUI_Canvas::Tick(_float fTimeDelta)
{
	for (auto vecUI : m_vecUI)
	{
		for (auto UI : vecUI)
		{
			if (!UI->Get_Enable())
				continue;
			UI->Tick(fTimeDelta);
		}
	}
}

void CUI_Canvas::LateTick(_float fTimeDelta)
{
	for (auto vecUI : m_vecUI)
	{
		for (auto UI : vecUI)
		{
			if (!UI->Get_Enable())
				continue;
			UI->LateTick(fTimeDelta);
		}
	}
}

HRESULT CUI_Canvas::Render()
{
	for (_uint i = 0; i <= UI_TYPE::UI_END;i++)
	{
		VECTOR_UI vecUI = m_vecUI[i];
		for (auto UI : vecUI)
		{
			if (!UI->Get_Enable())
				continue;
			UI->Render();
		}
	}
	return S_OK;
}

void CUI_Canvas::Check_UI(CUI * pUI)
{

}
