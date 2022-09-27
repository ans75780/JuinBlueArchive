#include "stdafx.h"
#include "..\Public\Level_Manufacture.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Manufacture::CLevel_Manufacture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Manufacture::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Manufacture::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Manufacture::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Manufacture ·¹º§"));

	return S_OK;
}

CLevel_Manufacture * CLevel_Manufacture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Manufacture*		pInstance = new CLevel_Manufacture(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Manufacture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Manufacture::Free()
{
	__super::Free();



}

