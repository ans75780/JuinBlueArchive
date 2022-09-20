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


class CUI_Text final : public CUI
{
public:
	CUI_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Text() = default;

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
	void	SetUIText(const _tchar* _text) { lstrcpy(m_szText, _text); }
	void	SetUIScale(_float _Scale) { m_fScale = _Scale; }
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	_tchar	m_szText[MAX_PATH]; //출력할텍스트
	_float4	m_fColor;
	_float	m_fScale;

public:
	static CUI_Text* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END