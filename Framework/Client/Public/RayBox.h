#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CMeshContainer;
class CCollider;
END

BEGIN(Client)

class CStudent final : public CGameObject
{
public:
	CStudent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _tchar*	pModelTag);
	CStudent(const CStudent& rhs);
	virtual ~CStudent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


	void	Set_Transform(_vector vPos);


	_bool	Collision_AABB(RAYDESC& ray, _float& distance);

private:
	CCollider*				m_pAABBCom = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();


public:
	static CStudent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _tchar*	pModelTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
