#include "stdafx.h"
#include "..\Public\Camera_Free.h"
#include "GameInstance.h"
#include "Engine_Defines.h"

#include "Charater.h"
#include "BoneNode.h"
#include "Animation.h"

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


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix mat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(-90.f));

	if (FAILED(__super::Add_Component(LEVEL_SHOP, TEXT("Aru_Cam"), TEXT("Com_Model_Aru"), (CComponent**)&m_pModelCom_AruCam)))
	{
		MSG_BOX("아루캠컴포넌트못가져옴");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_SHOP, TEXT("Haruka_Cam"), TEXT("Com_Model_Haruka"), (CComponent**)&m_pModelCom_HarukaCam)))
	{
		MSG_BOX("하루카캠컴포넌트못가져옴");
		return E_FAIL;
	}
	if (FAILED(__super::Add_Component(LEVEL_SHOP, TEXT("Zunko_Cam"), TEXT("Com_Model_Zunko"), (CComponent**)&m_pModelCom_ZunkoCam)))
	{
		MSG_BOX("준코캠컴포넌트못가져옴");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!IsMainCam())
		return;


	if (m_pChara != nullptr)
		ExCamPlay(fTimeDelta);


	//카메라컨트롤 사용
	if (KEY(F4, TAP))
	{
		if (m_bCameraControl)
			m_bCameraControl = false;
		else
			m_bCameraControl = true;
	}
	//그내용
	if (m_bCameraControl)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (KEY(Q, TAP))
		{
			//_float4 _right, _up, _look, _trans;

			//_vector _vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
			//_vector _vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
			//_vector _vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			//_vector _vTrans = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

			//XMStoreFloat4(&_right, _vRight);
			//XMStoreFloat4(&_up, _vUp);
			//XMStoreFloat4(&_look, _vLook);
			//XMStoreFloat4(&_trans, _vTrans);

			//_tchar temp[MAX_PATH];
			//_s_tprintf_s(temp, MAX_PATH, L"R = (%.2f, %.2f, %.2f, %.2f)\n U = (%.2f, %.2f, %.2f, %.2f)\n L = (%.2f, %.2f, %.2f, %.2f)\n T = (%.2f, %.2f, %.2f, %.2f) "
			//	, _right.x, _right.y, _right.z, _right.w
			//	, _up.x, _up.y, _up.z, _up.w
			//	, _look.x, _look.y, _look.z, _look.w
			//	, _trans.x, _trans.y, _trans.z, _trans.w);

			//MessageBox(0, temp, TEXT("System Error"), MB_OK);
		}
		if (KEY(E, TAP))
		{
			m_pTransformCom->LookAt(pGameInstance->Get_GameObjects(LEVEL_SHOP, TEXT("Layer_Effect_Bullet")).front()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
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

		Safe_Release(pGameInstance);
	}

	if (m_bShake)
		ShakeTick(fTimeDelta);


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

void CCamera_Free::PlayExs(CCharater * _chara)	//외부에서한번실행
{
	m_pChara = _chara;
	m_bExCamOnce = true;
	m_tOrigineMat.r[0] = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	m_tOrigineMat.r[1] = m_pTransformCom->Get_State(CTransform::STATE_UP);
	m_tOrigineMat.r[2] = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	m_tOrigineMat.r[3] = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
}

void CCamera_Free::EndExs()		////외부에서한번실행
{
	m_pChara = nullptr;
	m_CameraDesc.fFovy = XMConvertToRadians(65.f); //65.f
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_tOrigineMat.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, m_tOrigineMat.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_tOrigineMat.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tOrigineMat.r[3]);
}

void CCamera_Free::ExCamPlay(_float& fTimeDelta)
{
	if (m_bExCamOnce)
	{
		GetModelCam();
		m_bExCamOnce = false;
	}

	m_pModelCom->Play_Animation(fTimeDelta);

	OBJ_DESC temp = m_pChara->Get_Desc();
	if (!lstrcmp(TEXT("Aru"), temp.sz_Name))
		m_CameraDesc.fFovy = XMConvertToRadians(30.f); //65.f
	else if (!lstrcmp(TEXT("Haruka"), temp.sz_Name))
		m_CameraDesc.fFovy = XMConvertToRadians(10.f); //65.f
	else if (!lstrcmp(TEXT("Zunko"), temp.sz_Name))
		m_CameraDesc.fFovy = XMConvertToRadians(30.f); //65.f
	_float4 MoveCharaPos;
	XMStoreFloat4(&MoveCharaPos, m_pChara->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_matrix vRotateTemp = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(-90.f));

	_matrix vMatrix = m_pModelCom->Find_Bone("Camera001")->Get_CombinedMatrix();
	_matrix vTargetViewMatrix = m_pModelCom->Find_Bone("Camera001.Target")->Get_CombinedMatrix();

	vMatrix *= vRotateTemp;
	vTargetViewMatrix *= vRotateTemp;

	_float4 fMatPos;
	XMStoreFloat4(&fMatPos, vMatrix.r[3]);
	
	fMatPos.x += MoveCharaPos.x;
	fMatPos.y += MoveCharaPos.y;
	fMatPos.z += MoveCharaPos.z;

	_vector MatFinalPos = XMLoadFloat4(&fMatPos);
	vMatrix.r[3] = MatFinalPos;

	_float4 fTargetPos;
	XMStoreFloat4(&fTargetPos, vTargetViewMatrix.r[3]);

	fTargetPos.x += MoveCharaPos.x;
	fTargetPos.y += MoveCharaPos.y;
	fTargetPos.z += MoveCharaPos.z;

	_vector TargetFinalPos = XMLoadFloat4(&fTargetPos);
	vTargetViewMatrix.r[3] = TargetFinalPos;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vMatrix.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vMatrix.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMatrix.r[2]);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMatrix.r[3]);
	m_pTransformCom->LookAt(vTargetViewMatrix.r[3]);
}

void CCamera_Free::GetModelCam()
{
	//모델 연결시켜주기
	
	OBJ_DESC temp = m_pChara->Get_Desc();

	if (!lstrcmp(TEXT("Aru"), temp.sz_Name))
		m_pModelCom = m_pModelCom_AruCam;
	else if (!lstrcmp(TEXT("Haruka"), temp.sz_Name))
		m_pModelCom = m_pModelCom_HarukaCam;
	else if (!lstrcmp(TEXT("Zunko"), temp.sz_Name))
		m_pModelCom = m_pModelCom_ZunkoCam;

	if(nullptr != m_pModelCom)
		m_pModelCom->Play_Animation(0);


}

void CCamera_Free::RotateMat(_matrix * _Mat, _fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * (XMVectorGetX(XMVector3Length(_Mat->r[0]))); //RIGHT
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * (XMVectorGetX(XMVector3Length(_Mat->r[1])));	//UP
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * (XMVectorGetX(XMVector3Length(_Mat->r[2])));	//LOOK

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	_Mat->r[0] = vRight;
	_Mat->r[1] = vUp;
	_Mat->r[2] = vLook;
}

void CCamera_Free::Shake(_float _time)
{
	m_fShakeTime = _time;
	m_bShake = true;
	m_vOringinPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	m_fShakeAver = 0;
	m_iShakeCount = 0;
	_float4 temp;
	XMStoreFloat4(&temp, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	m_fSetX = temp.x;
}

void CCamera_Free::ShakeTick(_float& fTimeDelta)
{
	_float4 thisPosF;
	_vector  thisPosV = m_vOringinPos;
	XMStoreFloat4(&thisPosF, thisPosV);

	if (m_iShakeCount++ >= 5)
	{
		if (m_fShakeAver  < 0.6f)
		{
			m_fShakeAver += 0.3f;
		}
		else if (m_fShakeAver  > -0.6f)
		{
			m_fShakeAver -= 0.3f;
		}
		m_iShakeCount = 0;
	}

	thisPosF.x = m_fSetX + m_fShakeAver;
	thisPosF.y += m_fShakeAver * 0.5f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&thisPosF));

	m_fShakeValue += fTimeDelta;
	if (m_fShakeTime < m_fShakeValue)
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_vOringinPos);
		m_fShakeValue = 0.f;
		m_fShakeTime = 0.f;
		m_bShake = false;
	}

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
