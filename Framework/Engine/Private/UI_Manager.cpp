#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "UI.h"
#include "UI_Canvas.h"

#include "../../Client/Public/UI_LevelMoveButton.h"
#include "../../Client/Public/UI_Text.h"
#include "../../Client/Public/UI_Frame.h"
#include "StrUtil.h"

IMPLEMENT_SINGLETON(CUI_Manager);

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Setup_Manager(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const GRAPHICDESC & GraphicDesc, _uint iNumLevels)
{
	m_iEditLevel = 0;
	m_bLevelEditMode = false;

	m_iNumLevels = iNumLevels;

	m_vecCanvas.reserve(iNumLevels);

	XMStoreFloat4x4(&CUI::g_UIMatProj, XMMatrixTranspose(XMMatrixOrthographicLH((_float)GraphicDesc.iWinCX, (_float)GraphicDesc.iWinCY, 0.f, 3.f)));

	//Reserved_Container에서 레벨 개수만큼 할당한 공간 개수 만큼 캔버스 생성
	for (_uint i = 0; i < m_vecCanvas.capacity(); i++)
	{
		CUI_Canvas* pCanvas = CUI_Canvas::Create(pDevice, pContext);
		m_vecCanvas.push_back(pCanvas);
	}
	return S_OK;
}


HRESULT CUI_Manager::Add_UI(_uint iLevelIndex, CUI * pUI,void * pArg)
{
	m_vecCanvas[iLevelIndex]->Add_UI(pUI);
	return S_OK;
}

void CUI_Manager::Tick(_float fTimeDelta)
{
	if (m_bDisableUI)
		return;

	CGameInstance* instance = GET_INSTANCE(CGameInstance);

#if _DEBUG
	if (m_bLevelEditMode)
	{
		m_vecCanvas[m_iEditLevel]->Tick(fTimeDelta);

		RELEASE_INSTANCE(CGameInstance);
		return;
	}
#endif

	_uint	iLevel = instance->Get_CurrentLevelID();

	m_vecCanvas[iLevel]->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::LateTick(_float fTimeDelta)
{
	if (m_bDisableUI)
		return;

	CGameInstance* instance = GET_INSTANCE(CGameInstance);

#if _DEBUG
	if (m_bLevelEditMode)
	{
		m_vecCanvas[m_iEditLevel]->LateTick(fTimeDelta);

		RELEASE_INSTANCE(CGameInstance);
		return;
	}
#endif

	_uint	iLevel = instance->Get_CurrentLevelID();

	m_vecCanvas[iLevel]->LateTick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::DisableCanvas(_uint iLevelIndex)
{
	
}

vector<class CUI_Canvas*> CUI_Manager::Get_Canvases()
{
	if (m_vecCanvas.empty())
		return vector<class CUI_Canvas*>();

	return m_vecCanvas;
}

HRESULT CUI_Manager::Save_UIVec()
{
	json root;

	int cnt = 0;

	for (auto& pCanvas : m_vecCanvas)
	{
		if (cnt == LEVEL_GACHA_PLAY)
			continue;

		vector<class CUI*>* UI_vec = pCanvas->Get_UIVec();

		for (_uint i = 0; i < UI_END; i++)
		{
			for (auto& it : UI_vec[i])
			{
				json element;

				char* _TextureName = CStrUtil::ConvertWCtoC(it->Get_UITextureName());
				char* _ClassName = CStrUtil::ConvertWCtoC(it->Get_UIClassName());
				char* _Name = CStrUtil::ConvertWCtoC(it->Get_UIName());
				
				element["TextureName"] = _TextureName;
				element["ClassName"] = _ClassName;
				element["Name"] = _Name;

				element["Level"] = it->Get_UILevel();
				element["Type"] = it->Get_UIType();

				element["Size_x"] = it->Get_Size().x;
				element["Size_y"] = it->Get_Size().y;
				element["Size_z"] = it->Get_Size().z;

				element["Pos_x"] = it->Get_Pos().x;
				element["Pos_y"] = it->Get_Pos().y;
				element["Pos_z"] = it->Get_Pos().z;
				
				element["ThrowPos_x"] = it->Get_ThrowPos().x;
				element["ThrowPos_y"] = it->Get_ThrowPos().y;
				
				if (!strcmp(_ClassName, "CUI_LevelMoveButton"))
				{
					element["MoveLevel"] = static_cast<CUI_LevelMoveButton*>(it)->GetMoveLevel();
				}
				else if (!strcmp(_ClassName, "CUI_Text"))
				{
					CUI_Text* _pUIText = static_cast<CUI_Text*>(it);

					char* UIText = CStrUtil::ConvertWCtoUTF8(_pUIText->GetUIText());
					element["UIText"] = UIText;
					Safe_Delete_Array(UIText);

					element["UITextScale"] = _pUIText->GetUIScale();

					element["UITextColor_x"] = _pUIText->GetUITextColor().x;
					element["UITextColor_y"] = _pUIText->GetUITextColor().y;
					element["UITextColor_z"] = _pUIText->GetUITextColor().z;
					element["UITextColor_w"] = _pUIText->GetUITextColor().w;
				}
				else if (!strcmp(_ClassName, "CUI_Frame"))
				{
					element["MaxFrame"] = static_cast<CUI_Frame*>(it)->Get_MaxFrame();
					element["Delay"] = static_cast<CUI_Frame*>(it)->Get_Delay();
				}


				Safe_Delete_Array(_Name);
				Safe_Delete_Array(_ClassName);
				Safe_Delete_Array(_TextureName);


				root["UI"].push_back(element);
			}
		}
		++cnt;
	}

	ofstream fout;
	fout.open(L"../../Resources/Data/SaveData.json");

	if (fout.is_open())
		fout << root.dump(2) << endl;
	else
		return E_FAIL;

	fout.close();

	MSG_BOX("SAVE성공");
	return S_OK;
}

void CUI_Manager::Clear_UIVec()
{
	for (auto& pCanvas : m_vecCanvas)
		pCanvas->Clear_UIVec();
}

void CUI_Manager::Clear_Canvas(_uint iLevelIndex)
{
	m_vecCanvas[iLevelIndex]->Clear_UIVec();
}

void CUI_Manager::AllUI_Initialization()
{
	for (auto& pCanvas : m_vecCanvas)
	{
		pCanvas->AllUICanvas_Initialization();
	}
}

void CUI_Manager::Free()
{
	for (auto& pCanvas : m_vecCanvas)
	{
		Safe_Release(pCanvas);
	}
	m_vecCanvas.clear();
}

