#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"
#include "Engine_Defines.h"
CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	/* 트랜스폼 컴포넌늩를 추가한다. */
	/* 내가 원하는 카메라의 초기상태를 동기화하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	lstrcpy(m_desc.sz_Name, TEXT("Camera_Free"));

	CCamera::Set_MainCam(this);

	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!IsMainCam())
		return;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);		

	if (KEY(Q,TAP))
	{
		//_float4 _vEye;

		//_vector eye = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		//XMStoreFloat4(&_vEye, eye);

		//_tchar temp[MAX_PATH];
		//_stprintf_s(temp, MAX_PATH, L"pos = (%.2f, %.2f, %.2f, %.2f)", _vEye.x, _vEye.y, _vEye.z, _vEye.w);

		//MessageBox(0, temp, TEXT("System Error"), MB_OK);

		_float4 _right, _up, _look, _trans;

		_vector _vRight	= m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector _vUp	= m_pTransformCom->Get_State(CTransform::STATE_UP);
		_vector _vLook	= m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_vector _vTrans	= m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		
		XMStoreFloat4(&_right, _vRight);
		XMStoreFloat4(&_up, _vUp);
		XMStoreFloat4(&_look, _vLook);
		XMStoreFloat4(&_trans, _vTrans);

		_tchar temp[MAX_PATH];
		_stprintf_s(temp, MAX_PATH, L"R = (%.2f, %.2f, %.2f, %.2f)\n U = (%.2f, %.2f, %.2f, %.2f)\n L = (%.2f, %.2f, %.2f, %.2f)\n T = (%.2f, %.2f, %.2f, %.2f) "
			, _right.x,		_right.y,		_right.z,		_right.w
			, _up.x,		_up.y,			_up.z,			_up.w
			, _look.x,		_look.y,		_look.z,		_look.w
			, _trans.x,		_trans.y,		_trans.z,		_trans.w);

		MessageBox(0, temp, TEXT("System Error"), MB_OK);

	}


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
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove);
	}


	if(KEY(SPACE, TAP))
	{
		_vector vecPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		
		int a = 10;
	}

	Safe_Release(pGameInstance);

	if (FAILED(Bind_PipeLine()))
		return ;
}

void CCamera_Free::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free*		pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void * pArg)
{
	CCamera_Free*		pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
