#include "stdafx.h"
#include "..\Public\Effect_Flare.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"

#include "Camera_Free.h"
#include "Charater.h"
#include "Hod.h"
#include <random>


CEffect_Flare::CEffect_Flare(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

HRESULT CEffect_Flare::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = 30.f;

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_tBulletDesc.CreatePos = (*(CCharater::BulletDesc*)pArg).CreatePos;
		m_tBulletDesc.TargetPos = (*(CCharater::BulletDesc*)pArg).TargetPos;
		m_tBulletDesc.Damage = (*(CCharater::BulletDesc*)pArg).Damage;
		m_tBulletDesc.Hod = (*(CCharater::BulletDesc*)pArg).Hod;

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tBulletDesc.CreatePos);
	}


	m_pTransformCom->Set_Scaled(_float3(2.f, 2.f, 2.f));
	m_gVcolor.x = 255.f / 255.f;
	m_gVcolor.y = 255.f / 255.f;
	m_gVcolor.z = 255.f / 255.f;
	m_gVcolor.w = 255.f / 255.f;
	return S_OK;
}

void CEffect_Flare::Tick(_float fTimeDelta)
{
	if (m_tBulletDesc.Hod == nullptr || m_tBulletDesc.Hod->Get_StopAnime())
		return;

	m_pTransformCom->Chase(m_tBulletDesc.TargetPos, fTimeDelta);

	_float4 PPos;
	XMStoreFloat4(&PPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	m_fCount += fTimeDelta;

	if(m_fCount > 2.f)
	{
		if (m_bDamageOnce)
		{

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			pGameInstance->Get_Instance()->Get_SoundManager()->Play_Sound(L"SFX_Aru_Skill_EX_4.wav", 1.0f);
			_matrix _mat = XMMatrixIdentity();
			_mat *= m_pTransformCom->Get_WorldMatrix();

			_float4	xPosPush;
			XMStoreFloat4(&xPosPush, _mat.r[3]);
			xPosPush.x -= 4.f;
			xPosPush.y += 1.f;
			xPosPush.z -= 3.f;

			_mat.r[3] = XMLoadFloat4(&xPosPush);


			if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, TEXT("Layer_Effect_FlareBoom"),
				TEXT("Prototype_GameObject_Effect_FlareBoom"), &_mat)))
				return;

			CCamera_Free* m_pCameraFreeF = static_cast<CCamera_Free*>(pGameInstance->Get_GameObjects(LEVEL_SHOP, TEXT("Layer_Camera")).front());
			m_pCameraFreeF->Shake(1.f);

			RELEASE_INSTANCE(CGameInstance);
			m_tBulletDesc.Hod->DamageAction(m_tBulletDesc.Damage);
			m_bDamageOnce = false;
		}
		m_bDelete = true;
		m_bEnable = true;
		return;
	}

	m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 1.f), fTimeDelta);

}

void CEffect_Flare::LateTick(_float fTimeDelta)
{
	if (m_tBulletDesc.Hod == nullptr || m_tBulletDesc.Hod->Get_StopAnime())
		return;

	if (nullptr == m_pTransformCom ||
		nullptr == m_pRendererCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, (CGameObject*)this);
}

HRESULT CEffect_Flare::Render()
{
	if (m_tBulletDesc.Hod == nullptr || m_tBulletDesc.Hod->Get_StopAnime())
		return S_OK;

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

HRESULT CEffect_Flare::SetUp_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Flare"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Flare::SetUp_ShaderResource()
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
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_gVcolor, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_Flare * CEffect_Flare::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_Flare*		pInstance = new CEffect_Flare(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CEffect_Flare");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Flare::Clone(void * pArg)
{
	CEffect_Flare*		pInstance = new CEffect_Flare(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Flare");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Flare::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
