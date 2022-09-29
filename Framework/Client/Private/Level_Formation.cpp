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
#include "PipeLine.h"
#include "Collider.h"
#include "UserData.h"
#include "State_Student_Formation_Pickup.h"
CLevel_Formation::CLevel_Formation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Formation::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_iFormationSize = 3;
	m_vecFormationPos.push_back(XMVectorSet(-2.f, -1.f, 0.f, 1.f));
	m_vecFormationPos.push_back(XMVectorSet(0.f, -1.f, 0.f, 1.f));
	m_vecFormationPos.push_back(XMVectorSet(2.f, -1.f, 0.f, 1.f));

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

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

	_float3 vPos = { 0.f,0.f,0.f };
	_float3 vSize = { 10.f,10.f,0.1f };
	
	m_iPickedIndex = 256;

	



	m_pRayBoard = new BoundingBox(vPos, vSize);

	return S_OK;
}

void CLevel_Formation::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY(SPACE, TAP))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		CUserData*			pUserData = CUserData::Get_Instance();


		//만약, 새롭게 들어가는 포메이션이 존재한다면
		pUserData->Clear_Formation();
		for (_uint i = 0; i < m_vecStudent.size(); i++)
			pUserData->Add_Formation(m_vecStudent[i]->Get_Desc());
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}
	CGameInstance* pInst = GET_INSTANCE(CGameInstance);
	RAYDESC ray;
	_float distance = 0.f;
	ray = pInst->Get_Ray();
	if (KEY(LBUTTON, TAP) && m_bPicked == false)
	{
		for (_uint i = 0; i < m_vecStudent.size();i++)
		{
			if (m_vecStudent[i]->Collision_AABB(ray, distance))
			{
				m_iPickedIndex = i;
				m_bPicked = true;
				m_vecStudent[m_iPickedIndex]->Get_StateMachine()->Get_CurrentState()->CallExit();
				break;
			}
		}
		
	}
	if (KEY(LBUTTON, HOLD))
	{
		if (m_bPicked)
		{
			if (m_pRayBoard->Intersects(ray.vRayOrigin, ray.vRayDir, distance))
			{
				_vector pickedPos = ray.vRayOrigin + ray.vRayDir * distance;

				
				BoundingBox* box = m_vecStudent[m_iPickedIndex]->Get_AABB()->Get_AABB();
				pickedPos -= XMLoadFloat3(&box->Extents);
				pickedPos = XMVectorSetZ(pickedPos, 0.f);

				m_vecStudent[m_iPickedIndex]->Set_Transform(pickedPos);

			}
			for (_uint i = 0; i < m_vecStudent.size(); i++)
			{
				if (i == m_iPickedIndex)
					continue;
				m_vecStudent[i]->Set_Transform(m_vecFormationPos[i]);
				if (m_vecStudent[m_iPickedIndex]->Get_AABB()->Collision_AABB((m_vecStudent[i]->Get_AABB())))
				{
					
					_vector vPos = m_vecFormationPos[i];
					
					vPos = XMVectorSetZ(vPos, 0.3f);
	
					m_vecStudent[i]->Set_Transform(vPos);
				
					break;
				}
			}
		}
	}

	if (KEY(LBUTTON, AWAY))
	{
		_bool bChange = false;
		if (m_iPickedIndex > 3)
			return;
		for (_uint i = 0; i < m_vecStudent.size(); i++)
		{
			if (i == m_iPickedIndex)
				continue;

			if (m_vecStudent[m_iPickedIndex]->Get_AABB()->Collision_AABB((m_vecStudent[i]->Get_AABB())))
			{
				//집은 캐릭터가 다른 캐릭터 위에 겹쳐져 있다면. 둘의 위치를 교환
				m_vecStudent[m_iPickedIndex]->Set_Transform(m_vecFormationPos[i]);
				m_vecStudent[i]->Set_Transform(m_vecFormationPos[m_iPickedIndex]);


				//둘의 벡터 위치를 교환
				CStudent*	pStudent = m_vecStudent[m_iPickedIndex];
				m_vecStudent[m_iPickedIndex] = m_vecStudent[i];
				m_vecStudent[i] = pStudent;

				m_vecStudent[i]->Get_StateMachine()->Get_CurrentState()->CallExit();
				bChange = true;
				break;
			}
		}
		if (!bChange && m_bPicked)
		{
			m_vecStudent[m_iPickedIndex]->Set_Transform(m_vecFormationPos[m_iPickedIndex]);
			m_vecStudent[m_iPickedIndex]->Get_StateMachine()->Get_CurrentState()->CallExit();
		}
		m_bPicked = false;
	}
	RELEASE_INSTANCE(CGameInstance);
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
	CameraDesc.vAt = _float4(0.f, 0.f, -1.f, 1.f);
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
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Sky"), 
		L"Prototype_Component_Texture_Formaiton_Background")))
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
	CUserData*			pUserData = CUserData::Get_Instance();


	CGameObject* pStudent = nullptr;
	_tchar	szStudentPath[MAX_PATH] = L"Prototype_Student";


	//만약 전에 편성된 포메이션이 존재한다면 포메이션을 가져옴
	vector<CGameObject::OBJ_DESC> vecStudentDesc = pUserData->Get_Formation();
	if (vecStudentDesc.size() == m_iFormationSize)
	{
		for (_uint i = 0; i < m_iFormationSize; i++)
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, szStudentPath, (void*)&(vecStudentDesc[i]), &pStudent)))
				return E_FAIL;
			((CStudent*)pStudent)->Set_Transform(m_vecFormationPos[i]);
			m_vecStudent.push_back((CStudent*)pStudent);
		}
	}
	else
	{
		//아니라면 새로 가져온 포메이션으로 
		map<const _tchar*, CGameObject::OBJ_DESC> StudentDesc = pUserData->Get_HavedStudent();
		_uint iIndex = 0;
		for (auto& pair : StudentDesc)
		{
			if (m_iFormationSize <= iIndex)
				break;
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_FORMATION, pLayerTag, szStudentPath, (void*)&(pair.second), &pStudent)))
				return E_FAIL;
			((CStudent*)pStudent)->Set_Transform(m_vecFormationPos[iIndex]);
			iIndex++;
			m_vecStudent.push_back((CStudent*)pStudent);
		}
	}

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
	Safe_Delete(m_pRayBoard);
}

