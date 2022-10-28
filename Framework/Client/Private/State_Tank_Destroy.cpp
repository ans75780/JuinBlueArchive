#include "stdafx.h"
#include "..\Public\State_Tank_Destroy.h"
#include "Actor.h"
#include "Animation.h"
#include "Model.h"
CState_Tank_Destroy::CState_Tank_Destroy(CActor * pStudent)
	:CState_Dead(pStudent)
{
	strcpy_s(m_szName, "_Vital_Death");
}

CState_Tank_Destroy * CState_Tank_Destroy::Create(CActor * pActor)
{
	CState_Tank_Destroy*	pInstance = new CState_Tank_Destroy(pActor);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

_bool CState_Tank_Destroy::Loop(_float fTimeDelta)
{
	if (m_pAnimation->IsFinished())
	{
		m_pAnimation->Pause();
		//m_pAnimation->Reset();
		//m_pOwner->Set_Enable(false);
		//m_pOwner->Set_Delete(true);
		//return true;
	}
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	
	return false;
}
