#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
public:
	enum class ANIM_STATE {ANIM_PLAY, ANIM_PAUSE, ANIM_END};
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;
public:
	HRESULT Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	void	Update_TransformationMatrices(_float fTimeDelta);

public:
	void	Update(_float fTimeDelta);
	void	Play();
	void	Pause();
	void	Stop();
	void	Reset();
	ANIM_STATE	Get_AnimState(){ return m_eAnimState; }
	_bool	IsFinished() { return m_isFinished; }
	const	char*		Get_Name() { return m_szName; }

	_float		Get_Duration() { return m_fDuration; }

	void		Set_TimeAcc(_float _TimeAcc) { m_fTickPerSecond = _TimeAcc; }
	_float		Get_TimeAcc() { return m_fTimeAcc; }

	_float		Get_PlayedRatio();

public:
	HRESULT Clone_Channel(CAnimation* pPrototype, class CModel* pModel);
private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iNumChannels = 0; /* 현재 애니메이션이 사용ㅇ해야할 뼈의 갯수. */
	vector<class CChannel*>		m_Channels; /* 현재 애님에서 사용되는 뼈의 상태를 저장한 객체이다. */
	
	_float			m_fDuration = 0.f;
	_float			m_fTickPerSecond = 0.f;
	_float			m_fTimeAcc = 0.f;
	_bool			m_isFinished = false;
	ANIM_STATE		m_eAnimState = ANIM_STATE::ANIM_END;
public:
	static CAnimation* Create(aiAnimation* pAIAnimation, class CModel* pModel);	
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;







};

END

