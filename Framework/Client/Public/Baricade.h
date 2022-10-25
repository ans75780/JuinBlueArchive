#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

class CBaricade final : public CGameObject
{
private:
	CBaricade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaricade(const CBaricade& rhs);
	virtual ~CBaricade() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;




private:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_ShaderResource();


public:
	static CBaricade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
