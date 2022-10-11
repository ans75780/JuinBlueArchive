#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CUI;
END

BEGIN(Client)


class CUI_Loading final : public CUI
{
public:
	CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Loading() = default;

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
	HRESULT SetUp_ShaderResource();

private:
	_float m_fJumpStartDelay;
	_float m_fJumpDelay;
	_float m_fJumpPos;

public:
	static CUI_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END