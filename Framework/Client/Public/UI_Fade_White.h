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

private:
	_float	m_fAlpha = 0.f;
	_bool	m_bAlpha = true;

public:
	static CUI_Fade_White* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END