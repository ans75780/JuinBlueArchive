#pragma once
#include "Engine_Defines.h"
#include "Effect.h"

BEGIN(Client)

class CEffect_Hit : public CEffect
{
private:
	CEffect_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Hit(const CEffect_Hit& Prototype);
	virtual ~CEffect_Hit() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	Reset();

protected:
	virtual HRESULT			SetUp_Component();
	virtual HRESULT			SetUp_ShaderResource();

private:
	_uint					m_iTextureIndex;
	_float					m_fImgChangeRatio;//몇 퍼센트일때마다 바꿀건가.
public:
	static CEffect_Hit*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) ;
	virtual void Free() override;
};

END