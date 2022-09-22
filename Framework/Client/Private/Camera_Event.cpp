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

void CCamera_Event::Ready_Event_Stage_Start(CCamera * pReturnCamera, CGameObject * pTarget, CAnimation * pAnimation, _float3 vOffset)
{
	m_pReturnToCam = pReturnCamera;
	m_pAnimation = pAnimation;
	m_vOffset = vOffset;

	m_pTarget = pTarget;
	m_eEventType = EVENT_TYPE::EVENT_STAGE_START;

	_vector		vCamPos = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vCamPos += XMLoadFloat3(&vOffset);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);

	CCamera::Set_MainCam(this);
}

void CCamera_Event::Ready_Event_Ex(CCamera * pReturnCamera, CGameObject * pTarget)
{
	m_pReturnToCam = pReturnCamera;

	m_pTarget = pTarget;

	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, ((CStudent*)m_pTarget)->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	//28
	strcat_s(pAnimaitonStr, "_Original_Exs_Cutin_Cam");

	m_pAnimation = ((CStudent*)m_pTarget)->Get_Animation(pAnimaitonStr);

	m_pAnimation->Play();
	m_eEventType = EVENT_TYPE::EVENT_EX;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	CCamera::Set_MainCam(this);

}

void CCamera_Event::Event_Stage_Start()
{
	if (m_pAnimation->IsFinished() == true)
	{
		CCamera::Set_MainCam(m_pReturnToCam);
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
	}
	m_pAnimation->Update(fTimeDelta);

	m_CameraDesc.fFovy = XMConvertToRadians(15.0f);
	_matrix vMatrix = ((CModel*)m_pTarget->Get_Component(L"Com_Model"))->Find_Bone("Camera001")->Get_CombinedMatrix();
	_matrix vTargetMatrix = ((CModel*)m_pTarget->Get_Component(L"Com_Model"))->Find_Bone("Camera001.Target")->Get_CombinedMatrix();


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMatrix.r[2]);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMatrix.r[3]);


	m_pTransformCom->LookAt(vTargetMatrix.r[3]);

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

