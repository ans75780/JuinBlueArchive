#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_GachaCard final : public CUI
{
public:
	CUI_GachaCard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_GachaCard() = default;

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

private:
	_float	m_fCardNum = 0.f;



public:
	static CUI_GachaCard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END