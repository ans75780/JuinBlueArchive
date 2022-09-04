#include "stdafx.h"
#include "..\Public\Idle.h"


CIdle::CIdle()
{
}


CIdle::~CIdle()
{
}

void CIdle::Enter()
{
}

_bool CIdle::Loop(_float fTimeDelta)
{
	return _bool();
}

CStateBase * CIdle::Exit()
{
	return nullptr;
}

CStateBase * CIdle::Create(CStudent * pStudent)
{
	CIdle*	pIdle = new CIdle()

	return nullptr;
}
