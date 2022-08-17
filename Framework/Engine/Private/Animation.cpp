#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(aiAnimation * pAIAnimation, class CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_iNumChannels = pAIAnimation->mNumChannels;

	/* 현재 애니메이션을 재상하는데 걸리는 시간. */
	m_fDuration = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*		pAIChannel = pAIAnimation->mChannels[i];

		CChannel*		pChannel = CChannel::Create(pAIChannel, pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrices(_float fTimeDelta)
{
	m_fTimeAcc += m_fTickPerSecond * fTimeDelta;

	if (m_fTimeAcc >= m_fDuration)
	{
		m_isFinished = true;
		m_fTimeAcc = 0.f;
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (m_isFinished)
			m_Channels[i]->ResetKeyFrame();

		m_Channels[i]->Update_TransformationMatrices(m_fTimeAcc);
	}

}

CAnimation * CAnimation::Create(aiAnimation * pAIAnimation, CModel* pModel)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}
