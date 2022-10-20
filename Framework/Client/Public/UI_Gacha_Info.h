#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Gacha_Info final : public CUI
{
public:
	CUI_Gacha_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Gacha_Info() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialization();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	OnLButtonDown();
	virtual void	OnLButtonUp();
	virtual void	OnLButtonClicked();

private:
	virtual HRESULT	SetUp_ShaderResource();
	virtual	HRESULT	SetUp_Component();

public:
	_bool		Get_FullWhite() { return m_bFullAlpha; }
	void		Set_Alpha(_bool _b) { m_bAlpha = _b; } //Æ®·çÀÏ¶§ ÇÏ¾á»öÀ¸·Î µ¤¾îÁü
	void		Set_TickStop(_bool _tickStop) { m_bTickStop = _tickStop; }
	void		Set_Render(_bool _Render) { m_bRender = _Render; }
	void		Set_CharaNum(_float _num) { m_fCharaNum = _num; }

private:
	_float	m_fAlpha = 0.f;
	_bool	m_bAlpha = true;
	_bool	m_bFullAlpha = false;
	_float	m_fCharaNum = 0.f;


	_bool	m_bTickStop = false;
	_bool	m_bRender = true;


public:
	static CUI_Gacha_Info* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END