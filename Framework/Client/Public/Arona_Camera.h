#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CArona_Camera final : public CCamera
{
public:
	CArona_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CArona_Camera(const CArona_Camera& rhs);
	virtual ~CArona_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CArona_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void	CameraMove(_float& fTimeDelta);
	void	LookAtArona();
};

END