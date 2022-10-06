#include "stdafx.h"
#include "ImguiMgr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "MainApp.h"

#include "Level_Loading.h"

#include "UI.h"
#include "UI_Canvas.h"
#include "UI_LevelMoveButton.h"
#include "UI_Text.h"
#include "BackGround.h"

#include "Json_Utility.h"
#include "StrUtil.h"

IMPLEMENT_SINGLETON(CImguiMgr)


CImguiMgr::CImguiMgr()
	: m_pGameInstance(CGameInstance::Get_Instance())
	, show_demo_window(false), show_mainBar(true)
	, MapToolCheckBox(false), m_currentLevelID(2)
	, UIToolCheckBox(true), m_pSelectUI(nullptr)
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
	
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
	
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	const char* LevelMoveComboSizeCheck[] = { SELECT_LEVEL , "LEVEL_END" };
	m_uSelectUILevelMoveNum = IM_ARRAYSIZE(LevelMoveComboSizeCheck) - 1; //0포함되서 -1해줌

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

void CImguiMgr::HelloJusin_View(void)	//메인탭임 책갈피로 현재 메인탭 오브젝트탭 레밸이동탭 으로 구성되있음
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

	GetLevelString(tempText_Level, 32);

	strcat_s(tempText_CurrentLevel, tempText_Level);

	ImGui::Text(tempText_CurrentLevel);
	
	if (m_currentLevelID == LEVEL_MAPTOOL)
	{
		ImGui::Separator();
		ImGui::Checkbox("MapTool", &MapToolCheckBox);
		ImGui::Checkbox("UI_Tool", &UIToolCheckBox);

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

void CImguiMgr::HelloJusin_Tap_Main(void)			//메인탭의 1번째 책갈피임
{
	ImGuiIO& io = ImGui::GetIO();

	static float f = 0.0f;
	static int counter = 0;
	static float backBuffer_Color[4] = { 0.274f, 0.49f, 1.f, 1.f };
	//70 125 255
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

void CImguiMgr::HelloJusin_Tap_Object(void)	//Main탭의  OBJ리스트와, UI 리스트 나열
{
	Tap_Object_CObj();

	ImGui::Separator();

	Tap_Object_CUI();
}

void CImguiMgr::Tap_Object_CObj(void)	//현재레벨에 존재하는 오브젝트들 나열
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
			char* pUtil_LayerNode = CStrUtil::ConvertWCtoC(iter_Layer.first);

			if (ImGui::TreeNode(pUtil_LayerNode))	//레이어 이름으로 노드생성
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
					char* pUtil_Name = CStrUtil::ConvertWCtoC(iter_List->Get_OBJ_DESC().sz_Name);

					strcpy_s(tempName, MAX_PATH, pUtil_Name);
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
					Safe_Delete_Array(pUtil_Name);
				}
				ImGui::TreePop();
			}
			Safe_Delete_Array(pUtil_LayerNode);
		}

	}
}

void CImguiMgr::Tap_Object_CUI(void)		//UI 오브젝트리스트 나열		
{
	if (ImGui::CollapsingHeader("CUI"))
	{
		vector<CUI_Canvas*> pCanvas = m_pGameInstance->Get_Canvases();

		if (pCanvas.empty())
		{
			ImGui::Text("UI_Canvas is empty");
			return;
		}

		_uint UILevelCount = 0;

		for (auto& iter_Canvas : pCanvas)
		{
			char UICanvasTreeName[32] = {};

			if (UILevelCount == LEVEL_STATIC || UILevelCount == LEVEL_LOADING || UILevelCount == LEVEL_END)
			{
				++UILevelCount;
				continue;
			}

			GetLevelString(UICanvasTreeName, 32, UILevelCount);  //레벨문자열넣기

			if (ImGui::TreeNode(UICanvasTreeName))
			{
				vector<CUI*>* UIVec = iter_Canvas->Get_UIVec();

				for (_uint i = 0; i < UI_END; i++)
				{
					for (auto& iter_UIVec : UIVec[i])
					{
						char* pUtil_UIName = CStrUtil::ConvertWCtoC(iter_UIVec->Get_UIName());
						if (ImGui::TreeNode(pUtil_UIName))
						{
							ImGui::TreePop();
						}
						Safe_Delete_Array(pUtil_UIName);
					}
				}
				ImGui::TreePop();
			}
			++UILevelCount;
		}
		

	}
}

void CImguiMgr::HelloJusin_Tap_Level(void)	//레벨탭으로 레벨이동할수있게 해두었음
{
	if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		for (int i = 0; i < LEVEL_END; i++)
		{
			char buf[32];

			if (i == LEVEL_STATIC || i == LEVEL_LOADING || i == LEVEL_LOADING_START ||  i == LEVEL_END)
				continue;

			GetLevelString(buf, 32, i);

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

void CImguiMgr::MapTool_View(void)		//맵툴  새창을띄움	
{
	if (ImGui::Begin("MapTool", &MapToolCheckBox, 0))
	{


		ImGui::End();
	}
}

void CImguiMgr::UITool_View(void)	//UI툴  새창을 띄움
{
	ImGui::Begin("UITool", &UIToolCheckBox, 0);

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
					ZeroMemory(&_Imagevec, sizeof(t_ImageVec));
					char* pUtil_name = CStrUtil::ConvertWCtoC(_name);

					strcpy_s(_Imagevec.name, MAX_PATH, pUtil_name);
					_Imagevec.texture = (CTexture*)iter.second;

					m_ImageVec.push_back(_Imagevec);

					Safe_Delete_Array(pUtil_name);
				}
			}
			{
				sort(m_ImageVec.begin(), m_ImageVec.end(),
					[](t_ImageVec &s1, t_ImageVec &s2)
				{
					if (s1.name[0] == s2.name[0])
					{
						if (s1.name[1] == s2.name[1])
						{
							if (s1.name[2] == s2.name[2])
							{
								if (s1.name[3] == s2.name[3])
								{
									if (s1.name[4] == s2.name[4])
									{
										if (s1.name[5] == s2.name[5])
										{
											if (s1.name[6] == s2.name[6])
											{
												if (s1.name[7] == s2.name[7])
												{
													if (s1.name[8] == s2.name[8])
													{
														if (s1.name[9] == s2.name[9])
														{
															if (s1.name[10] == s2.name[10])
															{
																if (s1.name[11] == s2.name[11])
																{
																	return s1.name[12] < s2.name[12];
																}
																return s1.name[11] < s2.name[11];
															}
															return s1.name[10] < s2.name[10];
														}
														return s1.name[9] < s2.name[9];
													}
													return s1.name[8] < s2.name[8];
												}
												return s1.name[7] < s2.name[7];
											}
											return s1.name[6] < s2.name[6];
										}
										return s1.name[5] < s2.name[5];
									}
									return s1.name[4] < s2.name[4];
								}
								return s1.name[3] < s2.name[3];
							}
							return s1.name[2] < s2.name[2];
						}
						return s1.name[1] < s2.name[1];
					}
					return s1.name[0] < s2.name[0];
				});
			}
		}
	}
#pragma endregion 

	if (ImGui::Button("Save"))
		m_pGameInstance->Save_UIVec();//세이브 저장
	
	ImGui::SameLine();
	
	if (ImGui::Button("Load"))//로드 불러오기
		Load_UIVec();
	
	ImGui::Separator();

	const char*	UI_Set_Level[] = { SELECT_LEVEL };	//@@@@@@@@@@@@@@@@@@@레벨추가시 헤더에있는거 추가좀@@@@@@@@@@@@@@@@@@@
	static int	UI_Set_LevelNum = 2;
	const char* UI_Set_Level_Value = UI_Set_Level[UI_Set_LevelNum];
	static bool	UI_EditMode = false;
	
	if (false == UI_EditMode && (m_currentLevelID != LEVEL_STATIC && m_currentLevelID != LEVEL_LOADING && m_currentLevelID != LEVEL_LOADING_START)) //에디트모드 꺼져있을땐 항상 현재스테이지로 생성하도록
		UI_Set_LevelNum = m_currentLevelID - 2;	//SELECT_LEVEL 갯수차이 스태틱, 로딩 포함하면 + 2라서;

	m_pGameInstance->Set_LevelEditMode(UI_EditMode);
	
	ImGui::Checkbox("Edit_UI_Select", &UI_EditMode);
	if (ImGui::BeginCombo("Level_Select", UI_Set_Level_Value, 0))
	{
		for (int i = 0; i < IM_ARRAYSIZE(UI_Set_Level); ++i)
		{
			const bool is_selected = (UI_Set_LevelNum == i);
			if (ImGui::Selectable(UI_Set_Level[i], is_selected))
				UI_Set_LevelNum = i;
		}
		m_pGameInstance->Set_EditLevel((_uint)UI_Set_LevelNum + 2);  //스태틱 로딩 해서 +2
		ImGui::EndCombo();
	}

	ImGui::Separator();
	
	ImGui::Text("Make UI");
	const char* UI_Class_Type[] = { "None", "LevelMoveButton", "Text" };	//UI 추가할때마다 생성해주기
	static int UI_Class_SelectNum = 0;
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

	switch (UI_Class_SelectNum)	//버튼클래스 생성
	{
	case 1:
		ImGui::Separator();
		if(UI_EditMode)		//에디트모드True 일때만 정해진 레벨에 생성 
			Create_LevelMoveButton((_uint)UI_Set_LevelNum + 2);	//레벨스태틱과 로딩 이후의 레벨 +
		else
			Create_LevelMoveButton(m_currentLevelID);
		break;

	case 2:
		ImGui::Separator();
		if (UI_EditMode)		
			Create_UIText((_uint)UI_Set_LevelNum + 2);
		else
			Create_UIText(m_currentLevelID);
		break;
		break;

	default:
		break;
	}

	ImGui::Separator();
	ImGui::Separator();
	ImGui::Checkbox("UseLevelMoveUIButton", &m_bSelectUILevelMoveMode);
	ImGui::Separator();
	
	UITool_SelectUI();	//선택한 UI의 정보 읽기 및 쓰기
	

	ImGui::End();
}

void CImguiMgr::UITool_SelectUI(void)
{
	static	_uint	ChangeLevelID = m_currentLevelID;	//레벨바뀌면 셀렉된 UI nullptr 되게만듬
	static	CUI*	ChancgeUI = nullptr;
	static	_float	InputSize[3] = { 100.f, 100.f, 1.f };
	static	_float	InputPos[3] = { 0.f, 0.f, 0.f };
	static	_float	InputThrowPos[2] = { 0.f, 0.f };
	static	char	InputName[MAX_PATH] = {};

	if (KEY(RBUTTON, TAP))	//우클릭시 null
		m_pSelectUI = nullptr;

	if (ChangeLevelID != m_currentLevelID)	//레벨바뀔시 Null
	{
		ChangeLevelID = m_currentLevelID;
		m_pSelectUI = nullptr;
	}

	ImGui::Text("Select_UI_Desc");

	if (nullptr == m_pSelectUI)
	{
		ImGui::Text("\n	select UI is NULL");
		return;
	}

	if (m_pSelectUI != ChancgeUI)	//클릭한게 바뀔경우 한번만실행
	{
		ChancgeUI = m_pSelectUI;

		char* tempName = CStrUtil::ConvertWCtoC(m_pSelectUI->Get_UIName());
		strcpy_s(InputName, MAX_PATH, tempName);
		Safe_Delete_Array(tempName);

		_float3 tempSize = m_pSelectUI->Get_Size();
		
		InputSize[0] = tempSize.x;
		InputSize[1] = tempSize.y;
		
		_float3 tempPos = m_pSelectUI->Get_Pos();
		
		InputPos[0] = tempPos.x;
		InputPos[1] = tempPos.y;

		_float2 tempThrowPos = m_pSelectUI->Get_ThrowPos();

		InputThrowPos[0] = tempThrowPos.x;
		InputThrowPos[1] = tempThrowPos.y;

		char* OnceClass = CStrUtil::ConvertWCtoC(m_pSelectUI->Get_UIClassName());	//각클래스별 한번만실행하는코드

		if (!strcmp(OnceClass, "CUI_LevelMoveButton"))
		{
			m_uSelectUILevelMoveNum = static_cast<CUI_LevelMoveButton*>(m_pSelectUI)->GetMoveLevel() - 2; //스태틱,로딩레벨을 뺀 레벨
		}
		else if (!strcmp(OnceClass, "CUI_Text"))
		{
			m_fSelectUITextScale = static_cast<CUI_Text*>(m_pSelectUI)->GetUIScale();
			
			_float4 _SelColor = static_cast<CUI_Text*>(m_pSelectUI)->GetUITextColor();
			m_fSelectUIColor[0] = _SelColor.x;
			m_fSelectUIColor[1] = _SelColor.y;
			m_fSelectUIColor[2] = _SelColor.z;
			m_fSelectUIColor[3] = _SelColor.w;
	}

		Safe_Delete_Array(OnceClass);
	}

	if (ImGui::InputText("Name##2", InputName, MAX_PATH))
	{
		_tchar* temp = CStrUtil::ConvertCtoWC(InputName);
		m_pSelectUI->Set_UIName(temp);
		Safe_Delete_Array(temp);
	}

	if (ImGui::InputFloat2("Set Size##2", InputSize, "%.1f", 0))
	{
		for (int i = 0; i < 2; i++)
		{
			if (0.f >= InputSize[i])	//바꾸더라도 0보다 작아지지않게함
				InputSize[i] = 1.f;
		}
		m_pSelectUI->Set_Size(_float3(InputSize[0], InputSize[1], InputSize[2]));
	}

	if (ImGui::InputFloat2("Set Pos##2", InputPos, "%.1f", 0))
		m_pSelectUI->Set_Pos(_float3(InputPos[0], InputPos[1], InputPos[2]));
	
	if (ImGui::InputFloat2("Set ThrowPos", InputThrowPos, "%.1f", 0))
		m_pSelectUI->Set_ThrowPos(_float2(InputThrowPos[0], InputThrowPos[1]));


	//@@@@@@@@@@@@@@@@@@클래스별 추가항목@@@@@@@@@@@@@@@@@@//
	char* tempClassName = CStrUtil::ConvertWCtoC(m_pSelectUI->Get_UIClassName());

	if (!strcmp(tempClassName, "CUI_LevelMoveButton"))
		SelectUI_LevelMoveButton();
	else if (!strcmp(tempClassName, "CUI_Text"))
		SelectUI_Text();

	Safe_Delete_Array(tempClassName);
	//@@@@@@@@@@@@@@@@@@클래스별 추가항목 END@@@@@@@@@@@@@@@@@@//

	ImGui::Checkbox("MouseMove", &m_bSelectUIMove);
	
	if (KEY(F3, TAP) && false == m_bSelectUIMove)	//M버튼을눌러도 이동시킬수있게
		m_bSelectUIMove = true;

	if (m_bSelectUIMove)
	{
		ImGuiIO& io = ImGui::GetIO();
		_float Offset[2] = { g_iWinCX * 0.5f  , g_iWinCY * 0.5f };

		InputPos[0] = io.MousePos.x - Offset[0];
		InputPos[1] = Offset[1] - io.MousePos.y;


		m_pSelectUI->Set_Pos(_float3(InputPos[0], InputPos[1], InputPos[2]));
		
		if (KEY(LBUTTON, TAP))
			m_bSelectUIMove = false;
	}

	if (ImGui::Button("DELETE"))
	{
		m_pSelectUI->Dead();
		m_pSelectUI = nullptr;
	}
}

void CImguiMgr::SelectUI_LevelMoveButton(void)
{
	const char* MoveLevelList[] = { SELECT_LEVEL, "LEVEL_END" };
	const char* MoveLevelValue = MoveLevelList[m_uSelectUILevelMoveNum];
	if (ImGui::BeginCombo("MoveLevel", MoveLevelValue, 0))
	{
		for (int i = 0; i < IM_ARRAYSIZE(MoveLevelList); ++i)
		{
			const bool is_selected = (m_uSelectUILevelMoveNum == i);
			if (ImGui::Selectable(MoveLevelList[i], is_selected))
			{
				m_uSelectUILevelMoveNum = i;

				static_cast<CUI_LevelMoveButton*>(m_pSelectUI)->SetMoveLevel(m_uSelectUILevelMoveNum + 2);
			}
		}
		ImGui::EndCombo();
	}
}

void CImguiMgr::SelectUI_Text(void)
{
	//매번 보내주는게 아닌 입력버튼을 눌러서 변경하는식으로 바꾸기
	static char _Input[MAX_PATH] = {0,};
	ImGui::InputText(u8"UI_텍스트", _Input, sizeof(_Input));
	ImGui::SameLine();
	if (ImGui::Button("Input"))
	{
		_tchar* pCharInput = CStrUtil::ConvertUTF8toWC(_Input);

		static_cast<CUI_Text*>(m_pSelectUI)->SetUIText(pCharInput);

		Safe_Delete_Array(pCharInput);
	}
	if (ImGui::InputFloat("Scale", &m_fSelectUITextScale))
	{
		if (0.f >= m_fSelectUITextScale)
			m_fSelectUITextScale = 0.01f;
		static_cast<CUI_Text*>(m_pSelectUI)->SetUIScale(m_fSelectUITextScale);
	}
	
	if (ImGui::ColorEdit3("Color", m_fSelectUIColor))
	{
		for (int i = 0; i < 4; i++)
		{
			if (0.f >= m_fSelectUIColor[i])
				m_fSelectUIColor[i] = 0.f;
		}
		_float4 _InputColor = { m_fSelectUIColor[0], m_fSelectUIColor[1], m_fSelectUIColor[2], m_fSelectUIColor[3] };
		static_cast<CUI_Text*>(m_pSelectUI)->SetUITextColor(_InputColor);
	}
}

void CImguiMgr::Create_LevelMoveButton(_uint _Level)	//LevelButton 을 정의하고 만들어줌 (Create는 밖으로 뻴것같음)
{
	static _float UI_Size[3] = { 100.f, 100.f, 1.f };
	static _float UI_Pos[3] = { 0.f, 0.f, 0.f };
	static _float UI_ThrowPos[2]{ 0.f, 0.f };
	static char UI_Name[MAX_PATH] = {};

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
				{
					Image_Num = i;

					ID3D11Resource* pResource = nullptr;
					m_ImageVec[Image_Num].texture->Get_ResourceView()->GetResource(&pResource);
					D3D11_RESOURCE_DIMENSION resourceType;
					pResource->GetType(&resourceType);

					if (D3D11_RESOURCE_DIMENSION_TEXTURE2D == resourceType)
					{
						ID3D11Texture2D* pTexture2D = (ID3D11Texture2D*)pResource;
						D3D11_TEXTURE2D_DESC desc;
						pTexture2D->GetDesc(&desc);

						UI_Size[0] = (float)desc.Width;
						UI_Size[1] = (float)desc.Height;
					}
				}
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

	ImGui::InputText("Name", UI_Name, MAX_PATH);
	ImGui::InputFloat2("Set Size", UI_Size, "%.1f", 0);
	ImGui::InputFloat2("Set Pos", UI_Pos, "%.1f", 0);
	ImGui::InputFloat2("Set ThrowPos", UI_ThrowPos, "%.1f", 0);


	if (m_currentLevelID == LEVEL::LEVEL_LOADING || Render_Num == 5/*UI_NONE*/) //로딩이거나, UI그룹설정안했다면
	{
		ImGui::Text("RenderType is NONE");
		return;
	}

	if (ImGui::Button("Create_UI"))
	{
		string ImageName = "Prototype_Component_Texture_";
		ImageName += m_ImageVec[Image_Num].name;
		
		CUI * pUI = CUI_LevelMoveButton::Create(m_pDevice, m_pContext);

		pUI->LoadUIImage(CStrUtil::ConvertCtoWC(ImageName.c_str()));
		pUI->Set_UIName(CStrUtil::ConvertCtoWC(UI_Name));
		pUI->Set_UIType((UI_TYPE)Render_Num);
		pUI->Set_Size(_float3(UI_Size[0], UI_Size[1], UI_Size[2]));
		pUI->Set_ThrowPos(_float2(UI_ThrowPos[0], UI_ThrowPos[1]));

		pUI->Set_UILevel(_Level);
		pUI->Initialization();

		if (FAILED(m_pGameInstance->Add_UI(_Level, pUI)))	//받아온레벨에다 생성
		{
			MSG_BOX("UI생성실패");
		}


		ImGui::Text("Create");
	}
}

void CImguiMgr::Create_UIText(_uint _Level)
{
	static _float UI_Size[3] = { 100.f, 100.f, 1.f };
	static _float UI_Pos[3] = { 0.f, 0.f, 0.f };
	static _float UI_ThrowPos[2] = { 0.f, 0.f };
	static char UI_Name[MAX_PATH] = {};

	const char* Render_Type[] = { "UI_POST", "UI_DIALOG_BUTTON", "UI_DIALOG", "UI_BUTTTON", "UI_BACKGROUND", "NONE" };
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

	ImGui::InputText("Name", UI_Name, MAX_PATH);
	ImGui::InputFloat2("Set Size", UI_Size, "%.1f", 0);
	ImGui::InputFloat2("Set Pos", UI_Pos, "%.1f", 0);
	ImGui::InputFloat2("Set ThrowPos", UI_ThrowPos, "%.1f", 0);

	if (m_currentLevelID == LEVEL::LEVEL_LOADING || Render_Num == 5/*UI_NONE*/) //로딩이거나, UI그룹설정안했다면
	{
		ImGui::Text("RenderType is NONE");
		return;
	}

	if (ImGui::Button("Create_UI"))
	{

		CUI * pUI = CUI_Text::Create(m_pDevice, m_pContext);

		pUI->Set_UIName(CStrUtil::ConvertCtoWC(UI_Name));
		pUI->Set_UIType((UI_TYPE)Render_Num);
		pUI->Set_Size(_float3(UI_Size[0], UI_Size[1], UI_Size[2]));
		pUI->Set_Pos(_float3(UI_Pos[0], UI_Pos[1], UI_Pos[2]));
		pUI->Set_ThrowPos(_float2(UI_ThrowPos[0], UI_ThrowPos[0]));
		pUI->Set_UILevel(_Level);
		pUI->Initialization();

		if (FAILED(m_pGameInstance->Add_UI(_Level, pUI)))	//받아온레벨에다 생성
		{
			MSG_BOX("UI생성실패");
		}


		ImGui::Text("Create");
	}

}

void CImguiMgr::Load_UIVec(void)	//불러오기
{
	m_pSelectUI = nullptr;
	m_pGameInstance->Clear_UIVec();

	json loadJson;

	ifstream fin;
	fin.open("../../Resources/Data/SaveData.json");

	if (fin.is_open())
		fin >> (loadJson);
	else
	{
		MSG_BOX("JSON_UI 불러오기 실패");
		return;
	}
	fin.close();

	auto UIJsonVec = loadJson["UI"];

	for (auto it = UIJsonVec.begin(); it != UIJsonVec.end(); ++it)
	{
		string	_ClassName = (*it)["ClassName"];
		string	_TextureName = (*it)["TextureName"];
		string	_Name = (*it)["Name"];

		_uint	_Level = (*it)["Level"];
		_uint	_Type = (*it)["Type"];

		_float3 _Pos;
		_Pos.x = (*it)["Pos_x"];
		_Pos.y = (*it)["Pos_y"];
		_Pos.z = (*it)["Pos_z"];

		_float3 _Size;
		_Size.x = (*it)["Size_x"];
		_Size.y = (*it)["Size_y"];
		_Size.z = (*it)["Size_z"];
		
		_float2 _ThrowPos;
		_ThrowPos.x = (*it)["ThrowPos_x"];
		_ThrowPos.y = (*it)["ThrowPos_y"];

		CUI* pUI = nullptr;

		if (!strcmp(_ClassName.c_str(), "CBackGround"))
			pUI = CBackGround::Create(m_pDevice, m_pContext);
		else if (!strcmp(_ClassName.c_str(), "CUI_LevelMoveButton"))
		{
			pUI = CUI_LevelMoveButton::Create(m_pDevice, m_pContext);

			_uint	_MoveLevel = (*it)["MoveLevel"];
			static_cast<CUI_LevelMoveButton*>(pUI)->SetMoveLevel(_MoveLevel);
		}
		else if (!strcmp(_ClassName.c_str(), "CUI_Text"))
		{
			pUI = CUI_Text::Create(m_pDevice, m_pContext);

			string _UITextTemp = (*it)["UIText"];
			_tchar* _UIText = CStrUtil::ConvertUTF8toWC(_UITextTemp.c_str());
			static_cast<CUI_Text*>(pUI)->SetUIText(_UIText);
			Safe_Delete_Array(_UIText);

			static_cast<CUI_Text*>(pUI)->SetUIScale( ((*it)["UITextScale"]) );

			_float4 _UITextColor;
			_UITextColor.x = (*it)["UITextColor_x"];
			_UITextColor.y = (*it)["UITextColor_y"];
			_UITextColor.z = (*it)["UITextColor_z"];
			_UITextColor.w = (*it)["UITextColor_w"];

			static_cast<CUI_Text*>(pUI)->SetUITextColor(_UITextColor);

		}

		if (nullptr == pUI)
		{
			MSG_BOX("클래스정보 에러로 생성불가");
			return;
		}

		_tchar* pUtil_ImageTag = CStrUtil::ConvertCtoWC(_TextureName.c_str());
		_tchar* pUtil_Name = CStrUtil::ConvertCtoWC(_Name.c_str());

		pUI->LoadUIImage(pUtil_ImageTag);
		pUI->Set_UIType((UI_TYPE)_Type);
		pUI->Set_Size(_float3(_Size.x, _Size.y, _Size.z));
		pUI->Set_Pos(_float3(_Pos.x, _Pos.y, _Pos.z));
		pUI->Set_UIName(pUtil_Name);
		pUI->Set_UILevel(_Level);
		pUI->Set_ThrowPos(_ThrowPos);
		pUI->Initialization();

		if (FAILED(m_pGameInstance->Add_UI(_Level, pUI)))
		{
			MSG_BOX("UI정보 불러오기 실패");
		}

		Safe_Delete_Array(pUtil_Name);
		Safe_Delete_Array(pUtil_ImageTag);
	}

	//MSG_BOX("로드성공");

	return;
}

void CImguiMgr::GetLevelString(char * str, _uint len, _uint _LEVEL) //@@@@@@@@@@@@@@@@@@@레벨추가시 업데이트@@@@@@@@@@@@@@@@@@@
{
	_uint _LevelID = m_currentLevelID;

	if (0 < _LEVEL)	//들어온 레벨값이 있을경우 그 레벨에 맞는 문자열로 넘겨줌  _LEVEL은 0으로 디폴트값있음
		_LevelID = _LEVEL;

	switch (_LevelID)
	{
	case LEVEL_STATIC:
		strcpy_s(str, len, "LEVEL_STATIC");
		break;
	case LEVEL_LOADING:
		strcpy_s(str, len, "LEVEL_LOADING");
		break;
	case LEVEL_LOGO:
		strcpy_s(str, len, "LEVEL_LOGO");
		break;
	case LEVEL_LOBBY:
		strcpy_s(str, len, "LEVEL_LOBBY");
		break;
	case LEVEL_CAFE:
		strcpy_s(str, len, "LEVEL_CAFE");
		break;
	case LEVEL_SCHEDULE:
		strcpy_s(str, len, "LEVEL_SCHEDULE");
		break;
	case LEVEL_STUDENTS:
		strcpy_s(str, len, "LEVEL_STUDENTS");
		break;
	case LEVEL_FORMATION:
		strcpy_s(str, len, "LEVEL_FORMATION");
		break;
	case LEVEL_CIRCLE:
		strcpy_s(str, len, "LEVEL_CIRCLE");
		break;
	case LEVEL_MANUFACTURE:
		strcpy_s(str, len, "LEVEL_MANUFACTURE");
		break;
	case LEVEL_SHOP:
		strcpy_s(str, len, "LEVEL_SHOP");
		break;
	case LEVEL_GACHA:
		strcpy_s(str, len, "LEVEL_GACHA");
		break;
	case LEVEL_WORK:
		strcpy_s(str, len, "LEVEL_WORK");
		break;
	case LEVEL_GAMEPLAY:
		strcpy_s(str, len, "LEVEL_GAMEPLAY");
		break;
	case LEVEL_MAPTOOL:
		strcpy_s(str, len, "LEVEL_MAPTOOL");
		break;
	case LEVEL_LOADING_START:
		strcpy_s(str, len, "LEVEL_LOADING_START");
		break;
	case LEVEL_END:
		strcpy_s(str, len, "ERROR");
		break;
	default:
		break;
	}
	return;
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