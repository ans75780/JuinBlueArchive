#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "Base.h"
namespace Engine
{
	class CGameObject;
	class CCamera;
}

BEGIN(Client)

class CUI_CombatFormation_Skill : public CBase
{
private:
	CUI_CombatFormation_Skill(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	~CUI_CombatFormation_Skill() = default;

public:

	HRESULT Initialize(void*	pArg);

	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT	StartGame();



private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;


private:
	vector<class CStudent*> m_vecStudent;

	class CUI_Default*	m_pUICostBack = nullptr;
	class CUI_Default*	m_pUICostFront = nullptr;
	
public:
	static CUI_CombatFormation_Skill*	Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);

private:
	_float		m_fCost;
	_float		m_fCostMax;

public:
	virtual void	Free() override;

};
END

