#include "stdafx.h"
#include "..\Public\Memorial_Haruka_Start.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"

CMemorial_Haruka_Start::CMemorial_Haruka_Start(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

HRESULT CMemorial_Haruka_Start::Initialize(void * pArg)
{
	XMStoreFloat4x4(&m_UIMatProj, 
		XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 3.f)));

	CTransform::TRANSFORMDESC		TransformDesc;

	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(0.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_fPos.x = 0.f;
	m_fPos.y = 0.f;
	m_fPos.z = 0.f;

	m_fSize.x = 1280.f;
	m_fSize.y = 720.f;
	m_fSize.z = 1.f;


	if (nullptr == m_pTransformCom)
		return E_FAIL;

	_vector vPos;
	m_pTransformCom->Set_Scaled(m_fSize);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		vPos = XMVectorSet(m_fPos.x, m_fPos.y, 0.9f, 1.f));

	XMStoreFloat3(&m_fPos, vPos);

	return S_OK;
}

HRESULT CMemorial_Haruka_Start::Initialization()
{
	return S_OK;
}

void CMemorial_Haruka_Start::Tick(_float fTimeDelta)
{
	if (KEY(LBUTTON, HOLD))
	{
		m_fFrame = 513.f;
	}

	m_fFrame += 514.f * (fTimeDelta * 0.05f);

	if (m_fFrame >= 514.f)
	{
		m_fFrame = 513.f;
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->Get_GameObjects(LEVEL_LOBBY, TEXT("Layer_Haruka_Memorial")).front()->Set_Delete(true);

		if (m_bOnce)
		{
			pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Haruka_Memorial"), TEXT("Prototype_GameObject_Memorial_Haruka_Idle"));
			m_bOnce = false;
		}

		RELEASE_INSTANCE(CGameInstance)
	}
}

void CMemorial_Haruka_Start::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, (CGameObject*)this);
}

HRESULT CMemorial_Haruka_Start::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;
	
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMemorial_Haruka_Start::SetUp_Components()
{
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Memorial_Haruka_Start"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMemorial_Haruka_Start::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;

	return S_OK;
}

CMemorial_Haruka_Start * CMemorial_Haruka_Start::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMemorial_Haruka_Start*		pInstance = new CMemorial_Haruka_Start(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CMemorial_Haruka_Start");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMemorial_Haruka_Start::Clone(void * pArg)
{
	CMemorial_Haruka_Start*		pInstance = new CMemorial_Haruka_Start(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMemorial_Haruka_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMemorial_Haruka_Start::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
