#pragma once
#include "imgui.h"
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)


class CImguiMgr : public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

public:
	CImguiMgr();
	virtual ~CImguiMgr() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	HRESULT Render();

	void	Imgui_ShowWindow(bool * p_open);

private:
	CGameInstance*			m_pGameInstance = nullptr;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	_bool					show_demo_window;

public:
	virtual void Free();

private:
	ImVec4 BackBuffer_Color = ImVec4(0.f, 0.f, 1.f, 1.f);


};

END