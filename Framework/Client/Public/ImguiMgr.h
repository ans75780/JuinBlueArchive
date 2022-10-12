#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CUI;
END

BEGIN(Client)

class CImguiMgr : public CBase
{
#define SELECT_LEVEL	"LEVEL_LOGO", "LEVEL_LOBBY", "LEVEL_CAFE",				\
						"LEVEL_SCHEDULE", "LEVEL_STUDENTS", "LEVEL_FORMATION",	\
						"LEVEL_CIRCLE", "LEVEL_MANUFACTURE", "LEVEL_SHOP",		\
						"LEVEL_GACHA", "LEVEL_WORK",							\
						"LEVEL_GAMEPLAY", "LEVEL_MAPTOOL", "LEVEL_LOADING_START",\
						"LEVEL_GACHA_PLAY" 	//레벨추가시 변경

	
	DECLARE_SINGLETON(CImguiMgr)

private:
	CImguiMgr();
	virtual ~CImguiMgr() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(float fTimeDelta);
	HRESULT Render();

public:
	struct t_ImageVec
	{
		CTexture* texture;
		char name[MAX_PATH];
	};

private:
	void	HelloJusin_View(void);
	void		HelloJusin_Tap_Main(void);
	
	void		HelloJusin_Tap_Object(void);
	void			Tap_Object_CObj(void);
	void			Tap_Object_CUI(void);

	void	HelloJusin_Tap_Level(void);

private:	//새창으로 구분
	void		MapTool_View(void);
	
	void		UITool_View(void);
	void			UITool_SelectUI(void);	//m_pSelectUI에 담긴UI 포인터의 정보를 보고 수정할수있음
	void				SelectUI_LevelMoveButton(void);	
	void				SelectUI_Text(void);
	void				SelectUI_Frame(void);
	void			Create_LevelMoveButton(_uint _Level);
	void			Create_UIText(_uint _Level);
	void			Create_UILoading(_uint _Level);

private:
	void		Load_UIVec(void);
	void		GetLevelString(char * str, _uint len, _uint _LEVEL = 0);

private:
	CGameInstance*         m_pGameInstance = nullptr;
	ID3D11Device*         m_pDevice = nullptr;
	ID3D11DeviceContext*   m_pContext = nullptr;


	_uint	m_currentLevelID = 0;
	bool	show_demo_window;
	bool	show_mainBar;
	bool	MapToolCheckBox;
	bool	UIToolCheckBox;

	vector<t_ImageVec> m_ImageVec;

	CUI*	m_pSelectUI;						//선택한 UI 포인터   각 UI의 클릭을 통해 주소를넘겨줌
	bool	m_bSelectUIMove = false;			//선택한 UI마우스로 이동시키기

	_uint	m_uSelectUILevelMoveNum = 0;			//선택한 UI가 레벨무브버튼일때 쓰는 콤보박스용인덱스
	bool	m_bSelectUILevelMoveMode = false;	//체크시 레벨이동이 가능해지는데...이거 릴리즈때..????

	_float	m_fSelectUITextScale = 1.f;		//클릭한 UITextScale 확인용
	float	m_fSelectUIColor[4] = { 1.f, };	

	_float	m_fSelectUIFrameMaxFrame = 0.f;
	_float	m_fSelectUIFrameDelay = 0.f;

public:
	void	Set_SelectUI(CUI* _UI) { m_pSelectUI = _UI; }
	bool	Get_LevelMoveMode() { return m_bSelectUILevelMoveMode; }
public:
	virtual void Free();
}; END