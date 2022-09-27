#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Students final : public CLevel
{
public:
	CLevel_Students(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Students() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static CLevel_Students* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END