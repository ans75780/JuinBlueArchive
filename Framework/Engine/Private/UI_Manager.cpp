#include "..\Public\UI_Manager.h"



CUI_Manager::CUI_Manager()
{
}


CUI_Manager::~CUI_Manager()
{
}

HRESULT CUI_Manager::Reserve_Container(_uint iNumLevels)
{
	return E_NOTIMPL;
}


HRESULT CUI_Manager::Add_UI(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	return E_NOTIMPL;
}

void CUI_Manager::Tick(_float fTimeDelta)
{
}

void CUI_Manager::LateTick(_float fTimeDelta)
{
}

void CUI_Manager::Clear(_uint iLevelIndex)
{
}



CUI * CUI_Manager::Find_UI(const _tchar * pUITag)
{
	return nullptr;
}

void CUI_Manager::Free()
{
}
