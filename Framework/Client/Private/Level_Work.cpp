#include "stdafx.h"
#include "..\Public\Level_Work.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

CLevel_Work::CLevel_Work(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Work::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Work::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

}

HRESULT CLevel_Work::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Work ·¹º§"));

	return S_OK;
}

CLevel_Work * CLevel_Work::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Work*		pInstance = new CLevel_Work(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Work");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Work::Free()
{
	__super::Free();



}

