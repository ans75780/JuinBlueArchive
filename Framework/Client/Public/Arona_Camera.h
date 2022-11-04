#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CAnimation;
class CUI;
END

BEGIN(Client)

class CArona_Camera final : public CCamera
{
public:
	CArona_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CArona_Camera(const CArona_Camera& rhs);
	virtual ~CArona_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CArona_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void	CameraMove(_float& fTimeDelta);
	void	LookAtArona();
	void	LinkCam();

	void	TickGacha(_float& fTimeDelta);
	void	MakeUI(_float& fTimeDelta);
	void	CreateUI(_uint i);
	void	CardOpen(_float& fTimeDelta);
	void	CardOpen_Num(_uint num, _float& fTimeDelta);


	void	CreateFadeWhite();
	void	CreateFadeBlack();
	void	CreateUIInfo();
	void	CreateUpWall();

private:
	class CArona_GachaCam*	m_pCam = nullptr;
	CAnimation*		m_pCamAni = nullptr;

	class CArona*	m_pArona = nullptr;
	CAnimation*		m_pAronaAni = nullptr;

	_bool m_bTickOnce = true;
	_bool m_bTickGachaOnce = true;
	_bool m_bMakeUIOnce = true;
	_bool m_bMakeUIOnce2 = true;


	_bool m_bOpenEvent = false;
	_bool m_bFadeWhite = false;

	_bool m_bOnceInfo = false;	//카드 1장 깟을때 한번실행
	_bool m_bInfoViewOnce = false;
	_float	m_fInfoViewDelay = 0.f;

	_bool m_bEndOnce = true;

	CUI*	m_pUI[10] = { nullptr, };
	CUI*	m_pFadeWhite = nullptr;
	CUI*	m_pFadeBlack = nullptr;
	CUI*	m_pUpWall = nullptr;
	CUI*	m_pInfoUI[3] = { nullptr, };
	CUI*	m_pLevelBackBtn = nullptr;

	_uint	m_uCardIndex = 0;
	_int	m_iCardOpenChange = -1;

	_float	m_fFOV = 10.f;
	_float	m_fCardDelay = 0.f;

	_bool	m_bAronaSoundOnce0 = true;
	_bool	m_bAronaSoundOnce1 = true;
	_bool	m_bAronaSoundOnce2 = true;

};

END