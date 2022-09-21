#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Channel.h"
#include "Model.h"
#include "Student.h"
#include "..\Public\Camera_Event.h"


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

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	_long		MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}

	Safe_Release(pGameInstance);

	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Event::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Event::Render()
{
	return S_OK;
}

void CCamera_Event::Set_Event(CCamera * pReturnCamera, CAnimation * pAnimation, _float3 vOffset)
{
	m_pReturnToCam = pReturnCamera;
	m_pAnimation = pAnimation;
	m_vOffset = vOffset;

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

