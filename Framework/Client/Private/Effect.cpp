#include "stdafx.h"
#include "Effect.h"
#include "GameObject.h"
#include "Texture.h"
#include "Renderer.h"
#include "GameInstance.h"
#include "Component.h"

CEffect::CEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect & Prototype)
	: CGameObject(Prototype)
	, m_FXDesc(Prototype.m_FXDesc)
	, m_pShader(Prototype.m_pShader)
	, m_pTexture(Prototype.m_pTexture)
	, m_pVIBuffer(Prototype.m_pVIBuffer)
	, m_pRenderer(Prototype.m_pRenderer)
{
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pVIBuffer);
	Safe_AddRef(m_pRenderer);
}

HRESULT CEffect::Initialize_Prototype()
{
	if (FAILED(SetUp_Component()))
		return E_FAIL;
	
	ZeroMemory(&m_FXDesc, sizeof(FX_DESC));

	return S_OK;
}

HRESULT CEffect::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	//GameObject 에서 디스크립션 초기화해서 먼저 이거 해줘야함.
	if (FAILED(CGameObject::Initialize(&TransformDesc)))
		return E_FAIL;

	m_bEnable = true;

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	m_fLifeTime += fTimeDelta;

	CGameInstance*	pInstacne = GET_INSTANCE(CGameInstance);

	//뷰행렬 가져옴.
	_matrix matViewInv = XMMatrixInverse(nullptr, pInstacne->Get_Transform(CPipeLine::D3DTS_VIEW));

	_float3	scale = m_pTransformCom->Get_Scaled();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, matViewInv.r[0] * scale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, matViewInv.r[1] * scale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, matViewInv.r[2] * scale.z);

	RELEASE_INSTANCE(CGameInstance);
}

void CEffect::LateTick(_float fTimeDelta)
{
	if (m_bEnable)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CEffect::Render()
{
	if (nullptr == m_pShader)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShader->Begin(0);
	m_pVIBuffer->Render();
	return S_OK;
}

void CEffect::Reset()
{
	m_fLifeTime = 0.f;
}

HRESULT CEffect::SetUp_Component()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexEffect"), TEXT("Com_Shader"), (CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::SetUp_ShaderResource()
{
	if (nullptr == m_pShader)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



void CEffect::Free()
{
	__super::Free();
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);

}