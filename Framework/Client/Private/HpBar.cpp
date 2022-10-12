#include "stdafx.h"
#include "GameObject.h"
#include "HpBar.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "GameInstance.h"



CHpBar::CHpBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CHpBar::CHpBar(const CHpBar & Prototype)
	:CGameObject(Prototype),
	m_vOffset(Prototype.m_vOffset),
	m_vSize(Prototype.m_vSize)
{

}

HRESULT CHpBar::Start_Level()
{
	return S_OK;
}

HRESULT CHpBar::Initialize_Prototype()
{
	m_vOffset = { 0.f,1.f,0.f };
	m_vSize = { 3.f,0.5f,1.f };

	return S_OK;
}

HRESULT CHpBar::Initialize(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_pTarget = (CGameObject*)pArg;	
	UNIT_TYPE eUnitType = m_pTarget->Get_Desc().eType;

	switch (eUnitType)
	{
	case Engine::UNIT_TYPE_STUDENT:
		m_vColor = { 0.f, 1.f, 0.f, 1.f };
		break;
	case Engine::UNIT_TYPE_ENEMY:
		m_vColor = { 1.f, 0.f, 0.f, 1.f };
		break;
	case Engine::UNIT_TYPE_BOSS:
		break;
	case Engine::UNIT_TYPE_PROP:
		break;
	case Engine::UNIT_TYPE_END:
		break;
	default:
		break;
	}


	return S_OK;
}

void CHpBar::Tick(_float fTimeDelta)
{
	//매 틱마다 오프셋만큼 조정
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION,
		m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) + XMLoadFloat3(&m_vOffset));

	m_fRatio = m_pTarget->Get_Desc().fHp / m_pTarget->Get_Desc().fMaxHp;

}

void CHpBar::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_ALPHABLEND, this);
}

HRESULT CHpBar::Render()
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

HRESULT CHpBar::SetUp_Component()
{
	//UI들은 기본적으로 렌더러를 달고 생성된다.	
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxHpBar"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHpBar::SetUp_ShaderResource()
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_fRatio", &m_fRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

CHpBar * CHpBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHpBar*	pInstance = new CHpBar(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Delete(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject * CHpBar::Clone(void * pArg)
{
	CHpBar*	pInstance = new CHpBar(*this);


	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Delete(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHpBar::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);


}
