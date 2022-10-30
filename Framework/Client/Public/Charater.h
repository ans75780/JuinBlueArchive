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

protected:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;

protected:
	HRESULT SetUp_Components(_tchar* ModelName);
	HRESULT SetUp_ShaderResource();

public:
	static CCharater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END