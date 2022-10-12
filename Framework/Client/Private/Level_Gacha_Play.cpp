#include "stdafx.h"
#include "..\Public\Level_Gacha_Play.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Gacha_Play::CLevel_Gacha_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Gacha_Play::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Bg()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Gacha_Play::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Gacha_Play::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Gacha_Play ·¹º§"));

	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_Layer_Bg()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA, TEXT("Layer_Gacha_BG"), TEXT("Prototype_GameObject_Gacha_BG"))))
	//	return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Gacha_Play * CLevel_Gacha_Play::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Gacha_Play*		pInstance = new CLevel_Gacha_Play(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Gacha_Play");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Gacha_Play::Free()
{
	__super::Free();



}

