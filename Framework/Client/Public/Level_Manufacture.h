#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Manufacture final : public CLevel
{
public:
	CLevel_Manufacture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Manufacture() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static CLevel_Manufacture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END