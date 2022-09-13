#pragma once
#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	CFrustum() {};
	virtual ~CFrustum() = default;

public:
	HRESULT		Initialize();
	void		Tick();//월드 공간산의 여섯개 평면을 구한다.

public:
	_bool	IsIn_Frustum_InWorldSpace(_fvector	vWorldPoint, _float fRange);

private:
	_float4 m_vOriginPoints[8];
	_float4	m_vWorldPlane[6];
};

END
