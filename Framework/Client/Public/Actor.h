#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CMeshContainer;
class CCollider;
class CAnimation;
END

BEGIN(Client)

class CActor : public CGameObject
{
public:
	//스테이지 이동중인지,전투중인지,죽었는지
	/*
	이때 중요한건 스테이지 이동은 말그대로 전투를 벗어나서 스테이지를 이동할때 전환됨.
	전투할때 이동하는건 그냥 배틀 상태인거임.
	*/
	enum STAGE_STATE { STAGE_STATE_MOVE, STAGE_STATE_BATTLE, STAGE_STATE_DEAD, STAGE_STATE_END };
public:
	CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CActor(const CActor& rhs);
	virtual ~CActor() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_MeshPart(CMeshContainer* pMesh);

	class CStateMachineBase*		Get_StateMachine() { return m_pStateMachine; }
	void	Set_Transform(_vector vPos);
	class CAnimation*	Get_Animation(const char* pAnimationName);
	_bool	Collision_AABB(RAYDESC& ray, _float& distance);
	CCollider*		Get_AABB() { return m_pAABBCom; }
	CModel*			Get_ModelCom(){ return m_pModelCom; }
	
	STAGE_STATE		Get_StageState() { return m_eStageState; }
	void			Set_StageState(STAGE_STATE eState) { m_eStageState = eState; }


	virtual void	Damaged(_float fAtk);


protected:
	class CStateMachineBase* m_pStateMachine = nullptr;
	CShader*				 m_pShaderCom = nullptr;
	CRenderer*				 m_pRendererCom = nullptr;
	CModel*					 m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;



	CCollider*				m_pAttackRangeCollider = nullptr;
	CCollider*				m_pBodyCollider = nullptr;


protected:
	_uint					m_iAnimIndex = 0;
	STAGE_STATE			m_eStageState = STAGE_STATE_END;
	class CHpBar*	m_pHpBar = nullptr;


protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();
	virtual HRESULT	SetUp_StateMachine(_uint iLevel) = 0;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
