#include "stdafx.h"
#include "Enemy.h"
#include "Actor.h"
#include "GameInstance.h"
#include "MeshContainer.h"
#include "StateMachineBase.h"
#include "State_Idle.h"
#include "State_Run.h"
#include "Collider.h"
#include "State_Student_Formation_Idle.h"
#include "Animation.h"
#include "HpBar.h"
CEnemy::CEnemy(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CActor(pDevice, pContext)
{
}


CEnemy::CEnemy(const CEnemy & rhs)
	: CActor(rhs)
{

}

HRESULT CEnemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	//GameObject 에서 디스크립션 초기화해서 먼저 이거 해줘야함.
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	memcpy(&m_desc, pArg, sizeof(OBJ_DESC));

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (FAILED(SetUp_StateMachine(0)))
		return E_FAIL;


	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);



	RELEASE_INSTANCE(CGameInstance);



	m_pTransformCom->Set_Scaled(_float3(1.00f, 1.00f, 1.00f));

	return S_OK;
}

void CEnemy::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CEnemy::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CEnemy::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CEnemy::SetUp_StateMachine(_uint iClonedLevel)
{
	m_pStateMachine = CStateMachineBase::Create(this);
	m_pStateMachine->Setup_StateMachine(CState_Idle::Create(this));

	return S_OK;
}

CEnemy * CEnemy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnemy*		pInstance = new CEnemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEnemy::Clone(void * pArg)
{
	CEnemy*		pInstance = new CEnemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy::Free()
{
	__super::Free();

}
