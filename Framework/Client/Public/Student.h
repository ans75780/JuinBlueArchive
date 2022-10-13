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
class CGameInstance;
END

BEGIN(Client)

class CStudent final : public CActor
{
public:
	CStudent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStudent(const CStudent& rhs);
	virtual ~CStudent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	
private:
	virtual HRESULT	StartLevel(_uint iLevel) override;

public:
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
private:
	virtual HRESULT SetUp_ShaderResource() override;
	virtual HRESULT SetUp_StateMachine(_uint iClonedLevel) override;
	virtual void	CheckState() override;//�浹üũ�� Ư�� �̺�Ʈ������ ���º�ȭ�� �����ϱ� ���ؼ� ���� �Լ�

public:
	static CStudent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
