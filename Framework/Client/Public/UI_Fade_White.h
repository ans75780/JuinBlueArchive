#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Fade_White final : public CUI
{
public:
	CUI_Fade_White(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Fade_White() = default;

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
	_bool	Get_FullWhite() { return m_bFullWhite; }
	void	Set_FullWhite(_bool _b) { m_bFullWhite = _b; }
	
	void	Set_Alpha(_bool _b) { m_bAlpha = _b; } //Æ®·çÀÏ¶§ ÇÏ¾á»öÀ¸·Î µ¤¾îÁü
	void	Set_AlphaValue(_float _value) { m_fAlpha = _value; }

private:
	_float	m_fAlpha = 0.f;
	_bool	m_bAlpha = true;
	_bool	m_bFullWhite = false;
	

public:
	static CUI_Fade_White* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END