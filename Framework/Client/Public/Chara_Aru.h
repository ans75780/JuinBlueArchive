#pragma once

#include "Client_Defines.h"
#include "Charater.h"

BEGIN(Client)

class CChara_Aru final : public CCharater
{
public:
	CChara_Aru(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChara_Aru(const CChara_Aru& rhs);
	virtual ~CChara_Aru() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void	AruExCheck();
	void	ExCamCheck();
	void	ExsPlayOnce();
	void	StateCheck(_float& fTimeDelta);


public:
	static CChara_Aru* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END