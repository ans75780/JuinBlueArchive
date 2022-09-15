#pragma once
#include "Base.h"


BEGIN(Engine)

class CQuadTree final : public CBase
{
public:
	enum  CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END};
public:
	CQuadTree() {};
	virtual ~CQuadTree() = default;

public:
	virtual HRESULT Initialize_Prototype() ;
	virtual HRESULT Initialize();



private:
	_uint		m_iCorner[CORNER::CORNER_END];

public:
	CQuadTree*		Create();
};

END
