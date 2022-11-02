#include "stdafx.h"
#include "..\Public\Chara_Haruka.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

#include "Hod_CutScene_Cam.h"
#include "UI_Fade_White.h"
#include "HpBar.h"
#include "Camera_Free.h"
#include "Hod.h"

CChara_Haruka::CChara_Haruka(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharater(pDevice, pContext)
{
}

CChara_Haruka::CChara_Haruka(const CChara_Haruka & rhs)
	: CCharater(rhs)
{
}

HRESULT CChara_Haruka::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChara_Haruka::Initialize(void * pArg)
{
	__super::Initialize(TEXT("Prototype_Component_Model_Haruka_Original"));

	lstrcpy(m_desc.sz_Name, TEXT("Haruka"));
	m_fForwardDistance = 4.5f;

	m_iMaxAmmo = 5;
	m_iAmmo = 5;
	m_fAttackDelay = 0.3f;

	m_pModelCom->Set_CurrentAnimation(23); //Move_Ing
	return S_OK;
}

void CChara_Haruka::Tick(_float fTimeDelta)
{
	if (m_bCharaDead)
		return;

	__super::Tick(fTimeDelta);

	if (m_bExTickStop && !m_bExUse)
		return;

	StateCheck(fTimeDelta);

	if (KEY(X, TAP))
		HarukaExCheck();

	if (m_bExUse)
		ExCamCheck();

	if (nullptr != m_pAnimation_Exs)
		ExsPlayOnce();

	if (!m_bCharaDead)
		m_pModelCom->Play_Animation(fTimeDelta);

}

void CChara_Haruka::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CChara_Haruka::Render()
{
	__super::Render();

	return S_OK;
}

void CChara_Haruka::HarukaExCheck()//�ѹ������
{
	m_bExTickStop = true;	//EX�� �ٸ�������Ʈ ƽ����
	m_bExUse = true;		//���θ� ƽ����
	m_pHpBar->Set_Render(false);	//HP�� ������
	m_pModelCom->Set_CurrentAnimation(3); //
	m_pAnimation_ExCutin = m_pModelCom->Get_AnimationFromName("Haruka_Original_Exs_Cutin");
	m_pAnimation_ExCutin->Reset();
	m_pCamera->PlayExs(this);
	m_eState = CHARA_STATE::EX_CUTIN;
}

void CChara_Haruka::ExCamCheck()
{
	if (nullptr == m_pAnimation_ExCutin)
		return;

	if (m_pAnimation_ExCutin->IsFinished())
	{
		m_bExTickStop = false;	//EX�� �ٸ�������Ʈ ƽ����
		m_bExUse = false;		//���θ� ƽ����
		m_pHpBar->Set_Render(true);	//HP�� ������
		m_pAnimation_ExCutin = nullptr;
		m_pModelCom->Set_CurrentAnimation(25); //
		m_pAnimation_Exs = m_pModelCom->Get_AnimationFromName("Haruka_Original_Exs_1");
		m_pAnimation_Exs->Reset();
		m_pCamera->EndExs();
		m_eState = CHARA_STATE::EX;
	}

}

void CChara_Haruka::ExsPlayOnce()// exķ ������ �ѹ����� exs null�ƴҶ� �����°���
{
	if (m_pAnimation_Exs->IsFinished())
	{
		m_pAnimation_Exs = nullptr;
		m_eState = CHARA_STATE::ATK_START;
		m_pModelCom->Set_CurrentAnimation(38);
		m_iAmmo = m_iMaxAmmo;
		m_bAtkIngOnce = true;
	}

}

void CChara_Haruka::StateCheck(_float & fTimeDelta)
{
	_float Duration;
	_float TimeAcc;


	switch (m_eState)
	{
	case IDLE:
		break;
	case RUN:
		m_fTime += fTimeDelta;
		m_pTransformCom->Go_Straight(fTimeDelta);
		if (m_fForwardDistance < m_fTime)
		{
			m_eState = CHARA_STATE::RUN_END;
			m_fTime = 0.f;
			m_pModelCom->Set_CurrentAnimation(8);
		}
		break;
	case RUN_END:
		if (m_pModelCom->Get_AnimationFromName("Haruka_Original_Move_End_Normal")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Haruka_Original_Move_End_Normal")->Reset();
			m_eState = CHARA_STATE::ATK_START;
			m_pModelCom->Set_CurrentAnimation(38);
		}
		break;
	case RELOAD:
		if (m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Reload")->IsFinished())
		{
			m_iAmmo = m_iMaxAmmo;
			m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Reload")->Reset();
			m_eState = CHARA_STATE::ATK_START;
			m_pModelCom->Set_CurrentAnimation(38);
		}
		break;
	case ATK_START:
		if (m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Start")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Start")->Reset();
			m_eState = CHARA_STATE::ATK_ING;
			m_pModelCom->Set_CurrentAnimation(15);
		}
		break;
	case ATK_DELAY:
		if (m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Delay")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Delay")->Reset();
			m_eState = CHARA_STATE::ATK_ING;
			m_pModelCom->Set_CurrentAnimation(15);
		}
		break;
	case ATK_ING:
		Duration = m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Ing")->Get_Duration();
		TimeAcc = m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Ing")->Get_TimeAcc();

		if (m_bBulletCreateOnce && TimeAcc > 0.09f)
		{
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
			_matrix _mat, rotMat;

			rotMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));
			_mat = m_pModelCom->Find_Bone("fire_01")->Get_CombinedMatrix();
			_mat *= rotMat;
			_mat *= m_pTransformCom->Get_WorldMatrix();

			_float4	xPosPush;
			XMStoreFloat4(&xPosPush, _mat.r[3]);
			xPosPush.x += 0.9f;
			xPosPush.y += 0.1f;
			_mat.r[3] = XMLoadFloat4(&xPosPush);

			if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, TEXT("Layer_Effect_ShotGun"),
				TEXT("Prototype_GameObject_Effect_ShotGun"), &_mat)))
				return;

			RELEASE_INSTANCE(CGameInstance);
			m_pHod->DamageAction(1.f);
			m_bBulletCreateOnce = false;
		}

		if (m_bAtkIngOnce)
		{
			m_iAmmo--;
			m_bAtkIngOnce = false;
		}
		if (m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Ing")->IsFinished())
		{
			m_bBulletCreateOnce = true;
			m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_Ing")->Reset();
			m_bAtkIngOnce = true;
			if (0 < m_iAmmo)
			{
				m_eState = CHARA_STATE::ATK_DELAY;
				m_pModelCom->Set_CurrentAnimation(33);
			}
			else
			{
				m_eState = CHARA_STATE::ATK_END;
				m_pModelCom->Set_CurrentAnimation(5);
			}
		}
		break;
	case ATK_END:
		if (m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_End")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Haruka_Original_Normal_Attack_End")->Reset();
			m_eState = CHARA_STATE::RELOAD;
			m_pModelCom->Set_CurrentAnimation(43);
		}
		break;
	case EX_CUTIN:
		break;
	case EX:
		m_pTransformCom->Go_Straight(fTimeDelta * 0.01f);
		break;
	case VICTORY:
		break;
	case DEAD:
		if (m_bDeadOnce)
		{
			m_pModelCom->Set_CurrentAnimation(18);
			m_bDeadOnce = false;
		}
		Duration = m_pModelCom->Get_AnimationFromName("Haruka_Original_Vital_Death")->Get_Duration();
		TimeAcc = m_pModelCom->Get_AnimationFromName("Haruka_Original_Vital_Death")->Get_TimeAcc();

		if (TimeAcc > 1.9f)
		{
			m_bCharaDead = true;
			m_pModelCom->Get_AnimationFromName("Haruka_Original_Vital_Death")->Pause();
		}
		break;
	case STATE_END:
		break;
	default:
		break;
	}
}

CChara_Haruka * CChara_Haruka::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CChara_Haruka*		pInstance = new CChara_Haruka(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChara_Haruka");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChara_Haruka::Clone(void * pArg)
{
	CChara_Haruka*		pInstance = new CChara_Haruka(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChara_Haruka");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChara_Haruka::Free()
{
	__super::Free();
}