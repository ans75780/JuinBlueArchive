#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CAnimation;
END

BEGIN(Client)

class CHod_CutScene_Cam final : public CGameObject
{
public:
	CHod_CutScene_Cam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHod_CutScene_Cam(const CHod_CutScene_Cam& rhs);
	virtual ~CHod_CutScene_Cam() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	class CAnimation*	Get_Animation(const char* pAnimationName);
	CModel*				Get_Model() { return m_pModelCom; }

	void				BattlePosSet();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;

private:
	CAnimation*				m_pCutSceneAnimation = nullptr;
	CGameObject*			m_pCamera = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CHod_CutScene_Cam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END