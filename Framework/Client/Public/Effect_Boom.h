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
class CEffect_Boom final : public CGameObject
{
public:
	CEffect_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffect_Boom() = default;

public:
	virtual HRESULT Initialize(void* pArg);
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

private:
	class CCamera_Free*			m_pCamera = nullptr;
	_float4						m_gVcolor = { 1.f, 1.f, 1.f, 1.f };
	_float						m_fFrame = 0.f;
	_bool						m_bOff = false;

public:
	static CEffect_Boom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END