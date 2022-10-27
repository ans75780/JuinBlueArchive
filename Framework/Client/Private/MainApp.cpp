#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Level_Loading_Start.h"

#include "UserData.h"
#include "ImguiMgr.h"
#include "Collider.h"
#include "Student.h"
#include "Enemy.h"
#include "StrUtil.h"
#include "Json_Utility.h"

#include "Baricade.h"
#include "VIBuffer_Point.h"

#include "Start_illust.h"
#include "UI_Progress_StartLoadingBar.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	//D3D11_SAMPLER_DESC

	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.isWindowMode = GRAPHICDESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Default"), TEXT("../../Resources/Fonts/gyeonggi_1000year_title_bold.spriteFont"))))
		return E_FAIL;

#if _DEBUG
	if (FAILED(CImguiMgr::Get_Instance()->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;
#endif	//IMGUI 추가

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Static_Resource()))
		return E_FAIL;

	if (FAILED(Ready_Static_NonAnimModels()))
		return E_FAIL;


	if (FAILED(Ready_Loading_Resource()))
		return E_FAIL;

	CUserData::Get_Instance()->Initialize();

	if (FAILED(Open_Level_Start(LEVEL_LOGO)))
		return E_FAIL;

	_float4 backBuffer_Color = { 0.274f, 0.49f, 1.f, 1.f };
	m_pGameInstance->Set_BackBufferColor(backBuffer_Color);

	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_fTimeAcc += fTimeDelta;

	m_pGameInstance->Tick_Engine(fTimeDelta);

#if _DEBUG
	CImguiMgr::Get_Instance()->Tick(fTimeDelta);
#endif	//IMGUI 추가

}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance || 
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(m_pGameInstance->Get_BackBufferColor());

	m_pGameInstance->Clear_DepthStencil_View();
	
	m_pRenderer->Draw_RenderGroup();

#if _DEBUG
	CImguiMgr::Get_Instance()->Render();
#endif	//IMGUI 추가

	m_pGameInstance->Render_Engine();

#if _DEBUG
	++m_iNumRender;
	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("에프피에스 : %d"), m_iNumRender);
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}
	m_pGameInstance->Render_Font(TEXT("Font_Default"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.f);
#endif	//IMGUI 추가

	m_pGameInstance->Present();

	return S_OK;
}


HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel_Loading*		pLevel_Loading = CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID);
	if (nullptr == pLevel_Loading)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel_Loading)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Open_Level_Start(LEVEL eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel_Loading_Start*	pLevel_Loading = CLevel_Loading_Start::Create(m_pDevice, m_pContext, eLevelID);
	if (nullptr == pLevel_Loading)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING_START, pLevel_Loading)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexEffect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxTexEffect.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex_Test"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxTexTest.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxHpBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxHpBar"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxHpBar.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;




	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;
	

	///* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;


	/////////////////////////////
	/* For.Prototype_Component_Shader_ProgressToPoint*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_PointToProgress"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_PointToProgress.hlsl"), VTXPOS_DECLARATION::Element, VTXPOS_DECLARATION::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"),
		CVIBuffer_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/////////////////////////////

	
	return S_OK;
}

HRESULT CMainApp::Ready_Static_Resource()
{


	return S_OK;
}

HRESULT CMainApp::Ready_Loading_Resource()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_StartLoadingBar"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_PointToProgress_StartLoadingBar.hlsl"), VTXPOS_DECLARATION::Element, VTXPOS_DECLARATION::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING_START, TEXT("Prototype_Component_Texture_Startillust"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Start/Start_%d.png"), 16))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING_START, TEXT("Prototype_Component_Texture_Pop_logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Pop_logo.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Start_illust"), CStart_illust::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Combat_Cost_Gauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/UI/UI_extract/image/Combat/Combat_Cost_Gauge.png")))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Combat_CostBg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/UI/UI_extract/image/Combat/Combat_CostBg.png")))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Combat_Cost_Gauge_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Combat_Cost_Gauge_Back.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Portrait_Aru_Original"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/UI/CharacterCard/Skill_Portrait_Aru_Original.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Portrait_Haruka_Original"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/UI/CharacterCard/Skill_Portrait_Haruka_Original.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Portrait_Zunko_Original"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/UI/CharacterCard/Skill_Portrait_Zunko_Original.png")))))
		return E_FAIL;

	m_pGameInstance->Get_Instance()->Get_SoundManager()->LoadSoundFile();

	return S_OK;
}

HRESULT CMainApp::Ready_Static_NonAnimModels()
{

	_matrix mat, rotMat;
	mat = XMMatrixIdentity();

	rotMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));



	return S_OK;
}

HRESULT CMainApp::Ready_Static_Sounds()
{
	return E_NOTIMPL;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MessageBox(g_hWnd, TEXT("Failed to Created : CMainApp"), TEXT("System Error"), MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
#if _DEBUG
	CImguiMgr::Get_Instance()->Destroy_Instance();
#endif

	CUserData::Get_Instance()->Destroy_Instance();
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	CGameInstance::Release_Engine();	
}
