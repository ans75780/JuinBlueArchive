#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Gacha final : public CLevel
{
public:
	CLevel_Gacha(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Gacha() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Ready_Layer_Bg();

public:
	static CLevel_Gacha* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END