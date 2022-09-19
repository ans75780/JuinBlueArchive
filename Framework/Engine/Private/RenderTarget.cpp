#include "RenderTarget.h"
#include "..\Public\RenderTarget.h"


HRESULT CRenderTarget::Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{

	m_vClearColor = vClearColor;

	ID3D11Texture2D*		pTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET || D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;


	m_pDevice->CreateTexture2D(&TextureDesc,nullptr, &pTexture);


	m_pDevice->CreateRenderTargetView(pTexture, nullptr, &m_pRTV);


	m_pDevice->CreateShaderResourceView(pTexture, nullptr, &m_pSRV);


	Safe_Release(pTexture);

	return S_OK;
}

CRenderTarget::CRenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice),m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}


CRenderTarget * CRenderTarget::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CRenderTarget*	pInstance = new CRenderTarget(pDevice, pContext);


	if (FAILED(pInstance->Initialize(iWidth, iHeight, eFormat, vClearColor)))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CRenderTarget::Free()
{

	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);



	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
