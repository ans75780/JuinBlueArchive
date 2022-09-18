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
	void		Tick();//���� �������� ������ ����� ���Ѵ�.

public:
	_bool	IsIn_Frustum_InWorldSpace(_fvector	vWorldPoint, _float fRange = 0.f);

public:
	_bool	IsIn_Frustum_InLocalSpace(_fvector	vLocalPoint, _float fRange = 0.f);

	void	Transform_ToLocalSpace(_fmatrix WorldMatrixInv);
private:
	_float4 m_vOriginPoints[8];//������������ ������� 8�� �� 
	
	_float4	m_vWorldPlane[6];
	_vector m_vWorldPoints[8];


	_float4	m_vLocalPlane[6];
};

END
