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
public:
	HRESULT Clone_Channel(CAnimation* pPrototype, class CModel* pModel);
private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iNumChannels = 0; /* ���� �ִϸ��̼��� ��뤷�ؾ��� ���� ����. */
	vector<class CChannel*>		m_Channels; /* ���� �ִԿ��� ���Ǵ� ���� ���¸� ������ ��ü�̴�. */

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

