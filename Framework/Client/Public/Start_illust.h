#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CStart_illust final : public CGameObject
{
public:
	CStart_illust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CStart_illust() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT initialization();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

	_float4x4	m_UIMatProj;
	_float		m_fFrame = 0.f;
	_float3		m_fPos;
	_float3		m_fSize;

public:
	static CStart_illust* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END