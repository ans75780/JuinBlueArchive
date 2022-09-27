#include "stdafx.h"
#include "..\Public\Level_Cafe.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Cafe::CLevel_Cafe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Cafe::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Cafe::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Cafe::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Cafe ·¹º§"));

	return S_OK;
}

CLevel_Cafe * CLevel_Cafe::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Cafe*		pInstance = new CLevel_Cafe(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Cafe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Cafe::Free()
{
	__super::Free();



}

