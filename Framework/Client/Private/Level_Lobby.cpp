#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Lobby::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로비레벨. "));

	return S_OK;
}

CLevel_Lobby * CLevel_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Lobby*		pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
	__super::Free();

}

