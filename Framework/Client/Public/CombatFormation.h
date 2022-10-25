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

class CCombatFormation : public CBase
{
public:
	typedef struct tagFormationDesc
	{
		LEVEL eLevel;
		_tchar szLayer[MAX_PATH];
		_bool	eGameStartEvent;//혹시 모르니 넣음.
	}FORMATIONDESC;

private:
	CCombatFormation(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	~CCombatFormation();

public:

	HRESULT Initialize(void*	pArg);

	void Tick(_float fTimeDelta) ;
	void LateTick(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT	StartGame();


private:
	HRESULT				SetUp_ExModels();

private:
	vector<class CStudent*> m_vecStudent;
	vector<class CStudent_FX*>	m_vecExCutinStudent;//EX스킬쓸때만 원점에서 나오는 모델들



	class CCamera_Stage*	m_pStageCam = nullptr;
	class CCamera_Event*	m_pEventCam = nullptr;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;


public:
	static CCombatFormation*	Create(FORMATIONDESC* pDesc, class CCamera* pStageCam, class CCamera* pEventCam, ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);

private:
	FORMATIONDESC  m_eDesc;
	vector<_vector>			m_vecFormationPos;
	
	class CUI_CombatFormation_Skill*	m_pCombatSkill = nullptr;


public:
	virtual void	Free() override;

};
END

