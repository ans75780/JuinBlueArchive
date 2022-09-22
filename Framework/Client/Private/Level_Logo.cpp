#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"

#include "UI_LevelMoveButton.h"
#include "UI_Text.h"

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
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI * pUI = CUI_Text::Create(m_pDevice, m_pContext);

	pUI->LoadUIImage(TEXT("Prototype_Component_Texture_UI_Default"));
	pUI->Set_UIName(TEXT("TextUI_Cafe"));
	pUI->Set_UIType(UI_TYPE::UI_POST);
	pUI->Set_Size(_float3(50.f, 50.f, 1.f));
	pUI->Set_Pos(_float3(0.f, 0.f, 0.f));
	pUI->Set_UILevel(LEVEL_LOGO);
	static_cast<CUI_Text*>(pUI)->SetUIScale(0.5f);
	static_cast<CUI_Text*>(pUI)->SetUIText(TEXT("카페"));
	pUI->initialization();

	if (FAILED(pGameInstance->Add_UI(LEVEL_LOGO, pUI)))	//받아온레벨에다 생성
	{
		MSG_BOX("UI생성실패");
	}
	

	Safe_Release(pGameInstance);

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

