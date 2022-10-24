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

public:
	CTransform* Get_Transform() { return m_pTransformCom; }
	void		Set_TickStop(_bool _tickStop) { m_bTickStop = _tickStop; }
	void		Set_Render(_bool _Render) { m_bRender= _Render; }
	_float		Get_CardNum() { return m_fCardNum; }
	void		Set_CardNum(_float _fNum) { m_fCardNum = _fNum; }

private:
	virtual HRESULT	SetUp_ShaderResource();

private:
	_float	m_fCardNum = 0.f;
	_bool	m_bTickStop = false;
	_bool	m_bRender = true;


public:
	static CUI_GachaCard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END