#include "stdafx.h"
#include "..\Public\Level_Schedule.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Schedule::CLevel_Schedule(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Schedule::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Schedule::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Schedule::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Schedule ·¹º§"));

	return S_OK;
}

CLevel_Schedule * CLevel_Schedule::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Schedule*		pInstance = new CLevel_Schedule(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Schedule");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Schedule::Free()
{
	__super::Free();



}

