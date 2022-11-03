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

class CUI_BossHpBar : public CBase
{
private:
	CUI_BossHpBar(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	~CUI_BossHpBar() = default;

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

	class CUI_Default*	m_pHpBarGray = nullptr;
	class CUI_Default*	m_pHpBarFrame = nullptr;
	class CUI_Default*	m_pHpBarColor = nullptr;



	class CUI_Default*	m_pUIBossIcon = nullptr;
	class CUI_Text*		m_pUICostText = nullptr;
	vector<class CUI_Default*>	m_vecSkillCard;

public:
	static CUI_BossHpBar*	Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, class CActor* pActor);

private:
	class CActor*			m_pBoss;

public:
	virtual void	Free() override;

};
END

