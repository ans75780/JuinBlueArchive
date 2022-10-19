#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"
#include "UI_Progress.h"
#include "UI_LevelMoveButton.h"
#include "UI_Text.h"
#include "UI_Default.h"
//Json 사용
#include "Json_Utility.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
	
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;	

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY))))
			return;

		Safe_Release(pGameInstance);
	}

}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("롷고레벨임. "));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);
	
	CUI_Progress*	pHpBar= CUI_Progress::Create(m_pDevice, m_pContext);

	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, pHpBar)))
	{
		MSG_BOX("UI생성실패");
	}


	CUI_Default* m_pUICostBarFront = CUI_Default::Create(m_pDevice, m_pContext);
	CUI_Default* m_pUICostBarBack = CUI_Default::Create(m_pDevice, m_pContext);
	CUI_Default* m_pUICostNumberBack = CUI_Default::Create(m_pDevice, m_pContext);
	CUI_Text* m_pUICostText = CUI_Text::Create(m_pDevice, m_pContext);



	CUI_Default*	m_pTestCard0 = CUI_Default::Create(m_pDevice, m_pContext);
	CUI_Default*	m_pTestCard1 = CUI_Default::Create(m_pDevice, m_pContext);
	CUI_Default*	m_pTestCard2 = CUI_Default::Create(m_pDevice, m_pContext);




	m_pUICostBarFront->Set_Pos(_float3(430.f, -320.f, 0.f));
	m_pUICostBarFront->Set_Size(_float3(368.f, 25.f, 1.f));

	m_pUICostBarBack->Set_Pos(_float3(430.f, -320.f, 0.f));
	m_pUICostBarBack->Set_Size(_float3(368.f, 25.f, 1.f));

	m_pUICostNumberBack->Set_Pos(_float3(200.f, -300.f, 0.f));
	m_pUICostNumberBack->Set_Size(_float3(80.f, 86.f, 1.f));

	m_pTestCard0->Set_Pos(_float3(310.f, -240.f, 0.f));
	m_pTestCard0->Set_Size(_float3(123.f, 101.f, 1.f));
	m_pTestCard0->Set_UIType(UI_TYPE::UI_BUTTTON);

	m_pTestCard1->Set_Pos(_float3(426.f, -240.f, 0.f));
	m_pTestCard1->Set_Size(_float3(123.f, 101.f, 1.f));
	m_pTestCard1->Set_UIType(UI_TYPE::UI_BUTTTON);


	m_pTestCard2->Set_Pos(_float3(539.f, -240.f, 0.f));
	m_pTestCard2->Set_Size(_float3(123.f, 101.f, 1.f));
	m_pTestCard2->Set_UIType(UI_TYPE::UI_BUTTTON);



	m_pUICostText->Set_Pos(_float3(185.f, -290.f, 0.f));

	m_pUICostText->SetUITextColor(_float4(1.f, 1.f, 1.f, 1.f));
	m_pUICostText->SetUIScale(1.f);
	m_pUICostText->SetUIText(L"0");

	m_pUICostBarFront->Set_UIType(UI_TYPE::UI_BUTTTON);
	m_pUICostBarBack->Set_UIType(UI_TYPE::UI_BACKGROUND);
	m_pUICostBarFront->SetPath(1);

	m_pUICostNumberBack->Set_UIType(UI_TYPE::UI_BACKGROUND);
	m_pUICostText->Set_UIType(UI_TYPE::UI_BUTTTON);
	m_pUICostText->initialization();

	if (FAILED(m_pUICostBarFront->LoadUIImage(L"Prototype_Component_Texture_Combat_Cost_Gauge")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, m_pUICostBarFront, nullptr)))
		return E_FAIL;

	if (FAILED(m_pUICostBarBack->LoadUIImage(L"Prototype_Component_Texture_Combat_Cost_Gauge_Back")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, m_pUICostBarBack, nullptr)))
		return E_FAIL;

	if (FAILED(m_pUICostNumberBack->LoadUIImage(L"Prototype_Component_Texture_Combat_CostBg")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, m_pUICostNumberBack, nullptr)))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, m_pUICostText, nullptr)))
		return E_FAIL;

	if (FAILED(m_pTestCard0->LoadUIImage(L"Prototype_Component_Texture_Skill_Portrait_Aru_Original")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, m_pTestCard0, nullptr)))
		return E_FAIL;

	if (FAILED(m_pTestCard1->LoadUIImage(L"Prototype_Component_Texture_Skill_Portrait_Aru_Original")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, m_pTestCard1, nullptr)))
		return E_FAIL;

	if (FAILED(m_pTestCard2->LoadUIImage(L"Prototype_Component_Texture_Skill_Portrait_Aru_Original")))
		return E_FAIL;
	if (FAILED(pInstance->Add_UI(LEVEL_LOGO, m_pTestCard2, nullptr)))
		return E_FAIL;



	Safe_Release(pInstance);

	return S_OK;
}





HRESULT CLevel_Logo::UI_Extract()
{
	Mat img = imread("../../Resources/UI/UI_original/image/ 자를 이미지 .png", IMREAD_UNCHANGED);

	if (img.empty())
	{
		return E_FAIL;
	}

	json	JsonTemp;
	
	if (FAILED(CJson_Utility::Load_Json(CJson_Utility::Complete_Path(
		L"../../Resources/UI/UI_original/json/제이슨파일.json 빼고넣으셈  ").c_str(), &JsonTemp)))
	{
		MSG_BOX("제이슨 실패ㅜㅜ");
		return E_FAIL;
	}

	auto json_mSprites = JsonTemp["mSprites"];

	for (auto it = json_mSprites.begin(); it != json_mSprites.end(); ++it)
	{
		string _name = (*it)["name"];
		int _x		= (*it)["x"];
		int _width = (*it)["width"];
		int _y		= (*it)["y"];
		int _height= (*it)["height"];

		Mat cropped_image = img(Range(_y, (_y + _height)), Range(_x, (_x + _width)));

		string savePath = "../../Resources/UI/UI_extract/image/저장할 파일경로/";
		
		savePath += _name;
		savePath += ".png";

		imwrite(savePath.c_str(), cropped_image);
	}
	
	return S_OK;
}


CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();



}

