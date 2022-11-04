#include "stdafx.h"
#include "..\Public\Level_Shop.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

#include "Camera.h"

CLevel_Shop::CLevel_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Shop::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Right()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Stage(TEXT("Layer_Stage"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Hod(TEXT("Layer_Hod"))))
		return E_FAIL;

	//if (FAILED(TestEffect()))
	//	return E_FAIL;

	CGameInstance*		pGameInstanceSound = GET_INSTANCE(CGameInstance);
	pGameInstanceSound->Get_Instance()->Get_SoundManager()->StopAll();
	pGameInstanceSound->Get_Instance()->Get_SoundManager()->PlayBGM(L"Hod_Bgm.ogg", 0.1f);
	RELEASE_INSTANCE(CGameInstance);


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

	//SetWindowText(g_hWnd, TEXT("CLevel_Shop ·¹º§"));

	return S_OK;
}

HRESULT CLevel_Shop::Ready_Layer_Stage(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP,
		pLayerTag, TEXT("Prototype_GameObject_Stage_Hod"), TEXT("Prototype_Component_Model_Stage_Hod"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Shop::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(1.8f, 7.5f, -6.5f, 1.f);
	CameraDesc.vAt = _float4(3.f, 0.f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;
	//Layer_Camera
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Shop::Ready_Layer_Hod(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, pLayerTag, TEXT("Prototype_GameObject_Hod"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Shop::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, pLayerTag, TEXT("Prototype_GameObject_Gacha_Sky"),L"Prototype_Component_Texture_hod_Sky")))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Shop::Ready_Right()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Shop::TestEffect()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, TEXT("Layer_Effect_Bullet"), TEXT("Prototype_GameObject_Effect_Bullet"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
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

