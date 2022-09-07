#pragma once
#include "Engine_Defines.h"
#include "Base.h"
BEGIN(Client)

class CSquad : public CBase
{
private:
	CSquad();
	~CSquad();

public:
	CSquad*		Create();
private:
	vector<class CStudent*>m_Students;

public:
	virtual void Free() override;
};

END