#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Shop final : public CLevel
{
public:
	CLevel_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Shop() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static CLevel_Shop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END