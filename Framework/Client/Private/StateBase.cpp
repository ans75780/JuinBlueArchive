#include "stdafx.h"
#include "..\Public\StateBase.h"
#include "Animation.h"
#include "Engine_Defines.h"
#include "Student.h"
#include "Model.h"

CStateBase::CStateBase(CStudent * pStudent)
	: m_pStudent(pStudent)
{
	Safe_AddRef(m_pStudent);
	m_pModel = (CModel*)m_pStudent->Get_Component(L"Com_Model");
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
	Safe_Release(m_pStudent);
}

