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

	CCamera::Set_MainCam(this);

	return S_OK;
}

void CArona_Camera::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!IsMainCam())
		return;

	static bool _once = true;

	if (_once)
	{
		_once = false;
		LookAtArona();
	}


	CameraMove(fTimeDelta);

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

void CArona_Camera::CameraMove(_float & fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta * 0.5f);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta * 0.5f);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta * 0.5f);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta * 0.5f);
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

	if (KEY(SPACE, TAP))
	{

	}

	Safe_Release(pGameInstance);
}

void CArona_Camera::LookAtArona()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransformCom->LookAt
	(pGameInstance->Get_GameObjects(LEVEL_GACHA_PLAY, L"Layer_Student").front()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));


	RELEASE_INSTANCE(CGameInstance);
}
