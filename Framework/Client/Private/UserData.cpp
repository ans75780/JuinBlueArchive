#include "stdafx.h"
#include "..\Public\UserData.h"


HRESULT CUserData::Initialize()
{
	m_vecFormation.push_back(L"Serika");

	return S_OK;
}

void CUserData::Free()
{
}
