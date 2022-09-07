#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CTestObject_Model_Haruka final : public CGameObject
{
public:
	CTestObject_Model_Haruka(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTestObject_Model_Haruka(const CTestObject_Model_Haruka& rhs);
	virtual ~CTestObject_Model_Haruka() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTexture*				m_pMouthCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;

private:
	_uint					m_iAnimIndex = 0;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	

public:
	static CTestObject_Model_Haruka* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
