#pragma once

#include "Client_Defines.h"
#include "Actor.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CMeshContainer;
class CCollider;
class CAnimation;
END

BEGIN(Client)

class CEnemy final : public CActor
{
protected:
	CEnemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

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


public:
	static CEnemy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
