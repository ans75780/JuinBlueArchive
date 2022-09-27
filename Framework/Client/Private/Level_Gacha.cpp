#include "stdafx.h"
#include "..\Public\Level_Gacha.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Gacha::CLevel_Gacha(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Gacha::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Gacha::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Gacha::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Gacha ����"));

	return S_OK;
}

CLevel_Gacha * CLevel_Gacha::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Gacha*		pInstance = new CLevel_Gacha(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Gacha");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Gacha::Free()
{
	__super::Free();



}

