#include "stdafx.h"
#include "..\Public\Arona.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"

CArona::CArona(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CArona::CArona(const CArona & rhs)
	: CGameObject(rhs)
{
}

HRESULT CArona::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArona::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimation(0);

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
	_vector	vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);

	vPos -= XMVector3Normalize(vUp) * 2.f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	

	return S_OK;
}

void CArona::Tick(_float fTimeDelta)
{
	if (GetKeyState(VK_LEFT) & 0x8000)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);

	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);

	if (GetKeyState(VK_DOWN) & 0x8000)
		m_pTransformCom->Go_Backward(fTimeDelta);


	m_pModelCom->Play_Animation(fTimeDelta * 0.5f);

}

void CArona::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CArona::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;


	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		return E_FAIL;*/


		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}

#ifdef _DEBUG
	/*m_pAABBCom->Render();
	m_pOBBCom->Render();
	m_pSphereCom->Render();
	m_pNaviCom->Render();*/
#endif // _DEBUG

	return S_OK;
}

HRESULT CArona::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GACHA_PLAY, TEXT("Prototype_Component_Model_Arona_Gacha"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CArona::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CArona * CArona::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CArona*		pInstance = new CArona(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArona");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArona::Clone(void * pArg)
{
	CArona*		pInstance = new CArona(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CArona");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArona::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}
