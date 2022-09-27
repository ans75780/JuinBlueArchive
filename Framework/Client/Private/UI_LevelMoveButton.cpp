#include "stdafx.h"
#include "..\Public\UI_LevelMoveButton.h"
#include "Level_Loading.h"

#include "ImguiMgr.h"

CUI_LevelMoveButton::CUI_LevelMoveButton(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_LevelMoveButton::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_eUIType = UI_BUTTTON;
	lstrcpy(m_szUIClass, TEXT("CUI_LevelMoveButton"));


	return S_OK;
}

HRESULT CUI_LevelMoveButton::initialization()
{
	m_fOriginSize = m_fSize;

	return S_OK;
}

void CUI_LevelMoveButton::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bUIButtonDown)
	{
		m_fSize.x = m_fOriginSize.x * 0.01f * 90.f;
		m_fSize.y = m_fOriginSize.y * 0.01f * 90.f;

		POINT ptTemp = GETMOUSEPOS;

		if (nullptr == Get_MouseOveredUI(ptTemp))
			m_bUIButtonDown = false;
	}
	else
	{
		m_fSize = m_fOriginSize;
	}


}

void CUI_LevelMoveButton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_LevelMoveButton::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_LevelMoveButton::OnLButtonDown()
{
	if (m_eUIType != UI_BACKGROUND)
		m_bUIButtonDown = true;
}

void CUI_LevelMoveButton::OnLButtonUp()
{
	if (m_bUIButtonDown)
	{
		if (LEVEL_END != m_MoveLevel)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

#if _DEBUG
			CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);

			if (!pImgui->Get_LevelMoveMode())  //레벨이동모드 켜져있음 이동 하도록함
			{
				RELEASE_INSTANCE(CImguiMgr);
				RELEASE_INSTANCE(CGameInstance);
				m_bUIButtonDown = false;
				return;		//레벨이동 꺼져있으면 그냥여기서 나가짐
			}
			RELEASE_INSTANCE(CImguiMgr);
#endif

			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, (LEVEL)m_MoveLevel))))
				MSG_BOX("레벨이동버튼 실패");

			RELEASE_INSTANCE(CGameInstance);
		}

		m_bUIButtonDown = false;
	}
}

void CUI_LevelMoveButton::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);
	
	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}

void CUI_LevelMoveButton::Set_Size(_float3 _fSize)
{
	m_fOriginSize = _fSize;
	m_fSize = _fSize;
}

HRESULT CUI_LevelMoveButton::SetUp_Components()
{
	return S_OK;
}

HRESULT CUI_LevelMoveButton::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

CUI_LevelMoveButton * CUI_LevelMoveButton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_LevelMoveButton*		pInstance = new CUI_LevelMoveButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_LevelMoveButton");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_LevelMoveButton::Free()
{
	__super::Free();
}
