#include "stdafx.h"
#include "..\Public\Level_Shop.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Shop::CLevel_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Shop::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Shop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Shop::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Shop ·¹º§"));

	return S_OK;
}

CLevel_Shop * CLevel_Shop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Shop*		pInstance = new CLevel_Shop(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Shop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Shop::Free()
{
	__super::Free();



}

