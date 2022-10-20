#include "stdafx.h"
#include "..\Public\Arona_Camera.h"
#include "GameInstance.h"

#include "Arona_GachaCam.h"
#include "Arona.h"
#include "Animation.h"
#include "BoneNode.h"
#include "MeshContainer.h"

#include "UI.h"
#include "UI_GachaCard.h"

#include "UI_Fade_White.h"
#include "UI_Fade_Black.h"

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


	if (m_bTickOnce)
	{
		m_bTickOnce = false;
		LookAtArona();
		LinkCam();
	}

	TickGacha(fTimeDelta);
	
	if (m_pAronaAni->IsFinished())
		MakeUI();
	
	if (m_bOpenEvent)
		CardOpen();

	//if (KEY(SPACE, TAP))
	//{
	//	static_cast<CUI_Fade_Black*>(m_pFadeBlack)->Set_AlphaValue(1.f);
	//}

	//CameraMove(fTimeDelta);
	
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
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}

	if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
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

void CArona_Camera::LinkCam()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	m_pCam = static_cast<CArona_GachaCam*>(pGameInstance->Get_GameObjects(LEVEL_GACHA_PLAY, L"Layer_Gacha_Cam").front());
	m_pCamAni = m_pCam->Get_Animation("Arona_Original_Gacha_Anim");
	
	m_pArona = static_cast<CArona*>(pGameInstance->Get_GameObjects(LEVEL_GACHA_PLAY, L"Layer_Student").front());
	m_pAronaAni = m_pArona->Get_Animation("ARONA|Arona_Original_Gacha_Anim|Base Layer");

	m_pCamAni->Play();
	m_pAronaAni->Play();

	m_pCam->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_pArona->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);
}

void CArona_Camera::TickGacha(_float & fTimeDelta)
{
	if (!m_bTickGachaOnce)
		return;

	if (nullptr == m_pCam || nullptr == m_pCamAni)
		return;

	if (m_pAronaAni->IsFinished() && m_bTickGachaOnce)
	{
		m_pTransformCom->Go_Up(3.f);


		m_bTickGachaOnce = false;
		return;
	}


	if (!m_pAronaAni->IsFinished())
	{
		if (117.f < m_pAronaAni->Get_TimeAcc())
		{
			m_pAronaAni->Set_TimeAcc(126.f);
			m_pCamAni->Set_TimeAcc(5.f);
		}
		if (KEY(TAB,TAP))
		{
			m_pAronaAni->Set_TimeAcc(126.f);
			m_pCamAni->Set_TimeAcc(5.f);
		}

		m_pCam->Get_Model()->Play_Animation(fTimeDelta * 0.8f);
		m_pArona->Get_Model()->Play_Animation(fTimeDelta * 0.8f);

		m_CameraDesc.fFovy = XMConvertToRadians(10.0f);


		_matrix vMatrix = m_pCam->Get_Model()->Find_Bone("Camera001")->Get_CombinedMatrix();
		_matrix vTargetViewMatrix = m_pCam->Get_Model()->Find_Bone("Camera001.Target")->Get_CombinedMatrix();
		
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vMatrix.r[0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vMatrix.r[1]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMatrix.r[2]);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMatrix.r[3]);


		m_pTransformCom->LookAt(vTargetViewMatrix.r[3]);
	}


}

void CArona_Camera::MakeUI(void)
{
	static _uint i = 0;

	if (9 < i)
	{
		if (i == 10)
		{
			for (int Index = 0; Index < 10; ++Index)
			{
				m_pUI[Index]->Set_Size(_float3(500.f, 500.f, 1.f));
				m_pUI[Index]->Set_Pos(_float3(-550.f, -630.f, 0.f));
				m_bOpenEvent = true;
			}
			++i;

			CreateFadeWhite();
			CreateFadeBlack();
		}
		return;
	}

	if (m_bMakeUIOnce)
	{
		CreateUI(i);
		m_bMakeUIOnce = false;
	}

	if (nullptr != m_pUI[i])
	{
		if (!m_pUI[i]->Get_Thorwing())
		{
			++i;
			if (9 < i)
				return;
			CreateUI(i);
		}
	}

}

void CArona_Camera::CreateUI(_uint i)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pUI[i] = CUI_GachaCard::Create(m_pDevice, m_pContext);

	if (FAILED(m_pUI[i]->LoadUIImage(TEXT("Prototype_Component_Texture_Gacha_Card"), LEVEL_GACHA_PLAY)))
	{
		MSG_BOX("UI 갸차카드 이미지 생성 실패");
		return;
	}
	_tchar _name[MAX_PATH] = {};

	wsprintf(_name, TEXT("Gacha_Play_Random_Card_%d"), i);
	m_pUI[i]->Set_UIName(_name);
	m_pUI[i]->Set_UIType(UI_TYPE::UI_BACKGROUND);
	m_pUI[i]->Set_Size(_float3(200.f, 200.f, 1.f));
	if (5 > i)
		m_pUI[i]->Set_Pos(_float3(-440.f + (_float)i * 220.f, 150.f, 0.f));
	else
		m_pUI[i]->Set_Pos(_float3(-440.f + (_float)(i - 5) * 220.f, -150.f, 0.f));

	if (5 > i)
		m_pUI[i]->Set_ThrowPos(_float2(300.f, 300.f));
	else
		m_pUI[i]->Set_ThrowPos(_float2(-300.f, -300.f));


	m_pUI[i]->Set_UILevel(LEVEL_GACHA_PLAY);
	m_pUI[i]->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pUI[i])))	//받아온레벨에다 생성
	{
		MSG_BOX("UI 갸차카드 생성 실패");
		return;
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CArona_Camera::CardOpen()
{
	for (int i = 0; i < 10; ++i)
	{
		m_pUI[i]->Set_Pos(_float3(0.f, 0.f, 0.f));
		m_pUI[i]->Set_ThrowPos(_float2(-500.f, -300.f));
		static_cast<CUI_GachaCard*>(m_pUI[i])->Set_TickStop(true);
		static_cast<CUI_GachaCard*>(m_pUI[i])->Set_Render(false);
		m_pUI[i]->Initialization();
	}



	m_bOpenEvent = false;
}

void CArona_Camera::CreateFadeWhite()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pFadeWhite = CUI_Fade_White::Create(m_pDevice, m_pContext);

	if (FAILED(m_pFadeWhite->LoadUIImage(TEXT("Prototype_Component_Texture_FadeWhite"), LEVEL_GACHA_PLAY)))
	{
		MSG_BOX("UI 화이트 이미지 생성 실패");
		return;
	}
	_tchar _name[MAX_PATH] = {};

	m_pFadeWhite->Set_UIName(TEXT("fade_White"));
	m_pFadeWhite->Set_UIType(UI_TYPE::UI_POST);
	m_pFadeWhite->Set_Size(_float3(1280.f, 720.f, 1.f));
	m_pFadeWhite->Set_Pos(_float3(0.f, 0.f, 0.f));
	m_pFadeWhite->Set_ThrowPos(_float2(0.f, 0.f));
	m_pFadeWhite->Set_UILevel(LEVEL_GACHA_PLAY);
	m_pFadeWhite->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pFadeWhite)))	//받아온레벨에다 생성
	{
		MSG_BOX("UI화이트 생성 실패");
		return ;
	}
	RELEASE_INSTANCE(CGameInstance);

	return;
}

void CArona_Camera::CreateFadeBlack()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pFadeBlack = CUI_Fade_Black::Create(m_pDevice, m_pContext);

	if (FAILED(m_pFadeBlack->LoadUIImage(TEXT("Prototype_Component_Texture_FadeBlack"), LEVEL_GACHA_PLAY)))
	{
		MSG_BOX("UI 화이트 이미지 생성 실패");
		return;
	}
	_tchar _name[MAX_PATH] = {};

	m_pFadeBlack->Set_UIName(TEXT("fade_Black"));
	m_pFadeBlack->Set_UIType(UI_TYPE::UI_POST);
	m_pFadeBlack->Set_Size(_float3(1280.f, 720.f, 1.f));
	m_pFadeBlack->Set_Pos(_float3(0.f, 0.f, 0.f));
	m_pFadeBlack->Set_ThrowPos(_float2(0.f, 0.f));
	m_pFadeBlack->Set_UILevel(LEVEL_GACHA_PLAY);
	m_pFadeBlack->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pFadeBlack)))	//받아온레벨에다 생성
	{
		MSG_BOX("UI화이트 생성 실패");
		return;
	}
	RELEASE_INSTANCE(CGameInstance);

	return;
}

