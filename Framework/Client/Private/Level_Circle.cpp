#include "stdafx.h"
#include "..\Public\Level_Circle.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Circle::CLevel_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Circle::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Circle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Circle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Circle ·¹º§"));

	return S_OK;
}

CLevel_Circle * CLevel_Circle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Circle*		pInstance = new CLevel_Circle(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Circle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Circle::Free()
{
	__super::Free();



}

