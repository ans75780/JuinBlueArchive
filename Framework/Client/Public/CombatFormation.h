#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

namespace Engine
{
	class CGameObject;
}

BEGIN(Client)

class CCombatFormation
{
public:
	typedef struct tagFormationDesc
	{
		LEVEL eLevel;
		_tchar szLayer[MAX_PATH];
		_bool	eGameStartEvent;//혹시 모르니 넣음.
	}FORMATIONDESC;

private:
	CCombatFormation();
	~CCombatFormation();

public:

	HRESULT Initialize(void*	pArg);
	void Tick(_float fTimeDelta) ;
	void LateTick(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT	StartGame();


private:
	vector<class CStudent*> m_vecStudent;

	class CCamera_Stage*	m_pStageCam = nullptr;
	class CCamera_Event*	m_pEventCam = nullptr;

public:
	static CCombatFormation*	Create(FORMATIONDESC* pDesc, class CCamera* pStageCam, class CCamera* pEventCam);

private:
	FORMATIONDESC  m_eDesc;
	vector<_vector>			m_vecFormationPos;

};
END

