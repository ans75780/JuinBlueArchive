#include "stdafx.h"
#include "..\Public\Camera_Formation.h"
#include "GameInstance.h"

CCamera_Formation::CCamera_Formation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Formation::CCamera_Formation(const CCamera_Formation & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Formation::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Formation::Initialize(void * pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	lstrcpy(m_desc.sz_Name, TEXT("Camera_Free"));

	return S_OK;
}

void CCamera_Formation::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (FAILED(Bind_PipeLine()))
		return;
}

void CCamera_Formation::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Formation::Render()
{
	return S_OK;
}

CCamera_Formation * CCamera_Formation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Formation*		pInstance = new CCamera_Formation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Formation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Formation::Clone(void * pArg)
{
	CCamera_Formation*		pInstance = new CCamera_Formation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Formation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Formation::Free()
{
	__super::Free();
}
