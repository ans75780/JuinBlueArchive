#include "stdafx.h"
#include "..\Public\Loader_Start.h"
#include "GameInstance.h"
#include "Student.h"
#include "Actor.h"
#include "Enemy.h"
#include "StrUtil.h"
#include "Json_Utility.h"
#include "Baricade.h"
#include "HpBar.h"
#include "Student_Fx.h"
#include "Camera_Free.h"
#include "Stage.h"
#include "Effect_Hit.h"

#include "Memorial_Haruka_Start.h"
#include "Memorial_Haruka_Idle.h"
#include "UI_Progress_StartLoadingBar.h"
#include "UI_Text.h"

CLoader_Start::CLoader_Start(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY LoadingMain_Start(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader_Start*		pLoader = (CLoader_Start*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT hr = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		hr = pLoader->Loading_ForLogoLevel();
		break;
	}

	if (FAILED(hr))
		MSG_BOX("Failed to Loading");

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader_Start::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain_Start, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;



	return S_OK;
}

HRESULT CLoader_Start::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("��ü�� �������Դϴ�."));
	
	CUI_Progress_StartLoadingBar* pUI_ProgreeBar = CUI_Progress_StartLoadingBar::Create(m_pDevice, m_pContext);
	pUI_ProgreeBar->Set_UILevel(LEVEL_LOADING_START);
	if (FAILED(pGameInstance->Add_UI(LEVEL_LOADING_START, pUI_ProgreeBar)))
	{
		MSG_BOX("���зε���");
		return E_FAIL;
	}

	CUI_Text* pUI_Text = CUI_Text::Create(m_pDevice, m_pContext);
	pUI_Text->Set_UIType(UI_TYPE::UI_POST);
	pUI_Text->Set_UIName(TEXT("UI_ProgreeBarLoadingText"));
	pUI_Text->Set_Size(_float3(50.f, 50.f, 0.f));
	pUI_Text->Set_Pos(_float3(-100.f, -300.f, 1.f));
	pUI_Text->SetUIScale(0.5f);
	pUI_Text->SetUITextColor(_float4(1.f, 1.f, 1.f, 1.f));
	pUI_Text->Set_ThrowPos(_float2(0.f, 0.f));
	pUI_Text->SetUIText(TEXT("��ü�� �������Դϴ�."));
	pUI_Text->Set_UILevel(LEVEL_LOADING_START);
	pUI_Text->Initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_LOADING_START, pUI_Text)))
	{
		MSG_BOX("����ε��ؽ�Ʈ");
		return E_FAIL;
	}

	pUI_ProgreeBar->Plus_WidthSize(100.f);	//10%
	pUI_Text->SetUIText(TEXT("�ؽ��ĸ� �ҷ��������Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����̺�. "));


	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FX_Hit_0"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Effects/Texture/FX_Hit_0/FX_TEX_HIT_0_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Default%d.jpg"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_AppIcon_Front */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AppIcon_Shiroko"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/AppIcon_Shiroko%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Default%d.jpg"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/AppIcon_Shiroko%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_TestDialogButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/DefaultButton.png"), 1))))
		return E_FAIL;

	//LoadUITexture("Combat");
	if (FAILED(LoadUITexture("Common")))
		return E_FAIL;
	//LoadUITexture("Emoji");
	//LoadUITexture("Floater");

	if (FAILED(LoadBg()))
		return E_FAIL;

	pUI_ProgreeBar->Plus_WidthSize(100.f); //20%
	pUI_Text->SetUIText(TEXT("�޸𸮾��� �ҷ��������Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("�޸𸮾��� �ε����̺�. "));


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Memorial_Haruka_Start"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Memorial/Haruka/Start/Haruka_home_D_35_%d.png"), 515))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Memorial_Haruka_Idle"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Memorial/Haruka/Idle/Haruka_home_I_35_%d.png"), 286))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Memorial_Haruka */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Memorial_Haruka_Start"),
	//	CMemorial_Haruka_Start::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	///* For.Prototype_GameObject_Memorial_Haruka */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Memorial_Haruka_Idle"),
	//	CMemorial_Haruka_Idle::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	pUI_ProgreeBar->Plus_WidthSize(200.f); //40%
	pUI_Text->SetUIText(TEXT("������Ʈ�� �������Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������Ʈ�� �ε����Դϴ�. "));

	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_Student_FX", CStudent_FX::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_Student", CStudent::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_Enemy", CEnemy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_Baricade", CBaricade::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_HpBar", CHpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	pUI_ProgreeBar->Plus_WidthSize(100.f); //50%
	pUI_Text->SetUIText(TEXT("���� �ҷ��������Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("���� �ε����̺�. "));

	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_Effect_Hit", CEffect_Hit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix mat, rotMat;
	mat = XMMatrixIdentity();

	rotMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(180.f));


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_FX_MESH_Circle"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Resources/Effects/Mesh/FX_MESH_Circle/", "FX_MESH_Circle.fbx", mat))))
	//	return E_FAIL;


	/* For.Prototype_Component_Model_Stage_School*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stage_School_1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Resources/Models/NonAnimModels/Stages/School/", "School_1.fbx",
			rotMat))))
		return E_FAIL;
	///* For.Prototype_Component_Model_Serika*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Aru_Original"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Aru_Original/", "Aru_Original.fbx", mat))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Serika*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Haruka_Original"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Haruka_Original/", "Haruka_Original.fbx", mat))))
	//	return E_FAIL;

	pUI_ProgreeBar->Plus_WidthSize(200.f); //70%

	///* For.Prototype_Component_Model_Serika*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Zunko_Original"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Resources/Models/AnimModels/Zunko_Original/", "Zunko_Original.fbx", mat))))
	//	return E_FAIL;


	/* For.Prototype_GameObject_Camera_Event*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUI_ProgreeBar->Plus_WidthSize(100.f); //80%
	pUI_Text->SetUIText(TEXT("���� �ҷ��������Դϴ�."));


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stage_School"),
		CStage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Stage_School*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stage_School_1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Resources/Models/NonAnimModels/Stages/School/", "School_1.fbx", mat))))
	//	return E_FAIL;


	pUI_ProgreeBar->Plus_WidthSize(200.f); //100%
	pUI_ProgreeBar->Set_LoadingSusccess(true);
	pUI_Text->SetUIText(TEXT("ENTER TO START"));

	lstrcpy(m_szLoadingText, TEXT("�ε� �� "));


	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader_Start::LoadUITexture(char * folderName)
{
	string jsonPath = "../../Resources/UI/UI_original/json/";
	jsonPath += folderName;
	jsonPath += ".json";

	json	JsonUI;
	wchar_t* pUtil_jsonPath = CStrUtil::ConvertCtoWC(jsonPath.c_str());
	if (FAILED(CJson_Utility::Load_Json(pUtil_jsonPath, &JsonUI)))
	{
		string jsonFail = "���̽�";
		jsonFail += folderName;
		jsonFail += "�ε� ����";
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

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC
			, CStrUtil::ConvertCtoWC(TextureName.c_str())
			, CTexture::Create(m_pDevice, m_pContext
			, CStrUtil::ConvertCtoWC(_ImagePath.c_str())))))
		{
			string jsonFail = "���̽��� ���� ��� �̹���";
			jsonFail += folderName;
			jsonFail += "�ε� ����";
			wchar_t* pUtil_JsonFail = CStrUtil::ConvertCtoWC(jsonFail.c_str());

			MessageBox(0, pUtil_JsonFail, TEXT("System Error"), MB_OK);
			Safe_Delete_Array(pUtil_JsonFail);
			return E_FAIL;
		}

		RELEASE_INSTANCE(CGameInstance);

	}

	return S_OK;
}

HRESULT CLoader_Start::LoadBg()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Common_Bottom_Menu_Bg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Common_Bottom_Menu_Bg%d.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Common_Top_Menu_Bg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Common_Top_Menu_Bg%d.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gacha_BtnBg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Gacha/Gacha_BtnBg.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Black_Bg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Loading/Black_Bg.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Dot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Loading/dot_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Default/Loading/Loading_Image/LoadingImage_%d.png"), 28))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLoader_Start * CLoader_Start::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader_Start*		pInstance = new CLoader_Start(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Start::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
