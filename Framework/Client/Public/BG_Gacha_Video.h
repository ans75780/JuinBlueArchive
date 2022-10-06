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
class CBG_Gacha_Video final : public CGameObject
{
public:
	CBG_Gacha_Video(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBG_Gacha_Video() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialization();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual _float3 Get_BgPos() { return m_fPos; }

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
	static CBG_Gacha_Video* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END