#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Frame final : public CUI
{
public:
	CUI_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Frame() = default;

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

public:
	_float	Get_Delay() { return m_fDelay; }
	void	Set_Delay(_float _delay) { m_fDelay = _delay; }
	
	_float	Get_MaxFrame() { return m_fMaxFrame; }
	void	Set_MaxFrame(_float _MaxFrame) { m_fMaxFrame = _MaxFrame; }

private:
	_float	m_fDelay;
	_float	m_fFrame;
	_float	m_fMaxFrame;

	_bool	m_bLoadingImage = false;

public:
	static CUI_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END