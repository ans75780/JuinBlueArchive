#pragma once

#include "Client_Defines.h"
#include "Charater.h"

BEGIN(Client)

class CChara_Haruka final : public CCharater
{
public:
	CChara_Haruka(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChara_Haruka(const CChara_Haruka& rhs);
	virtual ~CChara_Haruka() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void	HarukaExCheck();
	void	ExCamCheck();
	void	ExsPlayOnce();
	void	StateCheck(_float& fTimeDelta);
	void	CreateShot(_float _Damage);

private:
	_bool	m_bExShotGunOnce[9];


public:
	static CChara_Haruka* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END