#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CArona_Cam final : public CGameObject
{
public:
	CArona_Cam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CArona_Cam(const CArona_Cam& rhs);
	virtual ~CArona_Cam() = default;

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


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CArona_Cam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END