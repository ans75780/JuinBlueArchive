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
	enum HOD_STATE { READY,IDLE, ATK_START, ATK_DELAY, ATK_ING, ATK_END, GROGGY, EX1, EX2, EX3, EX4, EX5, DEAD, STATE_END };

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
	void				StateCheck(_float& fTimeDelta);
	
	_bool				Get_StopAnime() { return m_bStopAnime; }
	void				CreateCrack(const char* _boneName);
	void				AllCrackTrue();
	void				CrackCheck();

	void				CreateBoom();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;

private:
	CGameObject*			m_pCutSceneCam = nullptr;
	CAnimation*				m_pCutSceneAnimation = nullptr;
	CUI*					m_pUI = nullptr;
	class CHodHpBar*		m_pHpBar = nullptr;
	class CCamera_Free*		m_pCameraFree = nullptr;

	class CCharater*		m_pAru = nullptr;
	class CCharater*		m_pHaruka = nullptr;
	class CCharater*		m_pZunko = nullptr;



	HOD_STATE				m_eState = READY;
	_bool					m_bCutSceneOnce = true;
	_bool					m_bUIOnce = true;
	_float					m_fgDamageEffcet = 0.f;
	_bool					m_bDie = false;
	_bool					m_bStopAnime = false;

	_float					m_fTime = 0.f;
	_float					m_fAtkDelay = 10.f;
	_int					m_iExCount = 0;

	_bool					m_bFirstCrack[7];
	_bool					m_bSecCrack[7];
	_float					m_CreateCount = 0.f;
	_bool					m_bUpDown = true;

	_bool					m_bNormalAtkOnce = true;
	_bool					m_bCharaPointerGetOnce = true;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CHod* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END