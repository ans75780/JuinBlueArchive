#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Progress_StartLoadingBar  : public CUI
{
public:
	CUI_Progress_StartLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Progress_StartLoadingBar() = default;

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


	virtual	void	Set_Size(_float3 _fSize);
	virtual void	Set_ProgressWidth(_float fWidth);
	virtual void	Set_ProgressHeight(_float fHeight);

	void	Plus_WidthSize(_float _value)/*MAX 1000*/ { m_fPlusValue += _value; }
	_float	Get_PlusValue() { return m_fPlusValue; }

	void	Set_LoadingSusccess(_bool _b) { m_bLoadingSuccess = _b; }

protected:
	virtual HRESULT SetUp_Component();
	virtual HRESULT SetUp_ShaderResource();

protected:
	_uint	m_MoveLevel = LEVEL_END;
	_bool	m_bUIButtonDown = false;
	_float3	m_fOriginSize = { 0.f, 0.f, 0.f };
	
	_float2	m_fFrontBarSize;
	_float4 m_fFrontColor = { 0.f, 0.f, 0.f, 1.f };
	_float	m_fPlusValue = 10.f;
	_bool	m_bLoadingSuccess = false;
	_bool	m_bSoundOnce = true;

public:
	static CUI_Progress_StartLoadingBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END