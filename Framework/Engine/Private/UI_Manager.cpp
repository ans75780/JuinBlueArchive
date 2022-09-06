#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "UI.h"
#include "UI_Canvas.h"

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
	json root, element;
	string strOutput;
	for (auto& pCanvas : m_vecCanvas)
	{
		vector<class CUI*>* UI_vec = pCanvas->Get_UIVec();

		for (_uint i = 0; i < UI_END; i++)
		{
			for (auto& it : UI_vec[i])
			{
				char* _TextureName = Convert_WCtoC(it->Get_UITextureName());
				char* _ClassName = Convert_WCtoC(it->Get_UIClassName());
				char* _Name = Convert_WCtoC(it->Get_UIName());
				
				element["UI"]["ClassName"] = _ClassName;
				element["UI"]["Name"] = _TextureName;
				element["UI"]["TextureName"] = _Name;

				element["UI"]["Level"] = it->Get_UILevel();
				element["UI"]["Type"] = it->Get_UIType();

				element["UI"]["Size"]["x"] = it->Get_Size().x;
				element["UI"]["Size"]["y"] = it->Get_Size().y;
				element["UI"]["Size"]["z"] = it->Get_Size().z;

				element["UI"]["Pos"]["x"] = it->Get_Pos().x;
				element["UI"]["Pos"]["y"] = it->Get_Pos().y;
				element["UI"]["Pos"]["z"] = it->Get_Pos().z;
					
				delete[] _Name;
				delete[] _ClassName;
				delete[] _TextureName;

				root.push_back(element);
			}
		}

	}

	ofstream fout;
	fout.open(L"../../Resources/Data/SaveData.json");

	if (fout.is_open())
		fout << root.dump(4) << endl;
	else
		return E_FAIL;

	fout.close();

	MSG_BOX("SAVE성공");
	return S_OK;
}

char * CUI_Manager::Convert_WCtoC(const wchar_t * str)
{
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}

wchar_t * CUI_Manager::Convert_CtoWC(const char * str)
{
	wchar_t* pStr;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	pStr = new WCHAR[strSize];
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;
}


void CUI_Manager::Free()
{
	for (auto& pCanvas : m_vecCanvas)
	{
		Safe_Release(pCanvas);
	}
	m_vecCanvas.clear();
}

