#pragma once
#include "Enemy.h"

#include "Engine_Defines.h"


BEGIN(Client)

class CBoss_Tank final : public CEnemy
{
protected:
	CBoss_Tank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Tank(const CBoss_Tank& rhs);
	virtual ~CBoss_Tank() = default;

public:
	virtual HRESULT StartLevel(_uint iLevel) override;
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void LateTick(_float fTimeDelta);

public:
	virtual void	Damaged(_float fAtk);

protected:
	virtual void	CheckState();

protected:
	virtual HRESULT SetUp_StateMachine(_uint iClonedLevel) override;

protected:
	class CActor*	m_pTarget = nullptr;
	class CTexture*	m_pTexture = nullptr;
public:
	static CBoss_Tank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

