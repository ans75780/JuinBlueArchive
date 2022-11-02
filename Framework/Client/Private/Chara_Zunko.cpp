#include "stdafx.h"
#include "..\Public\Chara_Zunko.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

#include "Hod_CutScene_Cam.h"
#include "UI_Fade_White.h"
#include "HpBar.h"
#include "Camera_Free.h"

CChara_Zunko::CChara_Zunko(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharater(pDevice, pContext)
{
}

CChara_Zunko::CChara_Zunko(const CChara_Zunko & rhs)
	: CCharater(rhs)
{
}

HRESULT CChara_Zunko::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChara_Zunko::Initialize(void * pArg)
{
	__super::Initialize(TEXT("Prototype_Component_Model_Zunko_Original"));

	lstrcpy(m_desc.sz_Name, TEXT("Zunko"));
	m_fForwardDistance = 4.0f;

	m_iMaxAmmo = 5;
	m_iAmmo = 5;
	m_fAttackDelay = 0.3f;

	m_pModelCom->Set_CurrentAnimation(11); //Move_Ing
	return S_OK;
}

void CChara_Zunko::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bExTickStop && !m_bExUse)
		return;

	StateCheck(fTimeDelta);

	if (KEY(C, TAP))
		ZunkoExCheck();

	if (m_bExUse)
		ExCamCheck();

	if (nullptr != m_pAnimation_Exs)
		ExsPlayOnce();

	m_pModelCom->Play_Animation(fTimeDelta);

}

void CChara_Zunko::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CChara_Zunko::Render()
{
	__super::Render();

	return S_OK;
}

void CChara_Zunko::ZunkoExCheck()//한번실행됨
{
	m_bExTickStop = true;	//EX중 다른오브젝트 틱중지
	m_bExUse = true;		//본인만 틱돌기
	m_pHpBar->Set_Render(false);	//HP바 잠깐끄기
	m_pModelCom->Set_CurrentAnimation(18); //
	m_pAnimation_ExCutin = m_pModelCom->Get_AnimationFromName("Zunko_Original_Exs_Cutin");
	m_pAnimation_ExCutin->Reset();
	m_pCamera->PlayExs(this);
	m_eState = CHARA_STATE::EX_CUTIN;
}

void CChara_Zunko::ExCamCheck()
{
	if (nullptr == m_pAnimation_ExCutin)
		return;

	if (m_pAnimation_ExCutin->IsFinished())
	{
		m_bExTickStop = false;	//EX중 다른오브젝트 틱중지
		m_bExUse = false;		//본인만 틱돌기
		m_pHpBar->Set_Render(true);	//HP바 잠깐끄기
		m_pAnimation_ExCutin = nullptr;
		m_pModelCom->Set_CurrentAnimation(20); //
		m_pAnimation_Exs = m_pModelCom->Get_AnimationFromName("Zunko_Original_Exs");
		m_pAnimation_Exs->Reset();
		m_pCamera->EndExs();
		m_eState = CHARA_STATE::EX;
	}

}

void CChara_Zunko::ExsPlayOnce()// ex캠 복구후 한번들어옴 exs null아닐때 들어오는거임
{
	if (m_pAnimation_Exs->IsFinished())
	{
		m_pAnimation_Exs = nullptr;
		m_eState = CHARA_STATE::ATK_START;
		m_pModelCom->Set_CurrentAnimation(22);
		m_iAmmo = m_iMaxAmmo;
		m_bAtkIngOnce = true;
	}

}

void CChara_Zunko::StateCheck(_float & fTimeDelta)
{
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
			m_pModelCom->Set_CurrentAnimation(26);
		}
		break;
	case RUN_END:
		if (m_pModelCom->Get_AnimationFromName("Zunko_Original_Move_End_Normal")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Zunko_Original_Move_End_Normal")->Reset();
			m_eState = CHARA_STATE::ATK_START;
			m_pModelCom->Set_CurrentAnimation(22);
		}
		break;
	case RELOAD:
		if (m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Reload")->IsFinished())
		{
			m_iAmmo = m_iMaxAmmo;
			m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Reload")->Reset();
			m_eState = CHARA_STATE::ATK_START;
			m_pModelCom->Set_CurrentAnimation(22);
		}
		break;
	case ATK_START:
		if (m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_Start")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_Start")->Reset();
			m_eState = CHARA_STATE::ATK_ING;
			m_pModelCom->Set_CurrentAnimation(23);
		}
		break;
	case ATK_DELAY:
	//없는게나음
		break;
	case ATK_ING:
		if (m_bAtkIngOnce)
		{
			m_iAmmo--;
			m_bAtkIngOnce = false;
		}
		if (m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_Ing")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_Ing")->Reset();
			m_bAtkIngOnce = true;
			if (0 < m_iAmmo)
			{
				m_eState = CHARA_STATE::ATK_ING;
				m_pModelCom->Set_CurrentAnimation(23);
			}
			else
			{
				m_eState = CHARA_STATE::ATK_END;
				m_pModelCom->Set_CurrentAnimation(13);
			}
		}
		break;
	case ATK_END:
		if (m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_End")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_End")->Reset();
			m_eState = CHARA_STATE::RELOAD;
			m_pModelCom->Set_CurrentAnimation(32);
		}
		break;
	case EX_CUTIN:
		break;
	case EX:
		break;
	case VICTORY:
		break;
	case STATE_END:
		break;
	default:
		break;
	}
}

CChara_Zunko * CChara_Zunko::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CChara_Zunko*		pInstance = new CChara_Zunko(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChara_Zunko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChara_Zunko::Clone(void * pArg)
{
	CChara_Zunko*		pInstance = new CChara_Zunko(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChara_Zunko");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChara_Zunko::Free()
{
	__super::Free();
}