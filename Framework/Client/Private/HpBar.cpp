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
	m_vOffset = { 0.f,1.1f,0.f };
	m_vSize = { 1.f,0.1f,1.f };

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

	//빌보드 만드는법
	/*
	빌보드란? 항상 나를 바라보는 오브젝트.
	원래는 카메라위치 - 오브젝트 위치 = 방향벡터를 통해 회전시킨다.

	근데, 항상 카메라를 바라보는 행렬이 있잖아?.
	뷰 행렬을 생각해보자.
	뷰 행렬이란 것이 무엇인가?
	카메라를 Z양의 방향의 원점으로 돌리고, 원점으로 돌린 값을 저장한 행렬이 아닌가?
	그러면 이의 역행렬은 카메라를 바라보는 방향이라는 것이 아닌가?

	뷰 행렬의 역행렬.
	룩 벡터에 이거 그냥 곱해주면 되지 않나.
	*/

	CGameInstance*	pInstacne = GET_INSTANCE(CGameInstance);

	//뷰행렬 가져옴.
	_matrix matViewInv = XMMatrixInverse(nullptr, pInstacne->Get_Transform(CPipeLine::D3DTS_VIEW));
	
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, matViewInv.r[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, matViewInv.r[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, matViewInv.r[2]);
	
	RELEASE_INSTANCE(CGameInstance);

	m_pTransformCom->Set_Scaled(m_vSize);

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
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject * CHpBar::Clone(void * pArg)
{
	CHpBar*	pInstance = new CHpBar(*this);


	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
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
