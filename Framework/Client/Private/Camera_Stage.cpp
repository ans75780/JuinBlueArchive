#include "stdafx.h"
#include "..\Public\Camera_Stage.h"
#include "GameInstance.h"
#include "Student.h"

CCamera_Stage::CCamera_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Stage::CCamera_Stage(const CCamera_Stage & rhs)
	: CCamera(rhs)
{
	
}

HRESULT CCamera_Stage::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Stage::Initialize(void * pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	lstrcpy(m_desc.sz_Name, TEXT("Camera_Stage"));

	m_fSpeed = 3.f;
	m_vOffset = { -3.f,4.5f, 0.f };
	return S_OK;
}

void CCamera_Stage::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!IsMainCam())
		return;

	if (nullptr == m_pTarget)
		return;


	m_pTransformCom->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector		vCamPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vCamPos += XMLoadFloat3(&m_vOffset);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);


	if (KEY(SPACE, TAP))
	{
		_vector vecPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		int a = 10;
	}

	
	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Stage::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Stage::Render()
{
	return S_OK;
}

CCamera_Stage * CCamera_Stage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Stage*		pInstance = new CCamera_Stage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Stage::Clone(void * pArg)
{
	CCamera_Stage*		pInstance = new CCamera_Stage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Stage::Set_Target(CStudent * pTarget)
{
	m_pTarget = pTarget;
}

void CCamera_Stage::Free()
{
	__super::Free();
}
