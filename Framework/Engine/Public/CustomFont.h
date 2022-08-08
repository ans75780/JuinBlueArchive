#pragma once

#include "Base.h"

class CCustomFont final : public CBase
{
private:
	CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT	Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar * pString, const _float2& vPosition, _vector vColor);
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

private:
	SpriteBatch*	m_pBatch = nullptr;
	SpriteFont*		m_pFont = nullptr;

public:
	CCustomFont*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pPath);
	virtual void	Free() override;

};

