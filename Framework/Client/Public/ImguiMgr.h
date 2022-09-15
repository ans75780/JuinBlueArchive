#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CImguiMgr : public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

private:
	CImguiMgr();
	virtual ~CImguiMgr() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	void	HelloJusin_View(void);
	void		HelloJusin_Tap_Main(void);
	
	void		HelloJusin_Tap_Object(void);
	void			Tap_Object_CObj(void);
	void			Tap_Object_CUI(void);

	void	HelloJusin_Tap_Level(void);

private:
	void		MapTool_View(void);
	
	void		UITool_View(void);
	void			Define_LevelMoveButton(_uint _Level);
	
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

	struct t_ImageVec
	{
		CTexture* texture;
		char name[MAX_PATH];
	};
	vector<t_ImageVec> m_ImageVec;

public:
	virtual void Free();
}; END