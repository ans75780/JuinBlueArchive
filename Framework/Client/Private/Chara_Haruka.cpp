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
	m_fForwardDistance = 5.5f;

	m_iMaxAmmo = 1;
	m_iAmmo = 1;
	m_fAttackDelay = 0.3f;

	return S_OK;
}

void CChara_Haruka::Tick(_float fTimeDelta)
{
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

void CChara_Haruka::HarukaExCheck()//한번실행됨
{
	m_bExTickStop = true;	//EX중 다른오브젝트 틱중지
	m_bExUse = true;		//본인만 틱돌기
	m_pHpBar->Set_Render(false);	//HP바 잠깐끄기
	m_pModelCom->Set_CurrentAnimation(25); //
	m_pAnimation_ExCutin = m_pModelCom->Get_AnimationFromName("");
	m_pAnimation_ExCutin->Reset();
	m_pCamera->PlayExs(this);
	m_eState = CHARA_STATE::EX;
}

void CChara_Haruka::ExCamCheck()
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
	}

}

void CChara_Haruka::ExsPlayOnce()// ex캠 복구후 한번들어옴 exs null아닐때 들어오는거임
{
	if (m_pAnimation_Exs->IsFinished())
	{
		m_pModelCom->Set_CurrentAnimation(5);
		m_pAnimation_Exs = nullptr;
	}

}

void CChara_Haruka::StateCheck(_float & fTimeDelta)
{
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