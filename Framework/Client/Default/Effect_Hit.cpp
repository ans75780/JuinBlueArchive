#include "stdafx.h"
#include "Effect_Hit.h"
#include "..\Public\Effect_Hit.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"


CEffect_Hit::CEffect_Hit(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CEffect(pDevice, pContext)
{

}

CEffect_Hit::CEffect_Hit(const CEffect_Hit & Prototype)
	: CEffect(Prototype)
{

}

HRESULT CEffect_Hit::Initialize_Prototype()
{
	if (FAILED(CEffect::Initialize_Prototype()))
		return E_FAIL;

	m_FXDesc.bLoop = false;
	m_FXDesc.fDuration = 0.3f;
	m_FXDesc.fStart_Size = 1.6f;
	m_FXDesc.fStart_Speed = 1.f;
	m_FXDesc.fEnd_Size = 0.1f;

	return S_OK;
}

HRESULT CEffect_Hit::Initialize(void * pArg)
{
	if (FAILED(CEffect::Initialize(pArg)))
		return E_FAIL;
	Reset();
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 3.f, 0.f, 1.f));
	
	m_fImgChangeRatio = 0.25;
	return S_OK;
}

void CEffect_Hit::Tick(_float fTimeDelta)
{
	CEffect::Tick(fTimeDelta);


	_vector vector;

	_float	m_fRatio = min(m_fLifeTime / m_FXDesc.fDuration,1.f);

	m_iTextureIndex = (_uint)(m_fRatio / m_fImgChangeRatio);

	_float3	 TransformScale = m_pTransformCom->Get_Scaled();
	
	_vector	EndVector = XMVectorSet(m_FXDesc.fEnd_Size, m_FXDesc.fEnd_Size, m_FXDesc.fEnd_Size, 1.f);

	vector = XMVectorLerp(XMLoadFloat3(&TransformScale), EndVector, m_fRatio);
	
	_float3 vScale;
	_matrix	mat = m_pTransformCom->Get_WorldMatrix();

	XMStoreFloat3(&vScale, vector);
	m_pTransformCom->Set_Scaled(vScale);

	if (m_fLifeTime > m_FXDesc.fDuration)
	{
		if (m_FXDesc.bLoop)
		{
			Reset();
		}
		else
		{
			m_bDelete = true;
		}
	}
}

void CEffect_Hit::LateTick(_float fTimeDelta)
{
	CEffect::LateTick(fTimeDelta);

}

HRESULT CEffect_Hit::Render()
{
	if (FAILED(CEffect::Render()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Hit::Reset()
{

	m_fLifeTime = 0.f;
	m_iTextureIndex = 0;
	m_pTransformCom->Set_Scaled(_float3(m_FXDesc.fStart_Size, m_FXDesc.fStart_Size, m_FXDesc.fStart_Size));


}

HRESULT CEffect_Hit::SetUp_Component()
{
	if (FAILED(CEffect::SetUp_Component()))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_FX_Hit_0", L"Com_Texture", (CComponent**)&m_pTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Hit::SetUp_ShaderResource()
{
	if (FAILED(CEffect::SetUp_ShaderResource()))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_Color", &_float4(1.f,0.8f,0.f,1.f), sizeof(_float4))))
		return E_FAIL;
	if(FAILED(m_pTexture->Set_ShaderResourceView(m_pShader, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CEffect_Hit * CEffect_Hit::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	
	CEffect_Hit*		pInstance = new CEffect_Hit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Hit");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Hit::Clone(void * pArg)
{
	CEffect_Hit*		pInstance = new CEffect_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Hit::Free()
{
	CEffect::Free();
}
