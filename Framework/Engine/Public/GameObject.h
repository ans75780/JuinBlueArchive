#pragma once

#include "Transform.h"


BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct GameObjcetDesc		
	{
		_tchar		sz_Name[MAX_PATH];
		_float		m_fDamage = 1.f;
		_float		m_fRange = 5.f;
		_float		m_fMaxHp = 10.f;
		_float		m_fHp = 10.f;
		UNIT_CLASS	m_eClass = UNIT_CLASS_END;
	}OBJ_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag);

	_float Get_CamDistance() {
		return m_fCamDistance;
	}
	OBJ_DESC Get_OBJ_DESC() { return m_desc; }

public:
	class CTransform*	Get_Transform() { return m_pTransformCom; }

public:
	virtual HRESULT	Start_Level() { return S_OK; }

	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);

	virtual HRESULT Render(); 
	virtual const _tchar*	Get_Name();
	
	GameObjcetDesc&		Get_Desc()
	{
		return m_desc;
	}



	//Enable,DIsable
public:
	_bool	Get_Enable() { return m_bEnable; }
	void	Set_Enable(_bool _bEnable) { m_bEnable = _bEnable; }

public:
	_bool	Get_Delete() { return m_bDelete; }
	void	Set_Delete(_bool _bDelete) { m_bDelete = _bDelete; }


protected:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	class CTransform*			m_pTransformCom = nullptr;

	_float						m_fCamDistance = 0.f;
	_bool						m_bEnable = true;
	_bool						m_bDelete = false;

//For ObjectManager

	friend class CObject_Manager;
protected:
	virtual HRESULT		StartLevel(_uint iLevel);


protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

	static const _tchar*		m_pTransformTag;

protected:
	OBJ_DESC	m_desc;

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
private:
	class CComponent* Find_Components(const _tchar* pComponentTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END