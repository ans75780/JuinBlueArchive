#include "stdafx.h"
#include "..\Public\Hod_CutScene_Cam.h"

#include "GameInstance.h"

#include "Animation.h"
#include "Camera.h"
#include "BoneNode.h"

CHod_CutScene_Cam::CHod_CutScene_Cam(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CHod_CutScene_Cam::CHod_CutScene_Cam(const CHod_CutScene_Cam & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHod_CutScene_Cam::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHod_CutScene_Cam::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//_vector _vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	//_float4	 _fPos;
	//XMStoreFloat4(&_fPos, _vPos);
	//_fPos.x += 12.f;
	//_fPos.y += 1.f;
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&_fPos));
	//m_pTransformCom->Rotation(XMVectorSet(0, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));

	m_pModelCom->Set_CurrentAnimation(0);
	m_pCutSceneAnimation = m_pModelCom->Get_AnimationFromName("HOD_Original_BattleReady_Cam");
	

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pCamera = pGameInstance->Get_GameObjects(LEVEL_SHOP, TEXT("Layer_Camera")).front();

	RELEASE_INSTANCE(CGameInstance)


	return S_OK;
}

void CHod_CutScene_Cam::Tick(_float fTimeDelta)
{
	static _bool test = false;
	
	if (KEY(TAB, TAP))
	{
		test = true;
	}
	if (test)
		return;

	if (!m_pCutSceneAnimation->IsFinished())
	{
		if (nullptr != m_pCamera)
		{
			_matrix vMatrix = m_pModelCom->Find_Bone("Camera001")->Get_CombinedMatrix();
			_matrix vTargetViewMatrix = m_pModelCom->Find_Bone("Camera001.Target")->Get_CombinedMatrix();

			m_pCamera->Get_Transform()->Set_State(CTransform::STATE_RIGHT, vMatrix.r[0]);
			m_pCamera->Get_Transform()->Set_State(CTransform::STATE_UP, vMatrix.r[1]);
			m_pCamera->Get_Transform()->Set_State(CTransform::STATE_LOOK, vMatrix.r[2]);

			m_pCamera->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vMatrix.r[3]);


			m_pCamera->Get_Transform()->LookAt(vTargetViewMatrix.r[3]);
		}
	}
}

void CHod_CutScene_Cam::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CHod_CutScene_Cam::Render()
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

CAnimation * CHod_CutScene_Cam::Get_Animation(const char * pAnimationName)
{
	CAnimation*	m_pAnimation = nullptr;

	m_pAnimation = m_pModelCom->Get_AnimationFromName(pAnimationName);

	return m_pAnimation;
}

void CHod_CutScene_Cam::BattlePosSet()
{
	_float4 _Right = { 0.95f, 0.f, -0.31f, 0.f };
	_float4 _Up = { 0.17f, 0.83f, 0.54f, 0.f };
	_float4 _Look = { 0.25f, -0.56f, 0.78f, 0.f };
	_float4 _Trans = { 2.6f, 5.68f, -5.16f, 1.f };

	m_pCamera->Get_Transform()->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&_Right));
	m_pCamera->Get_Transform()->Set_State(CTransform::STATE_UP, XMLoadFloat4(&_Up));
	m_pCamera->Get_Transform()->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&_Look));
	m_pCamera->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&_Trans));

}

HRESULT CHod_CutScene_Cam::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_SHOP, TEXT("Prototype_Component_Model_Hod_CutScene_Cam"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHod_CutScene_Cam::SetUp_ShaderResource()
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

CHod_CutScene_Cam * CHod_CutScene_Cam::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHod_CutScene_Cam*		pInstance = new CHod_CutScene_Cam(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHod_CutScene_Cam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHod_CutScene_Cam::Clone(void * pArg)
{
	CHod_CutScene_Cam*		pInstance = new CHod_CutScene_Cam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHod_CutScene_Cam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHod_CutScene_Cam::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}