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

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CAnimation*				m_pAnimation = nullptr;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CHod* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_bool m_bRenderOnce = true;

};

END