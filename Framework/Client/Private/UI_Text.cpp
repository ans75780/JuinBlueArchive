#include "stdafx.h"
#include "..\Public\UI_Text.h"
#include "Level_Loading.h"

#include "ImguiMgr.h"

#include "UserData.h"

CUI_Text::CUI_Text(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Text::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	lstrcpy(m_szUIClass, TEXT("CUI_Text"));
	ZeroMemory(m_szText, MAX_PATH);
	lstrcpy(m_szText, TEXT("디폴트"));

	m_fColor = _float4(0.2f, 0.27f, 0.36f, 1.f);
	m_fScale = 0.6f;

	return S_OK;
}

HRESULT CUI_Text::initialization()
{
	__super::initialization();

	if (0 == lstrcmp(m_szUIName, TEXT("Diamond_Text"))) //유저데이터 에서 가지고있는돈가져오기
	{
		_tchar temp[10] = {};

		_itow_s(CUserData::Get_Instance()->Get_UserDesc().uDiamond, temp, 10);

		lstrcpy(m_szText, temp);
	}

	return S_OK;
}

void CUI_Text::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Text::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Text::Render()
{
	//if (nullptr == m_pShaderCom ||
	//	nullptr == m_pVIBufferCom)
	//	return E_FAIL;

	///* 셰이더 전역변수에 값을 던진다. */
	//if (FAILED(SetUp_ShaderResource()))
	//	return E_FAIL;

	//m_pShaderCom->Begin(0);
	//m_pVIBufferCom->Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float2 Pos = { (float)(g_iWinCX * 0.5f), (float)(g_iWinCY * 0.5f) };
	Pos.x += m_fPos.x;
	Pos.y -= m_fPos.y;
	
	pGameInstance->Render_Font(TEXT("Font_Default"), m_szText, Pos, XMLoadFloat4(&m_fColor), m_fScale);
	//m_pGameInstance->Render_Font(TEXT("Font_Default"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.f);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CUI_Text::OnLButtonDown()
{
}

void CUI_Text::OnLButtonUp()
{

}

void CUI_Text::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);
	
	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}

HRESULT CUI_Text::SetUp_ShaderResource()
{
	if (FAILED(__super::SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

CUI_Text * CUI_Text::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Text*		pInstance = new CUI_Text(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Text");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Text::Free()
{
	__super::Free();
}
