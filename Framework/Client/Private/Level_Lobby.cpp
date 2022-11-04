#include "stdafx.h"
#include "..\Public\Level_Lobby.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"
#include "Memorial_Haruka_Start.h"

CLevel_Lobby::CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	pGameInstance->Set_DisableUI(true);
	if (pGameInstance->Get_LobbyMemorialOnce())
	{
		if (FAILED(Ready_Layer_HarukaMemorial()))
			return E_FAIL;
		pGameInstance->Set_LobbyMemorialOnce(false);
	}
	else
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Haruka_Memorial"), TEXT("Prototype_GameObject_Memorial_Haruka_Idle"))))
			return E_FAIL;
	}
	pGameInstance->Get_Instance()->Get_SoundManager()->StopAll();
	pGameInstance->Get_Instance()->Get_SoundManager()->PlayBGM(L"Aru_Lobby.ogg", 0.1f);

	RELEASE_INSTANCE(CGameInstance);

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

HRESULT CLevel_Lobby::Ready_Layer_HarukaMemorial()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Haruka_Memorial"), TEXT("Prototype_GameObject_Memorial_Haruka_Start"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
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

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Get_Instance()->Get_SoundManager()->StopAll();
	RELEASE_INSTANCE(CGameInstance);
}

