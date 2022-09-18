#include "stdafx.h"
#include "..\Public\UserData.h"


IMPLEMENT_SINGLETON(CUserData);

HRESULT CUserData::Initialize()
{
	m_vecFormation.push_back(L"Serika");
	m_vecFormation.push_back(L"Zunko");
	m_vecFormation.push_back(L"Haruka");

	return S_OK;
}

void CUserData::Free()
{
}
