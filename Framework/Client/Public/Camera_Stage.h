#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Stage final : public CCamera
{
public:
	CCamera_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Stage(const CCamera_Stage& rhs);
	virtual ~CCamera_Stage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CCamera_Stage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	void	Set_Target(class CStudent*	pTarget);

	virtual void Free() override;

private:
	_float3			m_vOffset;

	_float3			m_vTargetPos;
	_float			m_fSpeed;

	class CStudent*	m_pTarget = nullptr;

};

END