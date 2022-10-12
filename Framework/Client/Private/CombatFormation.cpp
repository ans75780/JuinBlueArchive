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


CCombatFormation::CCombatFormation()
{
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

	return S_OK;
}

void CCombatFormation::Tick(_float fTimeDelta)
{
	if (KEY(R, TAP))
	{
		m_vecStudent[1]->Get_StateMachine()->Add_State
		(
			CState_Student_Default::Create(m_vecStudent[1], L"_Normal_Callsign")
		);
	}
	if (KEY(NUM1, TAP))
	{
		m_vecStudent[0]->Get_StateMachine()->Add_State
		(
			CState_Student_Ex::Create(m_vecStudent[0])
		);
		m_pEventCam->Ready_Event_Ex(m_pStageCam, m_vecStudent[0]);

	}
	if (KEY(NUM2, TAP))
	{
		m_vecStudent[1]->Get_StateMachine()->Add_State
		(
			CState_Student_Ex::Create(m_vecStudent[1])
		);
		m_pEventCam->Ready_Event_Ex(m_pStageCam, m_vecStudent[1]);

	}
	if (KEY(NUM3, TAP))
	{
		m_vecStudent[2]->Get_StateMachine()->Add_State
		(
			CState_Student_Ex::Create(m_vecStudent[2])
		);
		m_pEventCam->Ready_Event_Ex(m_pStageCam, m_vecStudent[2]);
	}
}

void CCombatFormation::LateTick(_float fTimeDelta)
{
}

HRESULT CCombatFormation::Render()
{
	return S_OK;
}

HRESULT CCombatFormation::StartGame()
{
	if (m_eDesc.eGameStartEvent)
	{
		m_vecStudent[0]->Get_StateMachine()->Add_State
		(
			CState_Student_Default::Create(m_vecStudent[1], L"_Normal_Callsign")
		);
		_float3	vOffset = { 1.5f,1.f, 1.5f };

		m_pEventCam->Ready_Event_Stage_Start(m_pStageCam, m_vecStudent[0],
			m_vecStudent[0]->Get_StateMachine()->Get_CurrentState()->Get_Animation(),
			vOffset, &m_vecStudent);
	}
	else
	{
		m_vecStudent[0]->Set_Transform(XMVectorSet(0.f, 0.f, 0.f, 1.f));
		m_vecStudent[1]->Set_Transform(XMVectorSet(1.f, 0.f, 0.f, 1.f));
		m_vecStudent[2]->Set_Transform(XMVectorSet(-1.f, 0.f, 0.f, 1.f));
	}
	
	return S_OK;
}

CCombatFormation * CCombatFormation::Create(FORMATIONDESC * pDesc, CCamera * pStageCam, CCamera * pEventCam)
{
	CCombatFormation*	pInstance = new CCombatFormation();
	
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
	m_vecStudent.clear();

}


