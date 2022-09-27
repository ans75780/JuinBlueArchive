#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UserData.h"
#include "ImguiMgr.h"
#include "Collider.h"

#include "StrUtil.h"
#include "Json_Utility.h"

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

	//if (FAILED(Ready_Static_Models()))
	//	return E_FAIL;

	CUserData::Get_Instance()->Initialize();

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

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

	++m_iNumRender;
	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("에프피에스 : %d"), m_iNumRender);
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}
	m_pGameInstance->Render_Font(TEXT("Font_Default"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.f);

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

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Default%d.jpg"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/AppIcon_Shiroko%d.png"), 1))))
		return E_FAIL;

	//Safe_AddRef(m_pRenderer);

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_TestDialogButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/DefaultButton.png"), 1))))
		return E_FAIL;
	//Safe_AddRef(m_pRenderer);


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



	return S_OK;
}

HRESULT CMainApp::Ready_Static_Resource()
{
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Default%d.jpg"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_AppIcon_Front */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AppIcon_Shiroko"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/AppIcon_Shiroko%d.png"), 1))))
		return E_FAIL;

	//LoadUITexture("Combat", m_pGameInstance);
	LoadUITexture("Common", m_pGameInstance);
	//LoadUITexture("Emoji", m_pGameInstance);
	//LoadUITexture("Floater", m_pGameInstance);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Common_Bottom_Menu_Bg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Common_Bottom_Menu_Bg%d.png"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Static_Models()
{
	_matrix mat;
	_float4x4 ScaleMatrix;
	mat = XMMatrixIdentity();


	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Serika"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Serika_Original/", "Serika_Original.fbx", mat))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Haruka"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Haruka_Original/", "Haruka_Original.fbx", mat))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::LoadUITexture(char * folderName, void * pGameInstance)
{
	string jsonPath = "../../Resources/UI/UI_original/json/";
	jsonPath += folderName;
	jsonPath += ".json";

	json	JsonUI;
	wchar_t* pUtil_jsonPath = CStrUtil::ConvertCtoWC(jsonPath.c_str());
	if (FAILED(CJson_Utility::Load_Json(pUtil_jsonPath, &JsonUI)))
	{
		string jsonFail = "제이슨";
		jsonFail += folderName;
		jsonFail += "로드 실패";
		wchar_t* pUtil_Fail = CStrUtil::ConvertCtoWC(jsonFail.c_str());

		MessageBox(0, pUtil_Fail, TEXT("System Error"), MB_OK);
		Safe_Delete_Array(pUtil_Fail);
		return E_FAIL;
	}
	Safe_Delete_Array(pUtil_jsonPath);

	auto UI_json_mSprite = JsonUI["mSprites"];
	for (auto it = UI_json_mSprite.begin(); it != UI_json_mSprite.end(); ++it)
	{
		string _name = (*it)["name"];

		string TextureName = "Prototype_Component_Texture_";
		TextureName += _name;
		TextureName += "_";
		TextureName += folderName;

		string _ImagePath = "../../Resources/UI/UI_extract/image/";
		_ImagePath += folderName;
		_ImagePath += "/";
		_ImagePath += _name;
		_ImagePath += ".png";

		CGameInstance* pGamePointer = (CGameInstance*)pGameInstance;

		if (FAILED(pGamePointer->Add_Prototype(LEVEL_STATIC
			, CStrUtil::ConvertCtoWC(TextureName.c_str())
			, CTexture::Create(m_pDevice, m_pContext
				, CStrUtil::ConvertCtoWC(_ImagePath.c_str())))))
		{
			string jsonFail = "제이슨을 통한 경로 이미지";
			jsonFail += folderName;
			jsonFail += "로드 실패";
			wchar_t* pUtil_JsonFail = CStrUtil::ConvertCtoWC(jsonFail.c_str());

			MessageBox(0, pUtil_JsonFail, TEXT("System Error"), MB_OK);
			Safe_Delete_Array(pUtil_JsonFail);
			return E_FAIL;
		}

	}

	return S_OK;
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

	//Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	CUserData::Get_Instance()->Release();
	CGameInstance::Release_Engine();	
}
