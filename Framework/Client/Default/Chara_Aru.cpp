#include "stdafx.h"
#include "..\Public\Chara_

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

#include "Hod_CutScene_Cam.h"
#include "UI_Fade_White.h"

CChara_Aru::CChara_Aru(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CChara_Aru::CChara_Aru(const CChara_Aru & rhs)
	: CGameObject(rhs)
{
}

HRESULT CChara_Aru::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChara_Aru::Initialize(void * pArg)
{
	if (nullptr == pArg)
	{
		MSG_BOX("¸Óµ¨ÀÌ¸§³Ö¾îÁà");
		return E_FAIL;
	}

	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components((_tchar*)pArg)))
		return E_FAIL;


	return S_OK;
}

void CChara_Aru::Tick(_float fTimeDelta)
{
}

void CChara_Aru::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CChara_Aru::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}
	return S_OK;
}

CAnimation * CChara_Aru::Get_Animation(const char * pAnimationName)
{
	CAnimation*	m_pAnimation = nullptr;

	m_pAnimation = m_pModelCom->Get_AnimationFromName(pAnimationName);

	return m_pAnimation;
}

HRESULT CChara_Aru::SetUp_Components(_tchar * ModelName)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, ModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChara_Aru::SetUp_ShaderResource()
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

CChara_Aru * CChara_Aru::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CChara_Aru*		pInstance = new CChara_Aru(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChara_Aru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChara_Aru::Clone(void * pArg)
{
	CChara_Aru*		pInstance = new CChara_Aru(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChara_Aru");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChara_Aru::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}