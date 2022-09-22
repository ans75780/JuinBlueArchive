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
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Student(const _tchar* pLayerTag);
	HRESULT Ready_Light();

private:
	vector<class CStudent*> m_vecStudent;
	vector<_vector>			m_vecFormationPos;

	class CCamera_Stage*	m_pStageCam = nullptr;
	class CCamera_Event*	m_pEventCam= nullptr;


public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END