#include "stdafx.h"
#include "..\Public\UI_Default.h"
#include "Level_Loading.h"

#include "ImguiMgr.h"

CUI_Default::CUI_Default(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Default::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eUIType = UI_BUTTTON;
	lstrcpy(m_szUIClass, TEXT("CUI_Default"));

	m_fRatio = { 1.f, 1.f };
	return S_OK;
}

HRESULT CUI_Default::initialization()
{
	__super::initialization();

	m_fOriginSize = m_fSize;

	return S_OK;
}

void CUI_Default::Tick(_float fTimeDelta)
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
		m_fSize = m_fOriginSize;
}

void CUI_Default::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Default::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPath);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Default::OnLButtonDown()
{
	if (m_eUIType != UI_BACKGROUND)
		m_bUIButtonDown = true;
}

void CUI_Default::OnLButtonUp()
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

void CUI_Default::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);

	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}

void CUI_Default::Set_Size(_float3 _fSize)
{
	m_fOriginSize = _fSize;
	m_fSize = _fSize;
}

void CUI_Default::SetRatioWidth(_float width)
{
	m_fRatio.x = width;
}


HRESULT CUI_Default::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_Ratio", &m_fRatio, sizeof(_float2))))
		return E_FAIL;

	return S_OK;
}

CUI_Default * CUI_Default::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Default*		pInstance = new CUI_Default(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Default");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Default::Free()
{
	__super::Free();
}
