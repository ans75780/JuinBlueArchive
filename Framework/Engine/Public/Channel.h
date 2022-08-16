#pragma once

#include "Base.h"

/* 각 애니메이션마다 따로 할당되어 보관된다. */

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim* pAIChannel, class CModel* pModel);
	void Update_TransformationMatrices(_float fCurrentTime);

private:
	char						m_szName[MAX_PATH] = "";
	_uint						m_iNumKeyframes = 0;
	vector<KEYFRAME>			m_KeyFrames;
	class CBoneNode*			m_pBoneNode = nullptr;
	_uint						m_iCurrrentKeyFrame = 0;

public:
	static CChannel* Create(aiNodeAnim* pAIChannel, class CModel* pModel);
	virtual void Free() override;
};

END