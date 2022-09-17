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
	virtual HRESULT Render_MeshPart(CMeshContainer* pMesh);

	const _tchar*	Get_Name() { return m_szStudentName; }
	class CStateMachineBase*		Get_StateMachine() { return m_pStateMachine; }
	void	Set_Transform(_vector vPos);


	_bool	Collision_AABB(RAYDESC& ray, _float& distance);


	CCollider*		Get_AABB() { return m_pAABBCom; }
private:
	class CStateMachineBase* m_pStateMachine = nullptr;
	CShader*				 m_pShaderCom = nullptr;
	CRenderer*				 m_pRendererCom = nullptr;
	CModel*					 m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;

private:
	CMeshContainer*			m_pHalo = nullptr;
	CMeshContainer*			m_pBody = nullptr;
	CMeshContainer*			m_pHair = nullptr;
	CMeshContainer*			m_pFace = nullptr;
	CMeshContainer*			m_pHead = nullptr;
	CMeshContainer*			m_pWeapon = nullptr;
private:
	_uint					m_iAnimIndex = 0;
	_tchar					m_szStudentName[MAX_PATH];
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();


public:
	static CStudent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _tchar*	pModelTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
