#include "stdafx.h"
#include "..\Public\Charater.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

#include "Hod_CutScene_Cam.h"
#include "UI_Fade_White.h"
#include "Camera_Free.h"

#include "Hod.h"
#include <random>

CCharater::CCharater(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CCharater::CCharater(const CCharater & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCharater::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharater::Initialize(void * pArg)
{
	if (nullptr == pArg)
	{
		MSG_BOX("머델이름넣어줘");
		return E_FAIL;
	}
	
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components((_tchar*)pArg)))
		return E_FAIL;

	m_desc.fMaxHp = 100.f;
	m_desc.fHp = 100.f;
	m_desc.fDamage = 5.f;

	static _float SetPos = 1.5f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&_float4(-4.6f, 0.f, SetPos, 1.f)));
	SetPos -= 1.5f;
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));
	m_pModelCom->Set_CurrentAnimation(0);
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_CurrentLevelID() == LEVEL::LEVEL_SHOP)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, L"Layer_HpBar", L"Prototype_HpBar", this, ((CGameObject**)&m_pHpBar))))
			return E_FAIL;
	}

	m_pHod = static_cast<CHod*>(pGameInstance->Get_GameObjects(LEVEL_SHOP, TEXT("Layer_Hod")).front());	//호드받아오기
	m_pCamera = static_cast<CCamera_Free*>(pGameInstance->Get_GameObjects(LEVEL_SHOP, TEXT("Layer_Camera")).front());

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

void CCharater::Tick(_float fTimeDelta)
{

}

void CCharater::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CCharater::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; i++)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			continue;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))

			m_pShaderCom->Begin(1);
		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}

	return S_OK;
}

CAnimation * CCharater::Get_Animation(const char * pAnimationName)
{
	CAnimation*	m_pAnimation = nullptr;

	m_pAnimation = m_pModelCom->Get_AnimationFromName(pAnimationName);

	return m_pAnimation;
}

void CCharater::Damage_Chara(_float _Damage)
{
	m_desc.fHp -= _Damage;
	
	if(m_desc.fHp <= 0.f)
		m_eState = DEAD;
}

_int CCharater::Get_Random3()//1,2,3
{
	random_device _rd;
	mt19937_64 _random(_rd());
	uniform_int_distribution<__int64> _range(1, 3);
	_int RandChara = (_uint)(_range(_random));

	return RandChara;
}

HRESULT CCharater::SetUp_Components(_tchar * ModelName)
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

HRESULT CCharater::SetUp_ShaderResource()
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

	_float value = 0.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_DamageEffcet", &value, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CCharater * CCharater::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCharater*		pInstance = new CCharater(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCharater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCharater::Clone(void * pArg)
{
	CCharater*		pInstance = new CCharater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCharater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharater::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}