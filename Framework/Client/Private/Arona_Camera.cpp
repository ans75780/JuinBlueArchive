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
#include "UI_Gacha_Info.h"
#include "UI_UpWall.h"
#include "UI_LevelMoveButton.h"

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

	CreateFadeWhite();
	CreateFadeBlack();
	CreateUIInfo();
	CreateUpWall();

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
		MakeUI(fTimeDelta);
	
	if (m_bOpenEvent)
		CardOpen(fTimeDelta);

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
		
		if (113.f < m_pAronaAni->Get_TimeAcc())
		{
			if (1.f < m_fFOV)
				m_fFOV -= 0.1f;
		}

		m_CameraDesc.fFovy = XMConvertToRadians(m_fFOV);


		_matrix vMatrix = m_pCam->Get_Model()->Find_Bone("Camera001")->Get_CombinedMatrix();
		_matrix vTargetViewMatrix = m_pCam->Get_Model()->Find_Bone("Camera001.Target")->Get_CombinedMatrix();
		
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vMatrix.r[0]);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vMatrix.r[1]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vMatrix.r[2]);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vMatrix.r[3]);


		m_pTransformCom->LookAt(vTargetViewMatrix.r[3]);
	}


}

void CArona_Camera::MakeUI(_float& fTimeDelta)
{
	static _uint i = 0;

	if (9 < i)
	{
		if (i == 10)
		{
			if (1.2f > m_fCardDelay)
			{
				m_fCardDelay += fTimeDelta;
				return;
			}

			for (int Index = 0; Index < 10; ++Index)
			{
				m_pUI[Index]->Set_Size(_float3(500.f, 500.f, 1.f));
				m_pUI[Index]->Set_Pos(_float3(0.f, 0.f, 0.f));
				m_pUI[Index]->Set_ThrowPos(_float2(-500.f, -300.f));
				static_cast<CUI_GachaCard*>(m_pUI[Index])->Set_TickStop(true);
				static_cast<CUI_GachaCard*>(m_pUI[Index])->Set_Render(false);
				m_pUI[Index]->Initialization();
			}
			m_bOpenEvent = true;
			++i;
			static_cast<CUI_Fade_Black*>(m_pFadeBlack)->Set_AlphaValue(1.f);
		}

		return;
	}

	if (m_bMakeUIOnce2)
	{
		static_cast<CUI_Fade_White*>(m_pFadeWhite)->Set_AlphaValue(1.f);
		static_cast<CUI_Fade_White*>(m_pFadeWhite)->Set_FullWhite(true);
		m_bMakeUIOnce2 = false;
	}

	if (static_cast<CUI_Fade_White*>(m_pFadeWhite)->Get_FullWhite())
		return;

	if (m_bMakeUIOnce)
	{
		m_CameraDesc.fFovy = XMConvertToRadians(10.f);
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

void CArona_Camera::CardOpen(_float& fTimeDelta)
{
	if(10 > m_uCardIndex)
		CardOpen_Num(m_uCardIndex, fTimeDelta);
	else
	{
		if (m_bEndOnce)
		{
			for (int i = 0; i < 10; i++)
			{

				m_pUI[i]->Set_Size(_float3(200.f, 200.f, 1.f));
				if (5 > i)
					m_pUI[i]->Set_Pos(_float3(-440.f + (_float)i * 220.f, 150.f, 0.f));
				else
					m_pUI[i]->Set_Pos(_float3(-440.f + (_float)(i - 5) * 220.f, -150.f, 0.f));

				if (5 > i)
					m_pUI[i]->Set_ThrowPos(_float2(300.f, 300.f));
				else
					m_pUI[i]->Set_ThrowPos(_float2(-300.f, -300.f));

				static_cast<CUI_GachaCard*>(m_pUI[i])->Set_Render(true);
				static_cast<CUI_GachaCard*>(m_pUI[i])->Get_Transform()->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), -1.f);
				static_cast<CUI_GachaCard*>(m_pUI[i])->Set_CardNum(
					static_cast<CUI_GachaCard*>(m_pUI[i])->Get_CardNum() + 3.f
				);
			}

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			m_pLevelBackBtn = CUI_LevelMoveButton::Create(m_pDevice, m_pContext);

			if (FAILED(m_pLevelBackBtn->LoadUIImage(TEXT(""), LEVEL_GACHA_PLAY)))
			{
				MSG_BOX("UI 레벨무브 백버튼 이미지 생성 실패");
				return;
			}

			m_pLevelBackBtn->Set_UIName(TEXT("Level_Back_Btn"));
			m_pLevelBackBtn->Set_UIType(UI_TYPE::UI_POST);
			m_pLevelBackBtn->Set_Size(_float3(1280.f, 720.f, 1.f));
			m_pLevelBackBtn->Set_Pos(_float3(0.f, 0.f, 0.f));
			m_pLevelBackBtn->Set_ThrowPos(_float2(0.f, 0.f));
			m_pLevelBackBtn->Set_UILevel(LEVEL_GACHA_PLAY);
			m_pLevelBackBtn->Initialization();

			if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pLevelBackBtn)))	//받아온레벨에다 생성
			{
				MSG_BOX("UI레벨무브 백버튼 생성 실패");
				return;
			}
			RELEASE_INSTANCE(CGameInstance);
			m_bEndOnce = false;
		}


	}

}

void CArona_Camera::CardOpen_Num(_uint num, _float& fTimeDelta)
{
	if (num != m_iCardOpenChange)
	{//다른숫자들어올경우 한번실행해줌
		m_iCardOpenChange = num;
		static_cast<CUI_GachaCard*>(m_pUI[num])->Set_Render(true);
		static_cast<CUI_GachaCard*>(m_pUI[num])->Set_TickStop(false);
		m_bFadeWhite = true;
		m_bOnceInfo = true;
		m_bInfoViewOnce = true;
	}

	if (!m_pUI[num]->Get_Thorwing() && m_bFadeWhite) //받아온숫자UI의 던지기가 끝나면 Fade걸기
	{
		static_cast<CUI_Fade_White*>(m_pFadeWhite)->Set_Alpha(true);
		m_bFadeWhite = false;
	}
	//일러랑 캐릭터설명 만들어놓고 렌더랑 틱스톱해놓을꺼임  이 아래 시점에 들어오면 뙇 하고 렌더되고틱스톰풀어서 날라오고 끝나면 다시끄고 복구
	if (!m_bFadeWhite && static_cast<CUI_Fade_White*>(m_pFadeWhite)->Get_FullWhite())
	{
		if (m_bOnceInfo)
		{
			for (int i = 0; i < 3; i++)
			{
				_float tempNum = static_cast<CUI_GachaCard*>(m_pUI[num])->Get_CardNum();
				static_cast<CUI_Gacha_Info*>(m_pInfoUI[i])->Set_CharaNum(tempNum);
			}//카드에맞는 번호로바꾸고 보여주기 최종 포지션크기는 생성값
			
			m_pInfoUI[0]->Set_Size(_float3(700.f, 700.f, 0.f));
			m_pInfoUI[0]->Set_Pos(_float3(50.f, -100.f, 0.f));
			//m_pInfoUI[0]->Set_Pos(_float3(-120.f, -160.f, 0.f)); 기존포지션
			m_pInfoUI[0]->Set_ThrowPos(_float2(200.f, 0.f));
			m_pInfoUI[0]->Initialization();
			//150 -100

			static_cast<CUI_Gacha_Info*>(m_pInfoUI[0])->Set_TickStop(false);
			static_cast<CUI_Gacha_Info*>(m_pInfoUI[0])->Set_Render(true);

			static_cast<CUI_GachaCard*>(m_pUI[num])->Set_Render(false);
			static_cast<CUI_Fade_White*>(m_pFadeWhite)->Set_Alpha(false);
			m_pUpWall->Set_Pos(_float3(2000.f, -1920.f, 0.f));

			m_bOnceInfo = false;
			
			return;
		}
	}

	if (false == m_pInfoUI[0]->Get_Thorwing() && false == m_bOnceInfo) //위초기화가 되고 던지는게 끝났다면
	{
		if (m_bInfoViewOnce)
		{
			m_pInfoUI[0]->Set_Size(_float3(1024.f, 1024.f, 1.f));
			m_pInfoUI[0]->Set_Pos(_float3(-120.f, -160.f, 0.f));
			m_pInfoUI[0]->Set_ThrowPos(_float2(50.f, 0.f));
			m_pInfoUI[0]->Initialization();

			m_pInfoUI[1]->Set_ThrowPos(_float2(-500.f, 0.f));
			m_pInfoUI[2]->Set_ThrowPos(_float2(0.f , -25.f));

			m_pInfoUI[1]->Initialization();
			m_pInfoUI[2]->Initialization();
			static_cast<CUI_Gacha_Info*>(m_pInfoUI[2])->Set_AlphaValue(0.f);

			static_cast<CUI_Gacha_Info*>(m_pInfoUI[1])->Set_TickStop(false);
			static_cast<CUI_Gacha_Info*>(m_pInfoUI[1])->Set_Render(true);
			static_cast<CUI_Gacha_Info*>(m_pInfoUI[2])->Set_TickStop(false);
			static_cast<CUI_Gacha_Info*>(m_pInfoUI[2])->Set_Render(true);

			m_bInfoViewOnce = false;
		}
		else
		{
			m_fInfoViewDelay += fTimeDelta;

			if (0.1f < m_fInfoViewDelay)
			{
				if (KEY(SPACE, TAP))
				{
					static_cast<CUI_Gacha_Info*>(m_pInfoUI[2])->Set_AlphaValue(0.95f);

					static_cast<CUI_Gacha_Info*>(m_pInfoUI[0])->Set_TickStop(true);
					static_cast<CUI_Gacha_Info*>(m_pInfoUI[0])->Set_Render(false);
					static_cast<CUI_Gacha_Info*>(m_pInfoUI[1])->Set_TickStop(true);
					static_cast<CUI_Gacha_Info*>(m_pInfoUI[1])->Set_Render(false);
					static_cast<CUI_Gacha_Info*>(m_pInfoUI[2])->Set_TickStop(true);
					static_cast<CUI_Gacha_Info*>(m_pInfoUI[2])->Set_Render(false);
					m_uCardIndex++;
					m_fInfoViewDelay = 0.f;
				}
			}
		}
		
	}

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

void CArona_Camera::CreateUIInfo()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pInfoUI[0] = CUI_Gacha_Info::Create(m_pDevice, m_pContext);
	if (FAILED(m_pInfoUI[0]->LoadUIImage(TEXT("Prototype_Component_Texture_Chara"), LEVEL_GACHA_PLAY)))
	{
		MSG_BOX("UI 인포 캐릭 이미지 생성 실패");
		return;
	}

	m_pInfoUI[0]->Set_UIName(TEXT("Chara"));
	m_pInfoUI[0]->Set_Size(_float3(1024.f, 1024.f, 1.f));
	m_pInfoUI[0]->Set_Pos(_float3(-120.f, -160.f, 0.f));
	m_pInfoUI[0]->Set_UIType(UI_TYPE::UI_BUTTTON);
	m_pInfoUI[0]->Set_ThrowPos(_float2(0.f, 0.f));
	m_pInfoUI[0]->Set_UILevel(LEVEL_GACHA_PLAY);
	m_pInfoUI[0]->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pInfoUI[0])))	//받아온레벨에다 생성
	{
		MSG_BOX("UI인포 캐릭 생성 실패");
		return;
	}





	m_pInfoUI[1] = CUI_Gacha_Info::Create(m_pDevice, m_pContext);
	if (FAILED(m_pInfoUI[1]->LoadUIImage(TEXT("Prototype_Component_Texture_Title"), LEVEL_GACHA_PLAY)))
	{
		MSG_BOX("UI 이름 이미지 생성 실패");
		return;
	}

	m_pInfoUI[1]->Set_UIName(TEXT("Title"));
	m_pInfoUI[1]->Set_Size(_float3(650.f, 325.f, 1.f));
	m_pInfoUI[1]->Set_Pos(_float3(-275.f, -260.f, 0.f));
	m_pInfoUI[1]->Set_UIType(UI_TYPE::UI_DIALOG);
	m_pInfoUI[1]->Set_ThrowPos(_float2(0.f, 0.f));
	m_pInfoUI[1]->Set_UILevel(LEVEL_GACHA_PLAY);
	m_pInfoUI[1]->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pInfoUI[1])))	//받아온레벨에다 생성
	{
		MSG_BOX("UI 이름 생성 실패");
		return;
	}
	



	m_pInfoUI[2] = CUI_Gacha_Info::Create(m_pDevice, m_pContext);
	if (FAILED(m_pInfoUI[2]->LoadUIImage(TEXT("Prototype_Component_Texture_SubInfo"), LEVEL_GACHA_PLAY)))
	{
		MSG_BOX("UI 소개 이미지 생성 실패");
		return;
	}

	m_pInfoUI[2]->Set_UIName(TEXT("SubInfo"));
	m_pInfoUI[2]->Set_Size(_float3(400.f, 520.f, 1.f));
	m_pInfoUI[2]->Set_Pos(_float3(350.f, 0.f, 0.f));
	m_pInfoUI[2]->Set_UIType(UI_TYPE::UI_DIALOG);
	m_pInfoUI[2]->Set_ThrowPos(_float2(0.f, 0.f));
	m_pInfoUI[2]->Set_UILevel(LEVEL_GACHA_PLAY);
	m_pInfoUI[2]->Get_Transform()->Set_Rotate(0.1f);
	m_pInfoUI[2]->Get_Transform()->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), 0.3f);
	m_pInfoUI[2]->Initialization();


	if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pInfoUI[2])))	//받아온레벨에다 생성
	{
		MSG_BOX("UI소개 생성 실패");
		return;
	}


	RELEASE_INSTANCE(CGameInstance);

	for (int i = 0; i < 3; i++)
	{
		static_cast<CUI_Gacha_Info*>(m_pInfoUI[i])->Set_TickStop(true);
		static_cast<CUI_Gacha_Info*>(m_pInfoUI[i])->Set_Render(false);
	}
	return;
}

void CArona_Camera::CreateUpWall()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pUpWall = CUI_UpWall::Create(m_pDevice, m_pContext);

	if (FAILED(m_pUpWall->LoadUIImage(TEXT("Prototype_Component_Texture_Wall"), LEVEL_GACHA_PLAY)))
	{
		MSG_BOX("UI WALL 이미지 생성 실패");
		return;
	}

	m_pUpWall->Set_UIName(TEXT("fade_Wall"));
	m_pUpWall->Set_UIType(UI_TYPE::UI_POST);
	m_pUpWall->Set_Size(_float3(2200.f, 2200.f, 1.f));
	m_pUpWall->Set_Pos(_float3(-840.f, 920.f, 0.f));
	m_pUpWall->Set_ThrowPos(_float2(0.f, 0.f));
	m_pUpWall->Set_UILevel(LEVEL_GACHA_PLAY);
	m_pUpWall->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_GACHA_PLAY, m_pUpWall)))	//받아온레벨에다 생성
	{
		MSG_BOX("UIWALL 생성 실패");
		return;
	}
	RELEASE_INSTANCE(CGameInstance);
}

