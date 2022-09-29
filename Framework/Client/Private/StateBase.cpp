#include "stdafx.h"
#include "..\Public\StateBase.h"
#include "Animation.h"
#include "Engine_Defines.h"
#include "Student.h"
#include "Model.h"
#include "Actor.h"


CStateBase::CStateBase(CActor * pOwner)
	: m_pOwner(pOwner)
{
	Safe_AddRef(m_pOwner);
	m_pModel = (CModel*)m_pOwner->Get_Component(L"Com_Model");
	m_bCalledExit = false;
}

_bool CStateBase::Loop(_float fTimeDelta)
{
	return true;
}

CStateBase* CStateBase::Exit()
{
	return nullptr;
}


void CStateBase::Free()
{
	Safe_Release(m_pOwner);
}

