#include "stdafx.h"
#include "ImguiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "MainApp.h"

#include "StrUtil.h"

#include "Level_Loading.h"

#include "UI.h"
#include "UI_LevelMoveButton.h"

IMPLEMENT_SINGLETON(CImguiMgr)


CImguiMgr::CImguiMgr()
	: m_pGameInstance(CGameInstance::Get_Instance())
	, show_demo_window(false), show_mainBar(true)
	, MapToolCheckBox(false), m_currentLevelID(0)
	, UIToolCheckBox(true)
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImguiMgr::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);


	//ImGui::Text("cmp hi   hi =  %d", wcscmp(TEXT("hi"), TEXT("hi")));
	//ImGui::Text("cmp ComTexture_Component Texture=  %d", wcscmp(TEXT("ComTexture_Component"), TEXT("ComTexture")));
	//ImGui::Text("cmp hiru hiruungdi=  %d", wcscmp(TEXT("hiru"), TEXT("hiruungdi")));





	return S_OK;
}


void CImguiMgr::Tick(float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_F1))
		show_mainBar = true;
	if (m_pGameInstance->Get_DIKeyState(DIK_F2))
		show_mainBar = false;
}

HRESULT CImguiMgr::Render(void)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	////////////////////////
	if (show_mainBar)
	{
		HelloJusin_View();
	}
	////////////////////////

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

void CImguiMgr::HelloJusin_View(void)
{
#pragma region 임구이바 옵션
	bool no_titlebar = false;
	bool no_scrollbar = false;
	bool no_menu = true;
	bool no_move = false;
	bool no_resize = false;
	bool no_collapse = false;
	bool no_nav = false;
	bool no_background = false;
	bool no_bring_to_front = false;
	bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
#pragma endregion

	ImGui::Begin("Hello, Jusin!", &show_mainBar, window_flags);// 임구이 윈도우 생성

	char tempText_CurrentLevel[64] = "Current Level = \0";
	char tempText_Level[32] = {};

	m_currentLevelID = m_pGameInstance->Get_CurrentLevelID();

	switch (m_currentLevelID)
	{
	case LEVEL_STATIC:
		strcpy_s(tempText_Level, "LEVEL_STATIC");
		break;
	case LEVEL_LOADING:
		strcpy_s(tempText_Level, "LEVEL_LOADING");
		break;
	case LEVEL_LOGO:
		strcpy_s(tempText_Level, "LEVEL_LOGO");
		break;
	case LEVEL_GAMEPLAY:
		strcpy_s(tempText_Level, "LEVEL_GAMEPLAY");
		break;
	case LEVEL_MAPTOOL:
		strcpy_s(tempText_Level, "LEVEL_MAPTOOL");
		break;

	case LEVEL_END:
		strcpy_s(tempText_Level, "ERROR");
		break;
	}

	strcat_s(tempText_CurrentLevel, tempText_Level);

	ImGui::Text(tempText_CurrentLevel);
	
	ImGui::Separator();
	ImGui::Checkbox("UI_Tool", &UIToolCheckBox);

	if (m_currentLevelID == LEVEL_MAPTOOL)
	{
		ImGui::Separator();

		ImGui::Checkbox("MapTool", &MapToolCheckBox);

	}

	ImGui::Separator();

	if (ImGui::BeginTabBar("MainTab", ImGuiTabBarFlags_None))	//메인책갈피
	{

		if (ImGui::BeginTabItem("Main"))
		{
			HelloJusin_Tap_Main();					//HelloJusin_Tap_Main

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Object"))
		{
			HelloJusin_Tap_Object();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Level"))
		{
			HelloJusin_Tap_Level();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	if (UIToolCheckBox)
		UITool_View();

	if (MapToolCheckBox)
		MapTool_View();

	ImGui::End();
}

void CImguiMgr::HelloJusin_Tap_Main(void)			//HelloJusin_Tap_Main
{
	ImGuiIO& io = ImGui::GetIO();

	static float f = 0.0f;
	static int counter = 0;
	static float backBuffer_Color[4] = { 0.f, 0.f, 1.f, 1.f };

	ImGui::Text("BLUE ARCHIVE");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("BackBuffer_Color", backBuffer_Color);
	{
		m_pGameInstance->Set_BackBufferColor(_float4(backBuffer_Color));
	}
	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	if (ImGui::IsMousePosValid())
		ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);


}

void CImguiMgr::HelloJusin_Tap_Object(void)
{
	Tap_Object_CObj();

	ImGui::Separator();

	Tap_Object_CUI();
}

void CImguiMgr::Tap_Object_CObj(void)
{
	if (ImGui::CollapsingHeader("CGameObjects"))
	{
		map<const _tchar*, class CLayer*> pGameObject_Layer = m_pGameInstance->Get_Layer(m_currentLevelID);

		if (pGameObject_Layer.empty())
		{
			ImGui::Text("Layer is Empty ");
			return;
		}

		for (auto& iter_Layer : pGameObject_Layer)		//레이어탐색
		{
			if (ImGui::TreeNode(CStrUtil::ConvertWCtoC(iter_Layer.first)))	//레이어 이름으로 노드생성
			{
				list<CGameObject*> pGameObject_List = m_pGameInstance->Get_GameObjects(m_currentLevelID, iter_Layer.first);

				if (pGameObject_List.empty())
				{
					ImGui::Text("GameObject is Empty ");
					continue;
				}

				int count = 1;

				char tempName[MAX_PATH] = {};
				char tempNum[32] = {};

				for (auto& iter_List : pGameObject_List)	//레이어안에있는 오브젝트 순회
				{
					strcpy_s(tempName, MAX_PATH, CStrUtil::ConvertWCtoC(iter_List->Get_OBJ_DESC().sz_Name));
					_itoa_s(count, tempNum, 32, 10);
					strcat_s(tempName, MAX_PATH, tempNum);

					if (ImGui::TreeNode(tempName))
					{
						if (ImGui::TreeNode("Transform"))
						{
							auto* temp = (CTransform*)iter_List->Get_Component(TEXT("Com_Transform"));

							_float3 _Scale = temp->Get_Scaled();
							float _ScaleFloat3[3] = { _Scale.x, _Scale.y, _Scale.z };
							if (ImGui::InputFloat3("Scale", _ScaleFloat3, "%.3f", 0))
							{
								for (int plus = 0; plus < 3; ++plus)
								{
									if (0.f >= _ScaleFloat3[plus])
										_ScaleFloat3[plus] = 0.001f;
								}

								temp->Set_Scaled(_float3(_ScaleFloat3[0], _ScaleFloat3[1], _ScaleFloat3[2]));
							}


							_vector _Translation = temp->Get_State(CTransform::STATE_TRANSLATION);
							_float4 _fTrans;
							XMStoreFloat4(&_fTrans, _Translation);
							float _TransFloat4[4] = { _fTrans.x, _fTrans.y, _fTrans.z, _fTrans.w };
							if (ImGui::InputFloat3("Translation", _TransFloat4, "%.3f", 0))
							{
								for (int plus = 0; plus < 4; ++plus)
								{
									if (0.f >= _TransFloat4[plus])
										_TransFloat4[plus] = 0.001f;
								}

								_fTrans = { _TransFloat4[0], _TransFloat4[1], _TransFloat4[2], _TransFloat4[3] };

								temp->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&_fTrans));
							}


							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
					count++;
				}
				ImGui::TreePop();
			}
		}

	}
}

void CImguiMgr::Tap_Object_CUI(void)
{
	if (ImGui::CollapsingHeader("CUI"))
	{

	}
}

void CImguiMgr::HelloJusin_Tap_Level(void)
{
	if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		for (int i = 0; i < LEVEL_END; i++)
		{
			char buf[32];

			switch (i)
			{
			case LEVEL_LOGO:
				strcpy_s(buf, "LEVEL_LOGO");
				break;
			case LEVEL_GAMEPLAY:
				strcpy_s(buf, "LEVEL_GAMEPLAY");
				break;
			case LEVEL_MAPTOOL:
				strcpy_s(buf, "LEVEL_MAPTOOL");
				break;
			default:
				continue;
				break;
			}
			ImGui::TableNextColumn();
			if(ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
			{
				if (LEVEL_LOADING == m_currentLevelID || i == m_currentLevelID)
					continue;

				if (m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, (LEVEL)i)))
					return;
			}
			
		}
		ImGui::EndTable();
	}

}

void CImguiMgr::MapTool_View(void)
{
	if (ImGui::Begin("MapTool", &MapToolCheckBox, 0))
	{


		ImGui::End();
	}
}

void CImguiMgr::UITool_View(void)
{

	ImGui::Begin("UITool", &UIToolCheckBox, 0);
	ImGui::Text("Make UI");

	const char* UI_Class_Type[] = { "None", "LevelMoveButton", "DialogButton" };
	static int UI_Class_SelectNum = 0;	//이거로 생성
	const char* UI_Class_Value = UI_Class_Type[UI_Class_SelectNum];
	if (ImGui::BeginCombo("Class Type", UI_Class_Value, 0))
	{
		for (int i = 0; i < IM_ARRAYSIZE(UI_Class_Type); ++i)
		{
			const bool is_selected = (UI_Class_SelectNum == i);
			if (ImGui::Selectable(UI_Class_Type[i], is_selected))
				UI_Class_SelectNum = i;
		}
		ImGui::EndCombo();
	}


	switch (UI_Class_SelectNum)
	{
	case 1:
		ImGui::Separator();
		Define_LevelMoveButton();
		break;
	case 2:
		ImGui::Separator();
		break;
	default:
		break;
	}

	ImGui::End();
}

void CImguiMgr::Define_LevelMoveButton(void)
{

#pragma region 이미지받기
	if (m_ImageVec.empty())
	{
		map<const _tchar*, class CComponent*> _ComMap = m_pGameInstance->Get_Prototype_Component_Map(LEVEL_STATIC);

		if (!_ComMap.empty())
		{
			for (const auto& iter : _ComMap)
			{
				if (0 < wcscmp(iter.first, TEXT("Prototype_Component_Texture_")))
				{
					_tchar _name[MAX_PATH];

					lstrcpy(_name, iter.first + lstrlenW(TEXT("Prototype_Component_Texture_")));

					t_ImageVec _Imagevec;
					_Imagevec.name = CStrUtil::ConvertWCtoC(_name);
					_Imagevec.texture = (CTexture*)iter.second;

					m_ImageVec.push_back(_Imagevec);
				}
			}
		}
	}
#pragma endregion 

	static unsigned int Image_Num = 0;
	if (!m_ImageVec.empty())
	{
		const char* Image_Value = m_ImageVec[Image_Num].name;

		ImGui::Image(m_ImageVec[Image_Num].texture->Get_ResourceView(), ImVec2(100.f, 100.f), ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 0.5f));

		if (ImGui::BeginCombo("Static_Image", Image_Value, 0))
		{
			for (int i = 0; i < m_ImageVec.size(); ++i)
			{
				const bool is_selected = (Image_Num == i);
				if (ImGui::Selectable(m_ImageVec[i].name, is_selected))
					Image_Num = i;
			}

			ImGui::EndCombo();
		}
	}

	const char* Render_Type[] = { "UI_POST", "UI_DIALOG_BUTTON", "UI_DIALOG", "UI_BUTTTON", "UI_BACKGROUND", "NONE"};
	static unsigned int Render_Num = 5;
	const char* Render_Value = Render_Type[Render_Num];
	if (ImGui::BeginCombo("Render Type", Render_Value, 0))
	{
		for (int i = 0; i < IM_ARRAYSIZE(Render_Type); ++i)
		{
			const bool is_selected = (Render_Num == i);
			if (ImGui::Selectable(Render_Type[i], is_selected))
				Render_Num = i;
		}
		ImGui::EndCombo();
	}

	static _float UI_Size[2] = { 100.f, 100.f };
	static _float UI_Pos[2] = { 0.f, 0.f };

	if (ImGui::InputFloat2("Set Size", UI_Size, "%.1f", 0))
	{
	}

	if (ImGui::InputFloat2("Set Pos", UI_Pos, "%.1f", 0))
	{
	}

	if (m_currentLevelID == LEVEL::LEVEL_LOADING || Render_Num == 5)
	{
		ImGui::Text("CurrentLevel is LOADING OR RenderType is NONE");
		return;
	}

	if (ImGui::Button("Create_UI"))
	{
		string ImageName = "Prototype_Component_Texture_";
		ImageName += m_ImageVec[Image_Num].name;
		
		CUI * pUI = CUI_LevelMoveButton::Create(m_pDevice, m_pContext);
		pUI->LoadUIImage(CStrUtil::ConvertCtoWC(ImageName.c_str()));
		pUI->Set_UIType((UI_TYPE)Render_Num);
		pUI->Set_Size(_float3(UI_Size[0], UI_Size[1], UI_Size[2]));
		pUI->Set_Pos(_float3(UI_Pos[0], UI_Pos[1], UI_Pos[2]));

		if (FAILED(m_pGameInstance->Add_UI(m_currentLevelID, pUI)))
		{
			MSG_BOX("UI생성실패");
		}

		ImGui::Text("Create");
	}
}

void CImguiMgr::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}