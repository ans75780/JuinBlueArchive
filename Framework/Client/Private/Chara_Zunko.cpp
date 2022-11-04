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
#include "Hod.h"

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


	m_bCreateBulletThree[0] = true;
	m_bCreateBulletThree[1] = true;
	m_bCreateBulletThree[2] = true;

	for (int i = 0; i < 50; i++)
	{
		m_bExBulletCreate[i] = true;
	}

	return S_OK;
}

void CChara_Zunko::Tick(_float fTimeDelta)
{
	if (m_bCharaDead)
		return;

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

	if(!m_bCharaDead)
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

	CGameInstance*		pGameInstanceSound = GET_INSTANCE(CGameInstance);

	int ranNum = Get_Random3();

	if (ranNum == 1)
		pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_ExSkill_1.ogg", 0.5f);
	else if (ranNum == 2)
		pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_ExSkill_2.ogg", 0.5f);
	else if (ranNum == 3)
		pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_ExSkill_3.ogg", 0.5f);

	RELEASE_INSTANCE(CGameInstance);

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

		CGameInstance*		pGameInstanceSound = GET_INSTANCE(CGameInstance);

		int ranNum = Get_Random3();

		if (ranNum == 1)
			pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_ExSkill_Level_1.ogg", 0.5f);
		else if (ranNum == 2)
			pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_ExSkill_Level_2.ogg", 0.5f);
		else if (ranNum == 3)
			pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_ExSkill_Level_3.ogg", 0.5f);

		RELEASE_INSTANCE(CGameInstance);

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

		for (int i = 0; i < 50; i++)
			m_bExBulletCreate[i] = true;
		m_iCount = 0;
		m_bSoundOnceZun = true;
	}

}

void CChara_Zunko::StateCheck(_float & fTimeDelta)
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
		Duration = m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_Ing")->Get_Duration();
		TimeAcc = m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_Ing")->Get_TimeAcc();

		if (m_bCreateBulletThree[0] && TimeAcc > 0.10f)
		{
			CGameInstance*		pGameInstanceSound = GET_INSTANCE(CGameInstance);
			pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"SFX_Common_HMG_ZunkoShot_01.wav", 0.3f);
			RELEASE_INSTANCE(CGameInstance);

			CreateBullet(0.3f);
			m_bCreateBulletThree[0] = false;
		}
		
		if (m_bCreateBulletThree[1] && TimeAcc > 0.20f)
		{
			CreateBullet(0.3f);
			m_bCreateBulletThree[1] = false;
		}

		if (m_bCreateBulletThree[2] && TimeAcc > 0.30f)
		{
			CreateBullet(0.3f);
			m_bCreateBulletThree[2] = false;
		}


		if (m_bAtkIngOnce)
		{
			m_iAmmo--;
			m_bAtkIngOnce = false;
		}
		if (m_pModelCom->Get_AnimationFromName("Zunko_Original_Normal_Attack_Ing")->IsFinished())
		{
			m_bCreateBulletThree[0] = true;
			m_bCreateBulletThree[1] = true;
			m_bCreateBulletThree[2] = true;

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
		Duration = m_pModelCom->Get_AnimationFromName("Zunko_Original_Exs")->Get_Duration();
		TimeAcc = m_pModelCom->Get_AnimationFromName("Zunko_Original_Exs")->Get_TimeAcc();

		for (int i = 0; i < 50; i++)
		{

			if (m_bExBulletCreate[i] && TimeAcc > 1.0f + (1.3f * ((_float)i / 49.f)))
			{
				if (m_bSoundOnceZun)
				{
					CGameInstance*		pGameInstanceSound = GET_INSTANCE(CGameInstance);
					pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"SFX_Common_AR_ZunkoEX.wav", 1.f);
					RELEASE_INSTANCE(CGameInstance);
					m_bSoundOnceZun = false;
				}
				m_bExBulletCreate[i] = false;
				CreateBullet(1.f);
			}
		}

		break;
	case VICTORY:
		break;
	case DEAD:
		if (m_bDeadOnce)
		{
			CGameInstance*		pGameInstanceSound = GET_INSTANCE(CGameInstance);
			pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_Battle_Damage_1.ogg", 0.5f);
			RELEASE_INSTANCE(CGameInstance);

			m_pModelCom->Set_CurrentAnimation(24);
			m_bDeadOnce = false;
		}
		Duration = m_pModelCom->Get_AnimationFromName("Zunko_Original_Vital_Death")->Get_Duration();
		TimeAcc = m_pModelCom->Get_AnimationFromName("Zunko_Original_Vital_Death")->Get_TimeAcc();


		if (TimeAcc > 1.4f)
		{
			CGameInstance*		pGameInstanceSound = GET_INSTANCE(CGameInstance);
			pGameInstanceSound->Get_Instance()->Get_SoundManager()->Play_Sound(L"Zunko_Battle_Retire.ogg", 0.5f);
			RELEASE_INSTANCE(CGameInstance);

			m_bCharaDead = true;
			m_pModelCom->Get_AnimationFromName("Zunko_Original_Vital_Death")->Pause();
		}
		break;
	case STATE_END:
		break;
	default:
		break;
	}
}

void CChara_Zunko::CreateBullet(_float _Damage)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix _mat, rotMat;
	rotMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));
	
	if (m_bExBulletCreate[0] == false)
	{
		m_iCount++;
		if (m_iCount % 2 == 0)
			_mat = m_pModelCom->Find_Bone("fire_01")->Get_CombinedMatrix();
		else
			_mat = m_pModelCom->Find_Bone("fire_03")->Get_CombinedMatrix();

	}
	else
		_mat = m_pModelCom->Find_Bone("fire_01")->Get_CombinedMatrix();

	_mat *= rotMat;
	_mat *= m_pTransformCom->Get_WorldMatrix();

	_float4	xPosPush;
	XMStoreFloat4(&xPosPush, _mat.r[3]);
	xPosPush.x += 0.2f;
	_mat.r[3] = XMLoadFloat4(&xPosPush);

	BulletDesc temp;
	temp.CreatePos = _mat.r[3];
	temp.TargetPos = m_pHod->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	temp.Damage = _Damage;		//Damage
	temp.Hod = m_pHod;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, TEXT("Layer_Effect_Bullet"),
		TEXT("Prototype_GameObject_Effect_Bullet"), &temp)))
		return;

	RELEASE_INSTANCE(CGameInstance);
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