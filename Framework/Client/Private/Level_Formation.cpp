#include "stdafx.h"
#include "..\Public\Level_Formation.h"
#include "GameInstance.h"
#include "GameObject.h"

#include "Camera_Free.h"
#include "Light.h"
#include "State_Student_Formation_Idle.h"
#include "Student.h"
#include "Animation.h"
#include "StateMachineBase.h"
#include "Level_GamePlay.h"
#include "Level_Loading.h"
#include "Transform.h"
CLevel_Formation::CLevel_Formation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Formation::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Student(TEXT("Layer_Student"))))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;



	return S_OK;
}

void CLevel_Formation::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;

		Safe_Release(pGameInstance);
	}

}

HRESULT CLevel_Formation::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("Level : Formation"));

	return S_OK;
}


HRESULT CLevel_Formation::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Camera_Free */
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float4(0.0f, 0.f, -3.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_GameObject_Camera_Formation"), &CameraDesc)))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_Formation::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* For.Sky */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Formation::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Formation::Ready_Layer_Student(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::OBJ_DESC tempDesc;


	CGameObject* pStudent = nullptr;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_Student_Serika"), (void*)&tempDesc, &pStudent)))
		return E_FAIL;
	((CStudent*)pStudent)->Get_StateMachine()->Setup_StateMachine(CState_Student_Formation_Idle::Create((CStudent*)pStudent));
	XMVECTOR vTranslation;
	vTranslation = XMVectorSet(-1.f, 0.f, 0.f, 1.f);

	((CTransform*)pStudent->Get_Component(L"Com_Transform"))->Set_State(CTransform::STATE_TRANSLATION, vTranslation);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, TEXT("Prototype_Student_Haruka"), (void*)&tempDesc, &pStudent)))
		return E_FAIL;
	((CStudent*)pStudent)->Get_StateMachine()->Setup_StateMachine(CState_Student_Formation_Idle::Create((CStudent*)pStudent));
	
	vTranslation = XMVectorSet(1.f, 0.f, 0.f, 1.f);

	((CTransform*)pStudent->Get_Component(L"Com_Transform"))->Set_State(CTransform::STATE_TRANSLATION, vTranslation);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Formation::Ready_Layer_UI(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CLevel_Formation::Ready_Light()
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

CLevel_Formation * CLevel_Formation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Formation*		pInstance = new CLevel_Formation(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Formation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Formation::Free()
{
	__super::Free();

}

