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
class CEffect_Crack final : public CGameObject
{
public:
	CEffect_Crack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffect_Crack() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual _float3 Get_BgPos();

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
	_bool						m_bOff = false;
	_float						m_fLifeTime = 0.f;

public:
	static CEffect_Crack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END