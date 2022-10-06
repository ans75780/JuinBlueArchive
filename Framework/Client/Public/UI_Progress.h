#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CUI;
class CKey_Manager;
END

BEGIN(Client)


class CUI_Progress  : public CUI
{
public:
	CUI_Progress(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Progress() = default;

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



protected:
	virtual HRESULT SetUp_Component();
	virtual HRESULT SetUp_ShaderResource();

protected:
	_uint	m_MoveLevel = LEVEL_END;
	_bool	m_bUIButtonDown = false;
	_float3	m_fOriginSize = { 0.f, 0.f, 0.f };
	_float2		m_fProgressBarSize;


	//for Shader

public:
	static CUI_Progress* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END