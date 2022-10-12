#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"

#include "Level_Logo.h"
#include "Level_Lobby.h"
#include "Level_Cafe.h"
#include "Level_Schedule.h"
#include "Level_Students.h"
#include "Level_Formation.h"
#include "Level_Circle.h"
#include "Level_Manufacture.h"
#include "Level_Shop.h"
#include "Level_Gacha.h"

#include "Level_Work.h"
#include "Level_GamePlay.h"
#include "Level_MapTool.h"

#include "Level_Gacha_Play.h"
#include "GameInstance.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
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
			case LEVEL_LOBBY:
				pLevel = CLevel_Lobby::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_CAFE:
				pLevel = CLevel_Cafe::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_SCHEDULE:
				pLevel = CLevel_Schedule::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_STUDENTS:
				pLevel = CLevel_Students::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_FORMATION:
				pLevel = CLevel_Formation::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_CIRCLE:
				pLevel = CLevel_Circle::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_MANUFACTURE:
				pLevel = CLevel_Manufacture::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_SHOP:
				pLevel = CLevel_Shop::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GACHA:
				pLevel = CLevel_Gacha::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_WORK:
				pLevel = CLevel_Work::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_MAPTOOL:
				pLevel = CLevel_MapTool::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GACHA_PLAY:
				pLevel = CLevel_Gacha_Play::Create(m_pDevice, m_pContext);
			case LEVEL_END:
				break;
			default:
				break;
			}

			if (nullptr == pLevel)
				return;

			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				return;
			Safe_Release(pGameInstance);
		}
	}
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());	

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

