#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Event final : public CCamera
{
	enum class EVENT_TYPE{EVENT_EX, EVENT_APPEAR, EVENT_END};
public:
	CCamera_Event(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Event(const CCamera_Event& rhs);
	virtual ~CCamera_Event() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CCamera_Event* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CModel* model, EVENT_TYPE eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	class CAnimation*	m_pAnimation;
	_float4x4		m_matMyTransform;
	_float4x4		m_matTargetTransform;
};

END