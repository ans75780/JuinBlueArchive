#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CAnimation;
class CBoneNode;
class CUI;
END

BEGIN(Client)

class CHod final : public CGameObject
{
public:
	CHod(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHod(const CHod& rhs);
	virtual ~CHod() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	class CAnimation*	Get_Animation(const char* pAnimationName);
	CModel*				Get_Model() { return m_pModelCom; }

	void				CreateUI_Hod();	//호드등장UI띄우기
	void				BattlePosSet();	// 전투용포지션으로 맵도바꾸고 이동
	void				StartSet(_float& fTimeDelta);	//위에두개 묶어놓음

	void				DamageAction(_float _Damage);


private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;

private:
	CGameObject*			m_pCutSceneCam = nullptr;
	CAnimation*				m_pCutSceneAnimation = nullptr;
	CUI*					m_pUI = nullptr;
	class CHodHpBar*		m_pHpBar = nullptr;

	_bool					m_bCutSceneOnce = true;
	_bool					m_bUIOnce = true;
	_float					m_fgDamageEffcet = 0.f;
	_bool					m_bDie = false;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CHod* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END