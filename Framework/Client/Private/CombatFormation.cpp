#include "stdafx.h"
#include "CombatFormation.h"
#include "..\Public\CombatFormation.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Actor.h"	
#include "Student.h"
#include "UserData.h"
#include "StateMachineBase.h"
#include "StateBase.h"
#include "State_Student_Default.h"
#include "Camera.h"
#include "Camera_Event.h"
#include "Camera_Stage.h"
#include "State_Student_Ex.h"
#include "Base.h"
#include "UI_CombatFormation_Skill.h"


CCombatFormation::CCombatFormation(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext)
	:m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CCombatFormation::~CCombatFormation()
{
}



HRESULT CCombatFormation::Initialize(void * pArg)
{

	//리더 왼쪽 오른쪽 순으로
	m_vecFormationPos.push_back(XMVectorSet(0.f, 0.f, 0.5f, 1.f));
	m_vecFormationPos.push_back(XMVectorSet(0.5f, 0.f, 0.f, 1.f));
	m_vecFormationPos.push_back(XMVectorSet(-0.5f, 0.f, 0.f, 1.f));

	ZeroMemory(&m_eDesc, sizeof(FORMATIONDESC));

	memcpy(&m_eDesc, pArg, sizeof(FORMATIONDESC));


	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pStudent = nullptr;

	_tchar	szStudentPath[MAX_PATH] = L"Prototype_Student";

	//각 클론된 스튜던트에 정보를 넣어줌.
	CGameObject::OBJ_DESC	desc;

	vector<CGameObject::OBJ_DESC> m_formationDesc = CUserData::Get_Instance()->Get_Formation();
	for (_uint i = 0; i < m_formationDesc.size(); i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(m_eDesc.eLevel,m_eDesc.szLayer, szStudentPath, (void*)&m_formationDesc[i], &pStudent)))
			return E_FAIL;
		((CStudent*)pStudent)->Set_Transform(m_vecFormationPos[i]);
		m_vecStudent.push_back((CStudent*)pStudent);
	}
	Safe_Release(pGameInstance);
	
	
	m_pStageCam->Set_Formation(&m_vecStudent);


	if (m_eDesc.eLevel == LEVEL_GAMEPLAY)
	{
		SetUp_ExModels();
		m_pCombatSkill = CUI_CombatFormation_Skill::Create(m_pDevice, m_pContext);
		CGameInstance*	pInstance = CGameInstance::Get_Instance();

		pInstance->Get_SoundManager()->Play_Sound(L"Battle_In.ogg", 0.9f);


	}
	return S_OK;
}

void CCombatFormation::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	LEVEL eLevel = (LEVEL)pGameInstance->Get_CurrentLevelID();

	if (eLevel != LEVEL_GAMEPLAY)
		return;

	_uint i = 0;
	for (auto& elem = m_vecStudent.begin(); elem != m_vecStudent.end();)
	{
		if ((*elem)->Get_StageState() == CActor::STAGE_STATE_DEAD)
		{
			elem = m_vecStudent.erase(elem);
			m_vecExCutinStudent.erase(m_vecExCutinStudent.begin() + i);
		}
		else
		{
			elem++;
		}
		i++;
	}

	if (KEY(Z, TAP) && m_pStageCam->IsMainCam())
	{
		m_pEventCam->Ready_Event_Ex(m_pStageCam, (CActor*)(m_vecExCutinStudent[0]), m_vecStudent[0]);
	}
	if (KEY(X, TAP))
	{
		m_pEventCam->Ready_Event_Ex(m_pStageCam, (CActor*)(m_vecExCutinStudent[1]), m_vecStudent[1]);
	}
	if (KEY(C, TAP))
	{
		m_pEventCam->Ready_Event_Ex(m_pStageCam, (CActor*)(m_vecExCutinStudent[2]), m_vecStudent[2]);
	}
	m_pCombatSkill->Tick(fTimeDelta);
}

void CCombatFormation::LateTick(_float fTimeDelta)
{
	m_pCombatSkill->LateTick(fTimeDelta);

}

HRESULT CCombatFormation::Render()
{

	m_pCombatSkill->Render();

	return S_OK;
}

vector<class CStudent*>* CCombatFormation::Get_Students()
{
	return &m_vecStudent;
}

HRESULT CCombatFormation::StartGame()
{
	if (m_eDesc.eGameStartEvent)
	{
		m_vecStudent[0]->Get_StateMachine()->Add_State
		(
			CState_Student_Default::Create(m_vecStudent[0], L"_Normal_Callsign")
		);
		_float3	vOffset = { 1.5f,1.f, 1.5f };

		m_pEventCam->Ready_Event_Stage_Start(m_pStageCam, m_vecStudent[0],
			m_vecStudent[0]->Get_StateMachine()->Get_CurrentState()->Get_Animation(),
			vOffset, &m_vecStudent);
	}
	else
	{
		vector<_float4> StartPos = m_pEventCam->Get_VecStudentStartPos();
		for (_uint i = 0; i < StartPos.size();i++)
			m_vecStudent[i]->Set_Transform(XMLoadFloat4(&StartPos[i]));
	}
	m_pCombatSkill->StartGame();
	return S_OK;
}

HRESULT CCombatFormation::SetUp_ExModels()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pStudent = nullptr;

	_tchar	szActorPath[MAX_PATH] = L"Prototype_Student_FX";

	//각 클론된 스튜던트에 정보를 넣어줌
	vector<CGameObject::OBJ_DESC> m_ExModelDesc = CUserData::Get_Instance()->Get_Formation();
	for (_uint i = 0; i < m_ExModelDesc.size(); i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(m_eDesc.eLevel, L"Layer_Student_FX", szActorPath, (void*)&m_ExModelDesc[i], &pStudent)))
		{
			
			int a = 10;
		}
		else
		{
			pStudent->Set_Enable(false);
			m_vecExCutinStudent.push_back((CStudent_FX*)pStudent);
		}
	}
	Safe_Release(pGameInstance);


	return S_OK;
}


CCombatFormation * CCombatFormation::Create(FORMATIONDESC * pDesc, CCamera * pStageCam, CCamera * pEventCam, ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext)
{
	CCombatFormation*	pInstance = new CCombatFormation(_pDevice, _pContext);

	pInstance->m_pStageCam = (CCamera_Stage*)pStageCam;
	pInstance->m_pEventCam = (CCamera_Event*)pEventCam;

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CCombatFormation::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_vecStudent.clear();
	m_vecExCutinStudent.clear();
}


