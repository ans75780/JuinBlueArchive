#pragma once

#include "Client_Defines.h"
#include "Level.h"

#include <DirectXCollision.h>

namespace Engine
{
	class CCollider;
}

BEGIN(Client)

class CLevel_Formation final : public CLevel
{
public:
	CLevel_Formation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Formation() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Student(const _tchar* pLayerTag);

	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT Ready_Light();


private:
	_uint					m_iFormationSize;
	vector<class CStudent*> m_vecStudent;
	vector<_vector>			m_vecFormationPos;
	BoundingBox*			m_pRayBoard = nullptr;
	_bool					m_bPicked = false;
	_uint					m_iPickedIndex = 0;
public:
	static CLevel_Formation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END