#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Event final : public CCamera
{
	enum class EVENT_TYPE{EVENT_STAGE_START, EVENT_EX, EVENT_BOSS_APPEAR, EVENT_STAGE_VICTORY, EVENT_END};
public:
	CCamera_Event(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Event(const CCamera_Event& rhs);
	virtual ~CCamera_Event() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Ready_Event_Stage_Start(class CCamera* pReturnCamera, class CActor* pTarget, class CAnimation* pAnimation, _float3 vOffset);

	void	Ready_Event_Ex(class CCamera* pReturnCamera, class CActor* pTarget);


public:
	void	Event_Stage_Start();
	void	Event_Ex(_float fTimeDelta);


public:
	static CCamera_Event* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


private:

	EVENT_TYPE	m_eEventType = EVENT_TYPE::EVENT_END;
	class CCamera*	m_pReturnToCam = nullptr;
	class CActor* m_pTarget = nullptr;

private:
	_float3			m_vOffset;




	//for ExCam Variable
private:
	class CAnimation*	m_pAnimation;
	_float4		m_fTargetOriginPos;


};

END