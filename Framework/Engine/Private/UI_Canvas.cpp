#include "..\Public\UI_Canvas.h"
#include "GameInstance.h"
#include "UI.h"

CUI_Canvas::CUI_Canvas(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice), m_pContext(pContext)
{
}

CUI_Canvas::~CUI_Canvas()
{
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

void CUI_Canvas::Add_UI(CUI * pUI)
{
	m_vecUI.push_back(pUI);
}

HRESULT CUI_Canvas::Initialize()
{
	m_vecUI.reserve(20);

	return S_OK;
}

void CUI_Canvas::Tick(_float fTimeDelta)
{
	for (auto UI : m_vecUI)
	{
		if (!UI->Get_Enable())
			continue;
		UI->Tick(fTimeDelta);
	}
}

void CUI_Canvas::LateTick(_float fTimeDelta)
{
	for (auto UI : m_vecUI)
	{
		if (!UI->Get_Enable())
			continue;
		UI->LateTick(fTimeDelta);
	}
}

HRESULT CUI_Canvas::Render()
{
	for (auto UI : m_vecUI)
	{
		if (!UI->Get_Enable())
			continue;
		UI->Render();
	}
}
