#pragma once

#include "Client_Defines.h"
#include "Level.h"


namespace Engine
{
	class CGameObject;
	class CCamera;

}

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
public:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Enemy(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Student(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Baricade(const _tchar* pLayerTag);
	HRESULT Ready_Light();

private:
	class CCamera_Stage*	m_pStageCam = nullptr;
	class CCamera_Event*	m_pEventCam= nullptr;
	class CCamera_Free*		m_pFreeCam= nullptr;

	class CEnemy*			m_pEnemy = nullptr;
	class CCombatFormation*	m_pCombatFormaiton = nullptr;
public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END