#pragma once
#include "Engine_Defines.h"
#include "Actor.h"


BEGIN(Engine)

class CBoss abstract : public CActor
{
protected:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CBoss() = default;

public:


protected:



};

END

