#include "stdafx.h"
#include "..\Public\Level_Students.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Students::CLevel_Students(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Students::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Students::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Students::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Students ·¹º§"));

	return S_OK;
}

CLevel_Students * CLevel_Students::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Students*		pInstance = new CLevel_Students(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Students");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Students::Free()
{
	__super::Free();



}

