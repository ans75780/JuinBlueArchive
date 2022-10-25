#include "stdafx.h"
#include "..\Public\Level_MapTool.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Json_Utility.h"

#include "Camera_Free.h"
#include "Light.h"

CLevel_MapTool::CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_MapTool::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

//	if (FAILED(Ready_Layer_MapProp(TEXT("Layer_Map"))))
//		return E_FAIL;

	if (FAILED(Ready_Layer_StageMap(TEXT("Layer_Stage"))))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(UI_Extract()))
		return E_FAIL;

	return S_OK;
}

void CLevel_MapTool::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CLevel_MapTool::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("¸ÊÅø·¹º§ÀÓ. "));

	return S_OK;
}


HRESULT CLevel_MapTool::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.0f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 1.f;
	CameraDesc.fFar = 3000.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc,
		((CGameObject**)&m_pFreeCam)
	)))
	Safe_Release(pGameInstance);

	m_pFreeCam->Set_MainCam(m_pFreeCam);
	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Light()
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

	/*ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 5.f, 10.f, 0.f);
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_MapProp(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Sky */
	/*if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	return E_FAIL;*/

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, pLayerTag, TEXT("Prototype_GameObject_MapProp"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_StageMap(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAPTOOL, pLayerTag, TEXT("Prototype_GameObject_Stage_School"), L"Prototype_Component_Model_Stage_School_1")))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_MapTool::UI_Extract()
{
	//Mat img = imread("../../Resources/UI/UI_original/image/Floater.png", IMREAD_UNCHANGED);

	//if (img.empty())
	//{
	//	return E_FAIL;
	//}

	////json	JsonTemp;
	////
	////if (FAILED(CJson_Utility::Load_Json(CJson_Utility::Complete_Path(
	////	L"../../Resources/UI/UI_original/json/Floater"))))


	//Mat cropped_image = img(Range(152, 256), Range(0, 230));

	//imwrite("../../Resources/UI/UI_extract/image/temp.png", cropped_image);




	return S_OK;
}

CLevel_MapTool * CLevel_MapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_MapTool*		pInstance = new CLevel_MapTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_MapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_MapTool::Free()
{
	__super::Free();

}

