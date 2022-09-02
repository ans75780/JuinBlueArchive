#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CUI_Canvas final : public CBase
{
private:
	CUI_Canvas(ID3D11Device* pDevice, ID3D11DeviceContext*	pContext);
	~CUI_Canvas() = default;
public:
	static CUI_Canvas*	Create(ID3D11Device* pDevice, ID3D11DeviceContext*	pContext);
	
	void		Free() override;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void		Add_UI(class CUI* pUI);
	vector<class CUI*>*		Get_UIVec() {  return m_vecUI; }

private:
	void					Check_UI(CUI*	pUI);

private:
	_bool					m_bEventCurFrame = false;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	_bool					m_bOnDialog;
	vector<class CUI*>		m_vecUI[UI_END];
	typedef vector<class CUI*> VECTOR_UI;
};
END
