#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Channel.h"
#include "Model.h"
#include "Student.h"
#include "..\Public\Camera_Event.h"
#include "StateMachineBase.h"
#include "BoneNode.h"
#include "Actor.h"
#include "HpBar.h"
#include "State_Student_Run.h"
#include "State_Student_Ex_Cutin.h"
#include "State_Student_Ex.h"
#include "Stage.h"
#include "State_Student_Victory.h"
CCamera_Event::CCamera_Event(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Event::CCamera_Event(const CCamera_Event & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Event::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Event::Initialize(void * pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	lstrcpy(m_desc.sz_Name, TEXT("Camera_Event"));

	m_MatExRot = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));


	m_vecStartPos.push_back(_float4(0.0f , 0.f, 0.f, 1.f));
	m_vecStartPos.push_back(_float4(1.0f, 0.f, 0.f, 1.f));
	m_vecStartPos.push_back(_float4(-1.0f, 0.f, 0.f, 1.f));
	

	return S_OK;
}

void CCamera_Event::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!IsMainCam())
		return;

	switch (m_eEventType)
	{
	case Client::CCamera_Event::EVENT_TYPE::EVENT_STAGE_START:
		Event_Stage_Start();
		break;
	case Client::CCamera_Event::EVENT_TYPE::EVENT_EX:
		Event_Ex(fTimeDelta);
		break;
	case Client::CCamera_Event::EVENT_TYPE::EVENT_BOSS_APPEAR:
		break;
	case Client::CCamera_Event::EVENT_TYPE::EVENT_STAGE_VICTORY:
		Event_Stage_Victory();
		break;
	case Client::CCamera_Event::EVENT_TYPE::EVENT_END:
		break;
	default:
		break;
	}

	if (IsMainCam())
	{
		if (FAILED(Bind_PipeLine()))
			return;
	}
	
}

void CCamera_Event::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Event::Render()
{
	return S_OK;
}


void CCamera_Event::Ready_Event_Stage_Start(CCamera * pReturnCamera, CActor * pTarget, CAnimation * pAnimation, _float3 vOffset, vector<CStudent*>* pVecStudents)
{
	m_pReturnToCam = pReturnCamera;
	m_pAnimation = pAnimation;
	m_vOffset = vOffset;

	m_pTarget = pTarget;
	m_eEventType = EVENT_TYPE::EVENT_STAGE_START;

	_vector		vCamPos = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vCamPos += XMLoadFloat3(&vOffset);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);

	m_pVecStduent = pVecStudents;

	m_pTarget->Get_HpBar()->Set_Enable(false);

	CCamera::Set_MainCam(this);



}

void CCamera_Event::Ready_Event_Ex(CCamera * pReturnCamera, CActor * pTarget, CActor * pEx)
{
	m_pReturnToCam = pReturnCamera;
	m_pTargetEx = pEx;
	m_pTarget = pTarget;

	char pAnimaitonStr[MAX_PATH];
	char pAnimationPullName[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, ((CActor*)m_pTarget)->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	//28

	strcpy_s(pAnimationPullName, pAnimaitonStr);

	strcat_s(pAnimationPullName, "_Exs_Cutin_Cam");

	m_pAnimation = m_pTarget->Get_Animation(pAnimationPullName);

	if (m_pAnimation == nullptr)
	{
		strcpy_s(pAnimationPullName, pAnimaitonStr);

		strcat_s(pAnimationPullName, "_Exs_Cam");

		m_pAnimation = m_pTarget->Get_Animation(pAnimationPullName);
		if (nullptr == m_pAnimation)
		{
			MSG_BOX("Load Failed : Ex Cam");
		}
	}
	m_pAnimation->Play();
	m_eEventType = EVENT_TYPE::EVENT_EX;


	_vector	vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	XMStoreFloat4(&m_fTargetOriginPos, vTargetPos);

	//EX때는 캐릭터를 원점으로 돌려놓음(왜? 노드에 카메라가 원점을 기준으로 되어잇음)
	m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	CCamera::Set_MainCam(this);

	//이벤트 시작될동안 이벤트 오브젝트에 들어가있는 애들 빼고는 틱 멈춤
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);
	pInstance->Start_Event();


	pInstance->Add_EventObject(this);
	pInstance->Add_EventObject(m_pTarget);

	m_pTarget->Get_StateMachine()->Add_State(CState_Student_Ex_Cutin::Create(m_pTarget));

	m_pTarget->Set_Enable(true);

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Event::Ready_Event_Stage_Victory(CStage * pTarget, _float3 vOffset, vector<CStudent*>* pVecStudents)
{
	m_vOffset = vOffset;

	m_eEventType = EVENT_TYPE::EVENT_STAGE_VICTORY;

	_vector		vCamPos = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vCamPos += XMLoadFloat3(&vOffset);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);

	m_pVecStduent = pVecStudents;


	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);
	pInstance->Start_Event();


	pInstance->Add_EventObject(this);
	pInstance->Add_EventObject(pTarget);

	_uint i = 0;

	_vector pos = (*pVecStudents)[0]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	for (auto& elem : *pVecStudents)
	{
		_vector Amount = XMVectorSet(-1.5f + (1.5f * i), 0.f ,0.f, 1.f);

		_vector victoryPos = (pos + Amount);

		elem->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, victoryPos);

		pInstance->Add_EventObject(elem);
		elem->Get_StateMachine()->Add_State(CState_Student_Victory::Create(elem));


		i++;

	}

	m_pTarget->Set_Enable(true);

	RELEASE_INSTANCE(CGameInstance);

	CCamera::Set_MainCam(this);
}


void CCamera_Event::Event_Stage_Start()
{
	if (m_pAnimation->IsFinished() == true)
	{

		for (_uint i = 0; i < m_pVecStduent->size();i++)
		{
			(*m_pVecStduent)[i]->Set_Transform(XMLoadFloat4(&m_vecStartPos[i]));
			(*m_pVecStduent)[i]->Get_StateMachine()->Add_State(CState_Student_Run::Create((*m_pVecStduent)[i]));
		}
		CCamera::Set_MainCam(m_pReturnToCam);
		m_pTarget->Get_HpBar()->Set_Enable(true);
	}
	
	m_pTransformCom->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector		vCamPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vCamPos += XMLoadFloat3(&m_vOffset);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);

}

void CCamera_Event::Event_Ex(_float fTimeDelta)
{
	if (m_pAnimation->IsFinished())
	{
		m_CameraDesc.fFovy = XMConvertToRadians(65.0f);

		CCamera::Set_MainCam(m_pReturnToCam);
		m_pAnimation->Reset();

	//	m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_fTargetOriginPos));

		CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);
	
		pInstance->Clear_Event();


		RELEASE_INSTANCE(CGameInstance);
		m_pTarget->Set_Enable(false);
		m_pTargetEx->Get_StateMachine()->Add_State(CState_Student_Ex::Create(m_pTargetEx));
		return;
	}
	m_pAnimation->Update(fTimeDelta);

	m_CameraDesc.fFovy = XMConvertToRadians(15.0f);
	_matrix vMatrix = m_pTarget->Get_ModelCom()->Find_Bone("Camera001")->Get_CombinedMatrix();


	_matrix vTargetViewMatrix = m_pTarget->Get_ModelCom()->Find_Bone("Camera001.Target")->Get_CombinedMatrix();
	_vector	vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMatrix.r[2]);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMatrix.r[3]);


	m_pTransformCom->LookAt(vTargetViewMatrix.r[3]);

}

void CCamera_Event::Event_Stage_Victory()
{
	m_pTransformCom->LookAt((*m_pVecStduent)[1]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector		vCamPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vCamPos += XMLoadFloat3(&m_vOffset);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);
}

CCamera_Event * CCamera_Event::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Event*		pInstance = new CCamera_Event(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Event");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Event::Clone(void * pArg)
{
	CCamera_Event*		pInstance = new CCamera_Event(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Event");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Event::Free()
{
	__super::Free();
}

