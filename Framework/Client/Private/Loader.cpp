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
#include "Camera_Event.h"
#include "UI_LevelMoveButton.h"

#pragma endregion
#include "Camera_Formation.h"

#include "StrUtil.h"
#include "Json_Utility.h"

#include "MapProp.h"


//#include "Camera_Free.h"
#include "Camera_Free.h"
//#include "Monster.h"
#include "Terrain.h"
//#include "Player.h"
//#include "Effect.h"
#include "Sky.h"
#include "Camera_Stage.h"
#include "Stage.h"
#include "Enemy.h"

#include "BG_Gacha.h"
#include "BG_Gacha_Video.h"

#include "Arona.h"
#include "Arona_Camera.h"
#include "Arona_GachaCam.h"
#include "Gacha_Sky.h"

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
	case LEVEL_LOBBY:
		hr = pLoader->Loading_ForLobbyLevel();
		break;
	case LEVEL_CAFE:
		hr = pLoader->Loading_ForCafeLevel();
		break;
	case LEVEL_SCHEDULE:
		hr = pLoader->Loading_ForScheduleLevel();
		break;
	case LEVEL_STUDENTS:
		hr = pLoader->Loading_ForStudentsLevel();
		break;
	case LEVEL_FORMATION:
		hr = pLoader->Loading_ForFormationLevel();
		break;
	case LEVEL_CIRCLE:
		hr = pLoader->Loading_ForCircleLevel();
		break;
	case LEVEL_MANUFACTURE:
		hr = pLoader->Loading_ForManufactureLevel();
		break;
	case LEVEL_SHOP:
		hr = pLoader->Loading_ForShopLevel();
		break;
	case LEVEL_GACHA:
		hr = pLoader->Loading_ForGachaLevel();
		break;
	case LEVEL_WORK:
		hr = pLoader->Loading_ForWorkLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = pLoader->Loading_ForGamePlayLevel();
		break;
	case LEVEL_MAPTOOL:
		hr = pLoader->Loading_ForMapToolLevel();
		break;
	case LEVEL_GACHA_PLAY:
		hr = pLoader->Loading_ForGachaPlayLevel();
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

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));	

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForLobbyLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForCafeLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForScheduleLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForStudentsLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForFormationLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("로딩중..햄버거..먹고싶다.."));

	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Formation"),
		CCamera_Formation::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FORMATION, TEXT("Prototype_Component_Texture_Formaiton_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/SkyBox/Background.dds"), 1))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	_matrix mat, rotMat;
	mat = XMMatrixIdentity();

	rotMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));


	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aru_Original"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Aru_Original/", "Aru_Original.fbx", rotMat))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aru_Original_FX"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Effects/ExModel/Aru_Original/", "Aru_Original.fbx", mat))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Haruka_Original"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Haruka_Original/", "Haruka_Original.fbx", rotMat))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Haruka_Original_FX"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Effects/ExModel/Haruka_Original/", "Haruka_Original.fbx", mat))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Zunko_Original"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Zunko_Original/", "Zunko_Original.fbx", rotMat))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Serika*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Zunko_Original_FX"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Effects/ExModel/Zunko_Original/", "Zunko_Original.fbx", mat))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Soldier_Kaiserpmc_HG*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Soldier_Kaiserpmc_HG"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Enemies/Soldier_Kaiserpmc_HG/", "Soldier_Kaiserpmc_HG.fbx",
			XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f))))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Stage_School*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_School_Baricade"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Resources/Models/NonAnimModels/Obstacles/School_Baricade/", "School_Baricade.fbx", XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(90.f))))))
		return E_FAIL;


	m_isFinished = true;

	lstrcpy(m_szLoadingText, TEXT("로딩 끝."));


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForCircleLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForManufactureLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForShopLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGachaLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA, TEXT("Prototype_Component_Texture_Gacha_BG_Blur"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Gacha/Gacha_BGtemp.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA, TEXT("Prototype_Component_Texture_Gacha_Pv"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Gacha_Banner/GachaBanner_%d.jpg"), 533))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gacha_BG"),
		CBG_Gacha::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gacha_BG_Video"),
		CBG_Gacha_Video::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForWorkLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

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
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Stage"),
		CCamera_Stage::Create(m_pDevice, m_pContext))))
		return E_FAIL;		//왠진모르겠지만 얘지우면 레퍼런스카운트 잘받아짐

	/* For.Prototype_GameObject_Camera_Event*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Event"),
		CCamera_Event::Create(m_pDevice, m_pContext))))
		return E_FAIL;		//왠진모르겠지만 얘지우면 레퍼런스카운트 잘받아짐

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


#pragma endregion
          
#pragma region LOAD_SHADER

	

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

HRESULT CLoader::Loading_ForGachaPlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중이비낟. "));

		/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_Gacha_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/SkyBox/Background.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_Gacha_Card"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/Card_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_FadeWhite"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/1280_720_White.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_FadeBlack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/1280_720_Black.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_Chara"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/chara_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_Title"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/Title_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_SubInfo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/SubInfo_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_Wall"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/wall.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Texture_Btn_Check"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Gacha/Btn_Check_.png"), 1))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중이비낟. "));

	_matrix mat;
	mat = XMMatrixIdentity();
	mat = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(90.0f));

	/* For.Prototype_Component_Model_Arona*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Model_Arona_Gacha"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Arona/", "Arona.fbx", mat))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Model_Arona_Gacha_Cam"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Arona/", "CAM.fbx", mat))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Arona"),
		CArona::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Arona_Gacha_Cam"),
		CArona_GachaCam::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Arona_Camera"),
		CArona_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gacha_Sky"),
		CGacha_Sky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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
