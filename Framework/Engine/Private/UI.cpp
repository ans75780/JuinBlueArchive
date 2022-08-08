#include "..\Public\UI.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
_float4x4	CUI::g_UIMatProj;


CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext),m_bMouseClicked(false), m_bMouseOver(false)
{

}

HRESULT CUI::Initialize(void * arg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	if (arg == nullptr)
	{
		TransformDesc.fSpeedPerSec = 0.f;
		TransformDesc.fRotationPerSec = XMConvertToRadians(0.0f);
		if (FAILED(__super::Initialize((&TransformDesc))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Initialize((&TransformDesc))))
			return E_FAIL;
	}
	//UI들은 기본적으로 렌더러를 달고 생성된다.	
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{

}

void CUI::LateTick(_float fTimeDelta)
{

}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}

void CUI::OnLButtonDown()
{
}

void CUI::OnLButtonUp()
{
}

void CUI::OnLButtonClicked()
{
}

HRESULT CUI::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

void CUI::OnEvent(_uint iEventNum)
{
}

void CUI::Compute_Transform()
{
	if (nullptr == m_pTransformCom)
		return;
	
	m_pTransformCom->Set_Scaled(m_fSize);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, 
	XMVectorSet(m_fPos.x, m_fPos.y, ((_float)m_eUIType) / (_float)(UI_TYPE::UI_END), 1.f)
		);
}


HRESULT CUI::LoadUIImage(const _tchar * TextureTag, _uint iLevel)
{
	if (FAILED(__super::Add_Component(iLevel, TextureTag, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	return S_OK;
}



