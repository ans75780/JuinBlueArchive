#include "stdafx.h"
#include "..\Public\Arona_Camera.h"
#include "GameInstance.h"

CArona_Camera::CArona_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CArona_Camera::CArona_Camera(const CArona_Camera & rhs)
	: CCamera(rhs)
{
}

HRESULT CArona_Camera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CArona_Camera::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	lstrcpy(m_desc.sz_Name, TEXT("CArona_Camera"));

	return S_OK;
}

void CArona_Camera::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!IsMainCam())
		return;

	if (FAILED(Bind_PipeLine()))
		return;
}

void CArona_Camera::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CArona_Camera::Render()
{
	return S_OK;
}

CArona_Camera * CArona_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CArona_Camera*		pInstance = new CArona_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArona_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArona_Camera::Clone(void * pArg)
{
	CArona_Camera*		pInstance = new CArona_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CArona_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArona_Camera::Free()
{
	__super::Free();
}
