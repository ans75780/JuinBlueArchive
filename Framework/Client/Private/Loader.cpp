#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#pragma region GAMEOBJECT_TEST

#include "BackGround.h"
#include "TestObject_Test.h"
#include "UI_TestDialog.h"
#include "UI_TestDialogButton.h"
#include "TestObject_Model_Haruka.h"
#include "Student.h"
#pragma endregion

#include "MapProp.h"


//#include "Camera_Free.h"
#include "Camera_Free.h"
//#include "Monster.h"
#include "Terrain.h"
//#include "Player.h"
//#include "Effect.h"
#include "Sky.h"
CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT hr = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		hr = pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = pLoader->Loading_ForGamePlayLevel();
		break;
	case LEVEL_MAPTOOL:
		hr = pLoader->Loading_ForMapToolLevel();
		break;
	}	

	if (FAILED(hr))
		MSG_BOX("Failed to Loading");

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));
	
	/* For.Prototype_GameObject_BackGround */

	CUI* pUI = CBackGround::Create(m_pDevice, m_pContext);
	pUI->LoadUIImage(L"UI_Background");
	pUI->Set_UIType(UI_TYPE::UI_BACKGROUND);
	pUI->Set_Size(_float3(1280.f, 720.f, 1.f));
	if (FAILED(pGameInstance->Add_UI(LEVEL_LOGO, pUI)))
	{
		return E_FAIL;
	}
	
	CUI* pButton = CBackGround::Create(m_pDevice, m_pContext);
	pButton->LoadUIImage(L"UI_Default");
	pButton->Set_UIType(UI_TYPE::UI_BUTTTON);
	if (FAILED(pGameInstance->Add_UI(LEVEL_LOGO, pButton)))
	{
		return E_FAIL;
	}


	CUI_TestDialog* pDialog = CUI_TestDialog::Create(m_pDevice, m_pContext);
	pDialog->LoadUIImage(L"UI_Background");
	//pDialog->Set_UIType(UI_TYPE::UI_BUTTTON);
	if (FAILED(pGameInstance->Add_UI(LEVEL_LOGO, pDialog)))
	{
		return E_FAIL;
	}

	
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
	//	CBackGround::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

		

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));	

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Student_Serika */

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Student_Serika"),
		CStudent::Create(m_pDevice, m_pContext, TEXT("Serika")))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Monster */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
	//	CMonster::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

#pragma endregion

#pragma region BUFFER
	lstrcpy(m_szLoadingText, TEXT("버퍼를 로딩중이비낟. "));

	///* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion

#pragma region LOAD_TEXTURE

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Grass_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Mouth"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Character_Mouth.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

#pragma endregion

#pragma region LOAD_MODEL

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Height.bmp")))))
		return E_FAIL;	
	_matrix mat;
	mat = XMMatrixIdentity();
	/* For.Prototype_Component_Model_*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Serika"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Serika_Original/", "Serika_Original.fbx", mat))))
		return E_FAIL;


#pragma endregion
          
#pragma region LOAD_SHADER

	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중이빈다. "));
	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements))))
		return E_FAIL;
	
	/* For.Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;


#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoader::Loading_ForMapToolLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapProp"),
		CMapProp::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion


#pragma region LOAD_TEXTURE

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));
	//* For.Prototype_Component_Texture_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Texture_Terrain"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Grass_%d.dds"), 2))))
	//	return E_FAIL;




#pragma endregion

#pragma region LOAD_MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	/* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Height.bmp")))))
	//	return E_FAIL;


	/* For.Prototype_Component_Model_*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Test_Prototype_Component_Model_ForkLift"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Resources/Models/NonAnimModels/ForkLift/", "ForkLift.fbx"))))
	//	return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Cube*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	/* For.Prototype_Component_Model_City_Building_01*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_City_Building_01"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, 
	//		"../../Resources/Models/NonAnimModels/MapProp/city_building_01/", "sb_city_01_building_01.fbx"))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Test_Prototype_Component_Model_Haruka"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Resources/Models/NonAnimModels/ForkLift/", "ForkLift.fbx"))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_City_Building_01*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Model_City_Building_02"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM,
	//		"../../Resources/Models/NonAnimModels/MapProp/city_building_02", "sb_city_01_building_02.fbx"))))
	//	return E_FAIL;




#pragma endregion

#pragma region LOAD_SHADER
	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중이빈다. "));


	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAPTOOL, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;



#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
