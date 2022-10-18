#include "stdafx.h"
#include "..\Public\Level_Gacha_Play.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

#include "Camera.h"

CLevel_Gacha_Play::CLevel_Gacha_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Gacha_Play::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Student(TEXT("Layer_Student"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
		return E_FAIL;

	if (FAILED(Ready_Right()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
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

HRESULT CLevel_Gacha_Play::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.0f, 2.f, -3.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;
	//Layer_Camera
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA_PLAY, pLayerTag, TEXT("Prototype_GameObject_Arona_Camera"), &CameraDesc)))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_Layer_Student(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//Layer_Student
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA_PLAY, pLayerTag, TEXT("Prototype_GameObject_Arona"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA_PLAY, TEXT("Layer_Gacha_Cam"), TEXT("Prototype_GameObject_Arona_Gacha_Cam"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_Layer_Sky(const _tchar * pLayerTag)//Layer_Sky
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA_PLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"),
		L"Prototype_Component_Texture_Formaiton_Background")))


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_Right()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

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

