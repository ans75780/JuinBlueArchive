#include "stdafx.h"
#include "..\Public\StateBase.h"
#include "Animation.h"

CStateBase::CStateBase(CAnimation * pAnimation, STATE_TYPE eType)
	:m_pAnimation(pAnimation), m_eStateType(eType)
{
}

CStateBase::~CStateBase()
{
}

void CStateBase::Enter()
{
}

_bool CStateBase::Loop(_float fTimeDelta)
{
	m_pAnimation->Update_TransformationMatrices(fTimeDelta);

	if (m_pAnimation->IsFinished())
	{
		switch (m_eStateType)
		{
		case Client::CStateBase::STATE_TYPE::ONCE:
			return false;
		case Client::CStateBase::STATE_TYPE::LOOP:
			break;
		case Client::CStateBase::STATE_TYPE::NONE:
			break;
		default:
			break;
		}
	}
	return true;
}

CStateBase* CStateBase::Exit()
{
	return nullptr;
}

CStateBase * CStateBase::Create(CAnimation * pAnimation, STATE_TYPE eType)
{
	CStateBase*		pInstance = new CStateBase(pAnimation, eType);

	return pInstance;
}

