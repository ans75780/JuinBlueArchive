#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget final : public CBase
{
public:
	typedef struct tagPasses
	{
		ID3DX11EffectPass*			pPass = nullptr;
		ID3D11InputLayout*			pInputLayout = nullptr;
	}PASSDESC;

public:
	//ID3D11Texture2D라는 객체를 생성함
	
	/*
	ID3D11RenderTargetView를 생성
	ID3D11ShaderResourceView를 생성
	*/
	HRESULT		Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);

private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderTarget(const CRenderTarget& rhs);
	virtual ~CRenderTarget() = default;


private:

	ID3D11RenderTargetView*		m_pRTV = nullptr;


	//셰이더 전역변수로 바인딩 될수 ㅣ있는 객체.
	ID3D11ShaderResourceView*	m_pSRV = nullptr;


	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext= nullptr;
	_float4						m_vClearColor;

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	virtual void Free() override;
};

END