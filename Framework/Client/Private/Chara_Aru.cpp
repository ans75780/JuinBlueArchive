#include "stdafx.h"
#include "..\Public\Chara_Aru.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

#include "Hod_CutScene_Cam.h"
#include "UI_Fade_White.h"
#include "HpBar.h"
#include "Camera_Free.h"

CChara_Aru::CChara_Aru(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharater(pDevice, pContext)
{
}

CChara_Aru::CChara_Aru(const CChara_Aru & rhs)
	: CCharater(rhs)
{
}

HRESULT CChara_Aru::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChara_Aru::Initialize(void * pArg)
{
	__super::Initialize(TEXT("Prototype_Component_Model_Aru_Original"));

	lstrcpy(m_desc.sz_Name, TEXT("Aru"));
	m_fForwardDistance = 3.f;
	
	m_iMaxAmmo = 6;
	m_iAmmo = 6;
	m_fAttackDelay = 1.f;

	m_pModelCom->Set_CurrentAnimation(28); //Move_Ing
	return S_OK;
}

void CChara_Aru::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bExTickStop && !m_bExUse)
		return;

	StateCheck(fTimeDelta);


	if (KEY(Z, TAP))
		AruExCheck();

	if (m_bExUse)
		ExCamCheck();

	if (nullptr != m_pAnimation_Exs)
		ExsPlayOnce();
	
	m_pModelCom->Play_Animation(fTimeDelta);


}

void CChara_Aru::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CChara_Aru::Render()
{
	__super::Render();

	return S_OK;
}

void CChara_Aru::AruExCheck()//한번실행됨
{
	m_bExTickStop = true;	//EX중 다른오브젝트 틱중지
	m_bExUse = true;		//본인만 틱돌기
	m_pHpBar->Set_Render(false);	//HP바 잠깐끄기
	m_pModelCom->Set_CurrentAnimation(25); //
	m_pAnimation_ExCutin = m_pModelCom->Get_AnimationFromName("Aru_Original_Exs_Cutin");
	m_pAnimation_ExCutin->Reset();
	m_pCamera->PlayExs(this);
	m_eState = CHARA_STATE::EX_CUTIN;
}

void CChara_Aru::ExCamCheck()
{
	if (nullptr == m_pAnimation_ExCutin)
		return;

	if (m_pAnimation_ExCutin->IsFinished())
	{
		m_bExTickStop = false;	//EX중 다른오브젝트 틱중지
		m_bExUse = false;		//본인만 틱돌기
		m_pHpBar->Set_Render(true);	//HP바 잠깐끄기
		m_pAnimation_ExCutin = nullptr;
		m_pModelCom->Set_CurrentAnimation(17); //
		m_pAnimation_Exs = m_pModelCom->Get_AnimationFromName("Aru_Original_Exs");
		m_pAnimation_Exs->Reset();
		m_pCamera->EndExs();
		m_eState = CHARA_STATE::EX;
	}

}

void CChara_Aru::ExsPlayOnce()// ex캠 복구후 한번들어옴 exs null아닐때 들어오는거임
{
	if (m_pAnimation_Exs->IsFinished())
	{
		m_pAnimation_Exs = nullptr;
		m_eState = CHARA_STATE::ATK_START;
		m_pModelCom->Set_CurrentAnimation(20);
		m_iAmmo = 6;
		m_bAtkIngOnce = true;

	}

}

void CChara_Aru::StateCheck(_float & fTimeDelta)
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
			m_eState = CHARA_STATE::ATK_START;
			m_fTime = 0.f;
			m_pModelCom->Set_CurrentAnimation(20); //ATK_START
		}
		break;
	case RUN_END:
		break;
	case RELOAD:
		if (m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Reload")->IsFinished())
		{
			m_iAmmo = m_iMaxAmmo;
			m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Reload")->Reset();
			m_eState = CHARA_STATE::ATK_START;
			m_pModelCom->Set_CurrentAnimation(20);
		}
		break;
	case ATK_START:
		if (m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Attack_Start")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Attack_Start")->Reset();
			m_eState = CHARA_STATE::ATK_ING;
			m_pModelCom->Set_CurrentAnimation(0);
		}
		break;
	case ATK_DELAY:
		//안쓰는게좋음
		break;
	case ATK_ING:
		if (m_bAtkIngOnce)
		{
			m_iAmmo--;
			m_bAtkIngOnce = false;
		}
		if (m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Attack_Ing")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Attack_Ing")->Reset();
			m_bAtkIngOnce = true;
			if (0 < m_iAmmo)
			{
				m_eState = CHARA_STATE::ATK_ING;
				m_pModelCom->Set_CurrentAnimation(0);
			}
			else
			{
				m_eState = CHARA_STATE::ATK_END;
				m_pModelCom->Set_CurrentAnimation(13);
			}
		}
		break;
	case ATK_END:
		if (m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Attack_End")->IsFinished())
		{
			m_pModelCom->Get_AnimationFromName("Aru_Original_Normal_Attack_End")->Reset();
			m_eState = CHARA_STATE::RELOAD;
			m_pModelCom->Set_CurrentAnimation(2);
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

CChara_Aru * CChara_Aru::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CChara_Aru*		pInstance = new CChara_Aru(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChara_Aru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChara_Aru::Clone(void * pArg)
{
	CChara_Aru*		pInstance = new CChara_Aru(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChara_Aru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChara_Aru::Free()
{
	__super::Free();
}