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


class CUI_LevelMoveButton final : public CUI
{
public:
	CUI_LevelMoveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_LevelMoveButton() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT initialization();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	OnLButtonDown();
	virtual void	OnLButtonUp();
	virtual void	OnLButtonClicked();

public:
	void	SetMoveLevel(_uint _Level) { m_MoveLevel = _Level; }
	_uint	GetMoveLevel() { return m_MoveLevel; }

	virtual	void	Set_Size(_float3 _fSize);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	_uint	m_MoveLevel = LEVEL_END;
	_bool	m_bUIButtonDown = false;
	_float3	m_fOriginSize = { 0.f, 0.f, 0.f };

public:
	static CUI_LevelMoveButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END