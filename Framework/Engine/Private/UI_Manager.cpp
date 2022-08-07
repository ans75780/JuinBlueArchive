#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "UI.h"
#include "UI_Canvas.h"

IMPLEMENT_SINGLETON(CUI_Manager);

CUI_Manager::CUI_Manager()
{
}


HRESULT CUI_Manager::Setup_Manager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const GRAPHICDESC & GraphicDesc, _uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_vecCanvas.reserve(iNumLevels);

	XMStoreFloat4x4(&CUI::g_UIMatProj, XMMatrixTranspose(XMMatrixOrthographicLH(GraphicDesc.iWinCX, GraphicDesc.iWinCY, 0.f, 1.f)));

	//Reserved_Container에서 레벨 개수만큼 할당한 공간 개수 만큼 캔버스 생성
	for (_uint i = 0; i < m_vecCanvas.capacity(); i++)
	{
		CUI_Canvas* pCanvas = CUI_Canvas::Create(pDevice, pContext);
		m_vecCanvas.push_back(pCanvas);
	}
	return S_OK;


	return S_OK;
}


HRESULT CUI_Manager::Add_UI(_uint iLevelIndex, CUI * pUI,void * pArg)
{
	m_vecCanvas[iLevelIndex]->Add_UI(pUI);
	return S_OK;
}

void CUI_Manager::Tick(_float fTimeDelta)
{
	CGameInstance* instance = GET_INSTANCE(CGameInstance);

	_uint	iLevel = instance->Get_CurrentLevel();

	m_vecCanvas[iLevel]->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::LateTick(_float fTimeDelta)
{
	CGameInstance* instance = GET_INSTANCE(CGameInstance);


	_uint	iLevel = instance->Get_CurrentLevel();


	m_vecCanvas[iLevel]->LateTick(fTimeDelta);


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::DisableCanvas(_uint iLevelIndex)
{
	
}


void CUI_Manager::Free()
{
	for (auto pCanvas : m_vecCanvas)
	{
		Safe_Release(pCanvas);
	}
	m_vecCanvas.clear();
}

