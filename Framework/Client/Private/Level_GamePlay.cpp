#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Camera_Stage.h"
#include "Camera_Free.h"
#include "Light.h"
#include "UserData.h"
#include "Student.h"
#include "State_Idle.h"
#include "StateMachineBase.h"
#include "Level_Loading.h"
#include "State_Student_Default.h"
#include "Camera_Event.h"
#include "Camera.h"
#include "StateBase.h"
#include "State_Student_Ex.h"
#include "Animation.h"
#include "Camera.h"
#include "Enemy.h"
#include "CombatFormation.h"
#include "Effect_Hit.h"
#include "Sound_Device.h"
#include "UI_Warning.h"
#include "Boss_Tank.h"
CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Student(TEXT("Layer_Student"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Enemy(TEXT("Layer_Enemy"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Baricade(TEXT("Layer_Baricade"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Event(TEXT("Layer_Event"))))
		return E_FAIL;
	if (FAILED(Ready_Light()))
		return E_FAIL;


	m_pCombatFormaiton->StartGame();
	
	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	if (m_pBoss->Get_StageState() == CActor::STAGE_STATE_DEAD)
	{
		m_bBossDead = true;
		pGameInstance->Set_TimeMagnifiaction(0.7f);
	}

	if (m_bBossDead && m_bVictoryEventCalled == false)
	{
		m_fClearTick += fTimeDelta;
		if (m_fClearTime <= m_fClearTick)
		{
			pGameInstance->Reset_TimeMagnifiaction();
			_float3	vOffset = { 1.5f, 1.2f, 2.5f };
			m_pEventCam->Ready_Event_Stage_Victory(m_pStage, vOffset, m_pCombatFormaiton->Get_Students());
			m_bVictoryEventCalled = true;
		}

	}


	if (KEY(T, TAP))
	{
		if (m_pFreeCam->IsMainCam())
		{
			m_pStageCam->Set_MainCam(m_pStageCam);
		}
		else
		{
			m_pFreeCam->Set_MainCam(m_pFreeCam);
		}
	}

	if (KEY(SPACE, TAP))
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_FORMATION))))
			return;
	}
	if (m_pCombatFormaiton)
		m_pCombatFormaiton->Tick(fTimeDelta);

}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Stage"), &CameraDesc, 
		((CGameObject**)&m_pStageCam)
		)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Event"), &CameraDesc,
		((CGameObject**)&m_pEventCam)
	)))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc,
		((CGameObject**)&m_pFreeCam)
	)))

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Stage_School"), L"Prototype_Component_Model_Stage_School_1", ((CGameObject**)&m_pStage))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Enemy(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();


	_tchar	szPath[MAX_PATH] = L"Prototype_Enemy";
	CGameObject*			pObj;

	const map<const _tchar*, CGameObject::OBJ_DESC>* EnemyData  = CUserData::Get_Instance()->Get_Actors(UNIT_TYPE::UNIT_TYPE_ENEMY);
	for (auto& pair : *EnemyData)
	{
		for (int i = 1; i < 5; i++)
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, szPath, (void*)&pair.second, &pObj)))
				return E_FAIL;
			m_pEnemy = (CEnemy*)pObj;
			m_pEnemy->Set_Transform(XMVectorSet(-1.5f, 0, 10.f * i, 1.f));
			m_pEnemy->Get_Transform()->Rotation(XMVectorSet(0, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, szPath, (void*)&pair.second, &pObj)))
				return E_FAIL;
			m_pEnemy = (CEnemy*)pObj;
			m_pEnemy->Set_Transform(XMVectorSet(0, 0, 10.f * i, 1.f));
			m_pEnemy->Get_Transform()->Rotation(XMVectorSet(0, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, szPath, (void*)&pair.second, &pObj)))
				return E_FAIL;
			m_pEnemy = (CEnemy*)pObj;
			m_pEnemy->Set_Transform(XMVectorSet(1.5f, 0, 10.f * i, 1.f));
			m_pEnemy->Get_Transform()->Rotation(XMVectorSet(0, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));

		}
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, L"Prototype_GameObject_Boss_Tank",nullptr, &pObj)))
		return E_FAIL;
	m_pEnemy = (CEnemy*)pObj;
	m_pEnemy->Set_Transform(XMVectorSet(0.f,0.f,57.f,1.f));
	m_pEnemy->Get_Transform()->Rotation(XMVectorSet(0, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));
	m_pBoss = (CBoss_Tank*)m_pEnemy;
	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 20; ++i)
	{
		/* For.Effect */
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
			return E_FAIL;
	}



	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Student(const _tchar * pLayerTag)
{

	CCombatFormation::FORMATIONDESC desc;


	desc.eLevel = LEVEL::LEVEL_GAMEPLAY;
	lstrcpy(desc.szLayer, pLayerTag);
	desc.eGameStartEvent = true;

	m_pCombatFormaiton = CCombatFormation::Create(&desc, m_pStageCam, m_pEventCam, m_pDevice, m_pContext);
	
	if (nullptr == m_pCombatFormaiton)
		return  E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Baricade(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pObj;
	
	for (_uint i = 1; i <= 5; i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_Baricade"), L"Prototype_Component_Model_School_Baricade", &pObj)))
			return E_FAIL;
		pObj->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-1.f, 0.f, i * 8.f, 1.f));

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_Baricade"), L"Prototype_Component_Model_School_Baricade", &pObj)))
			return E_FAIL;
		pObj->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, i * 8.f, 1.f));

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_Baricade"), L"Prototype_Component_Model_School_Baricade", &pObj)))
			return E_FAIL;
		pObj->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.f, 0.f, i * 8.f, 1.f));
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Event(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pObj;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Warning"),nullptr, &pObj)))
		return E_FAIL;
	
	pObj->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 45.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Light()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.0f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
	Safe_Release(m_pCombatFormaiton);
}

