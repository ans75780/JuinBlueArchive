#include "stdafx.h"
#include "..\Public\Level_Loading_Start.h"

#include "Loader_Start.h"
#include "Level_Logo.h"
#include "GameInstance.h"

#include "UI_Progress_StartLoadingBar.h"
#include "UI_Frame.h"

CLevel_Loading_Start::CLevel_Loading_Start(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading_Start::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader_Start::Create(m_pDevice, m_pContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(Ready_Layer_Start_illust()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Get_Instance()->Get_SoundManager()->PlayBGM(L"Start_bgm.ogg", 0.5f);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_Loading_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	if (true == m_pLoader->is_Finished())
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			CLevel*			pLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			default:
				MSG_BOX("에러");
				break;
			}

			if (nullptr == pLevel)
				return;

			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance)
			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				return;
			RELEASE_INSTANCE(CGameInstance);
		}
	}
}

HRESULT CLevel_Loading_Start::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());	

	return S_OK;
}

HRESULT CLevel_Loading_Start::Ready_Layer_Start_illust()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING_START, TEXT("Layer_Loading_Start_illust"), TEXT("Prototype_GameObject_Start_illust"))))
		return E_FAIL;

	CUI * pUI = CUI_Frame::Create(m_pDevice, m_pContext);

	if (FAILED(pUI->LoadUIImage(TEXT("Prototype_Component_Texture_Pop_logo"), LEVEL_LOADING_START)))
		return E_FAIL;

	pUI->Set_UIName(TEXT("Loading_Start_BlueArchive_logo"));
	pUI->Set_UIType(UI_TYPE::UI_POST);
	pUI->Set_Size(_float3(250.f, 85.f, 1.f));
	pUI->Set_Pos(_float3(-490.f, 315.f, 0.f));
	pUI->Set_ThrowPos(_float2(0.f, 0.f));
	pUI->Set_UILevel(LEVEL_LOADING_START);
	pUI->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_LOADING_START, pUI)))	//받아온레벨에다 생성
	{
		MSG_BOX("UI 블루아카이브 로고 생성 실패");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Loading_Start * CLevel_Loading_Start::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLevel_Loading_Start*		pInstance = new CLevel_Loading_Start(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading_Start::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Get_Instance()->Get_SoundManager()->StopAll();
	RELEASE_INSTANCE(CGameInstance);
}

