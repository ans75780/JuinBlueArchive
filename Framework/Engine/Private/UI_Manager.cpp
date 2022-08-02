#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "UI.h"

CUI_Manager::CUI_Manager()
{
}


CUI_Manager::~CUI_Manager()
{

}

HRESULT CUI_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pUIs)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pUIs = new UIS[iNumLevels];

	return S_OK;
}

HRESULT CUI_Manager::Add_UI(_uint iLevelIndex, const _tchar * pUITag, CUI * pUI, void * pArg)
{
	UIS	Layer = m_pUIs[iLevelIndex];


	auto	iter = find_if(Layer.begin(), Layer.end(), CTag_Finder(pUITag));


	if (iter != Layer.end())
	{
		MSG_BOX("같은 이름의 UI오브젝트가 이미 존재합니다 : Add_UI");
		return E_FAIL;
	}
	Layer.emplace(pUITag, pUI);

	return S_OK;
}

void CUI_Manager::Tick(_float fTimeDelta)
{
	CGameInstance* instance = GET_INSTANCE(CGameInstance);

	_uint	iLevel = instance->Get_CurrentLevel();


	for (auto elem : m_pUIs[iLevel])
	{
		if (!elem.second->Get_Enable())
			continue;
		elem.second->Tick(fTimeDelta);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::LateTick(_float fTimeDelta)
{
	CGameInstance* instance = GET_INSTANCE(CGameInstance);


	_uint	iLevel = instance->Get_CurrentLevel();


	for (auto elem : m_pUIs[iLevel])
	{
		if (!elem.second->Get_Enable())
			continue;
		elem.second->LateTick(fTimeDelta);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::DisableUIs(_uint iLevelIndex)
{
	for (auto elem : m_pUIs[iLevelIndex])
	{
		elem.second->Set_Enable(false);
	}
}

void CUI_Manager::Clear(_uint iLevelIndex)
{

}


CGameObject * CUI_Manager::Find_UI(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	return nullptr;
}


void CUI_Manager::Free()
{

}

