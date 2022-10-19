#include "stdafx.h"
#include "..\Public\Arona.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

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
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimation(0);

	//_vector _vPos =	m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	//_float4	 _fPos;
	//XMStoreFloat4(&_fPos, _vPos);
	//_fPos.y -= 0.5f;

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&_fPos));
	return S_OK;
}

void CArona::Tick(_float fTimeDelta)
{
	//m_pModelCom->Play_Animation(fTimeDelta * 0.5f);
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

	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	if (nullptr == m_pAnimation && m_bRenderOnce)
	{
		m_pAnimation = m_pModelCom->Get_AnimationFromName("ARONA|Arona_Original_Gacha_Anim|Base Layer");
		
		if(nullptr != m_pAnimation)
			m_bRenderOnce = false;
	}

	if (m_bRenderOnce)
		return S_OK;

	/*
	0~46    Emo02 Off, Face Off				0 = Facebase	3 = Face  6 = EyeBrow	9 = FaceEmo02
	42~60  Emo03 Off, Face Off				1 = Skirt		4 = Face  7 = Sack		10 = FaceEmo03
	60~128  Base, Emo02, Emo03 Off			2 = Hair		5 = Body  8 = Halo		11 = Tablet_01
	*/

	for (_uint i = 0; i < iNumMeshContainers; ++i)	//이게 모델들여러개 그리는것임 이걸로 아로나 키프레임마다 그러야할것들을 알아내서 해볼것!
	{

		if (42 > m_pAnimation->Get_TimeAcc())
		{
			if (i == 3 || i == 4 || i == 9)
				continue;
		}
		else if (60 > m_pAnimation->Get_TimeAcc())
		{
			if (i == 3 || i == 4 || i == 10)
				continue;
		}
		else if (60 <= m_pAnimation->Get_TimeAcc())
		{
			if (i == 0 || i == 9 || i == 10)
				continue;
		}

		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}




	return S_OK;
}

CAnimation * CArona::Get_Animation(const char * pAnimationName)
{
	CAnimation*	m_pAnimation = nullptr;

	m_pAnimation = m_pModelCom->Get_AnimationFromName(pAnimationName);

	return m_pAnimation;
}

HRESULT CArona::SetUp_Components()				//Com_Transform
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