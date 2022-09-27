#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Cafe final : public CLevel
{
public:
	CLevel_Cafe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Cafe() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	static CLevel_Cafe* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END