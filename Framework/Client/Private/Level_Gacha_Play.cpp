#include "stdafx.h"
#include "..\Public\Level_Gacha_Play.h"
#include "GameInstance.h"

#include "Camera.h"
#include "PipeLine.h"

#include "Arona.h"
#include "Animation.h"

CLevel_Gacha_Play::CLevel_Gacha_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Gacha_Play::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_GachaCam(TEXT("Layer_Cam"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Student(TEXT("Layer_Student"))))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;
	
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CCamera::Set_MainCam(static_cast<CCamera*>(pGameInstance->Get_GameObjects(LEVEL_GACHA_PLAY, TEXT("Layer_Camera")).front()));
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_Gacha_Play::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	

	RELEASE_INSTANCE(CGameInstance);
	return;
}

HRESULT CLevel_Gacha_Play::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("CLevel_Gacha_Play 레벨"));

	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_Layer_Camera(const _tchar * pLayerTag) // Layer_Camera
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.0f, 0.f, -3.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, -1.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(45.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GACHA_PLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_GachaPlay"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_Layer_Student(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_GameObject(LEVEL_GACHA_PLAY, pLayerTag, TEXT("Prototype_GameObject_Arona"));


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_GachaCam(const _tchar* pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_GameObject(LEVEL_GACHA_PLAY, pLayerTag, TEXT("Prototype_GameObject_Arona_Cam"));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Gacha_Play::Ready_Light()
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
		MSG_BOX("Failed to Created : CLevel_Gacha_Play 레벨쪽에러");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Gacha_Play::Free()
{
	__super::Free();



}

