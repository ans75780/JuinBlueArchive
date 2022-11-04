#pragma once

#include "Client_Defines.h"
#include "Charater.h"

BEGIN(Client)

class CChara_Zunko final : public CCharater
{
public:
	CChara_Zunko(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChara_Zunko(const CChara_Zunko& rhs);
	virtual ~CChara_Zunko() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void	ZunkoExCheck();
	void	ExCamCheck();
	void	ExsPlayOnce();
	void	StateCheck(_float& fTimeDelta);
	void	CreateBullet(_float _Damage);

private:
	_bool	m_bCreateBulletThree[3];
	_bool	m_bExBulletCreate[50];
	_int	m_iCount = 0;
	_bool	m_bSoundOnceZun = true;

public:
	static CChara_Zunko* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END