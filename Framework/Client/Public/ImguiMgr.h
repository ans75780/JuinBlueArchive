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
	CGameInstance*         m_pGameInstance = nullptr;
	ID3D11Device*         m_pDevice = nullptr;
	ID3D11DeviceContext*   m_pContext = nullptr;

	_uint m_currentLevelID = 0;

	bool show_demo_window;
	bool show_mainBar;

public:
	virtual void Free();
}; END