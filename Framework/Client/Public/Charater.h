#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CAnimation;
class CBoneNode;
END

BEGIN(Client)

class CCharater : public CGameObject
{
public:
	enum CHARA_STATE { IDLE, RUN, RUN_END, RELOAD, ATK_START, ATK_DELAY, ATK_ING, ATK_END, EX_CUTIN, EX, VICTORY, DEAD, STATE_END };
	
	struct BulletDesc
	{
		_vector CreatePos;
		_vector TargetPos;
		_float	Damage;
		class CHod* Hod = nullptr;
	};

public:
	CCharater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharater(const CCharater& rhs);
	virtual ~CCharater() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	class CAnimation*	Get_Animation(const char* pAnimationName);
	CModel*				Get_Model() { return m_pModelCom; }
	void				Damage_Chara(_float _Damage);
	
	_int				Get_Random3();

protected:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	class CHpBar*			m_pHpBar = nullptr;
	class CHod*				m_pHod = nullptr;
	class CCamera_Free*		m_pCamera = nullptr;
	CAnimation*				m_pAnimation_ExCutin = nullptr;
	CAnimation*				m_pAnimation_Exs = nullptr;


protected:
	_bool					m_bExUse = false;
	CHARA_STATE				m_eState = RUN;
	_float					m_fTime = 0.f;
	_int					m_iAmmo = 0;

	_float					m_fForwardDistance = 0.f;
	_float					m_fAttackDelay = 0.f;
	_int					m_iMaxAmmo = 0;
	_bool					m_bAtkIngOnce = true;
	_bool					m_bBulletCreateOnce = true;
	_bool					m_bCharaDead = false;
	_bool					m_bDeadOnce = true;

private:
protected:
	HRESULT SetUp_Components(_tchar* ModelName);
	HRESULT SetUp_ShaderResource();

public:
	static CCharater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END