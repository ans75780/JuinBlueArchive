#include "stdafx.h"
#include "..\Public\Hod.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

#include "Hod_CutScene_Cam.h"
#include "UI_Fade_White.h"
#include "HodHpBar.h"

CHod::CHod(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CHod::CHod(const CHod & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHod::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHod::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_desc.fMaxHp = 600.f;
	m_desc.fHp = 600.f;
	m_desc.fDamage = 5.f;
	lstrcpy(m_desc.sz_Name, TEXT("HOD"));
	

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP,
		TEXT("Layer_CutSceneStage_"), TEXT("Prototype_GameObject_Stage_Hod"), TEXT("Prototype_Component_Model_Stage_Hod_CutScene"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP, TEXT("Layer_CutScene_Cam"), TEXT("Prototype_GameObject_Hod_CutScene_Cam"))))
		return E_FAIL;

	m_pCutSceneCam = pGameInstance->Get_GameObjects(LEVEL_SHOP, TEXT("Layer_CutScene_Cam")).front();

	RELEASE_INSTANCE(CGameInstance);

	m_pModelCom->Set_CurrentAnimation(8);
	m_pCutSceneAnimation = m_pModelCom->Get_AnimationFromName("HOD_Original_BattleReady");
	m_pCutSceneAnimation->Play();


	return S_OK;
}

void CHod::Tick(_float fTimeDelta)
{
	StartSet(fTimeDelta);	//시작이벤트가끝나면 더이상 실행되지않음
	
	if (0.f < m_fgDamageEffcet)
	{
		m_fgDamageEffcet -= 0.1f;
	}
}

void CHod::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CHod::Render()
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
		//if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	continue;
		//if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		//	m_pShaderCom->Begin(1);

		//m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			continue;
		m_pShaderCom->Begin(1);
		m_pModelCom->Render(i, m_pShaderCom, "g_Bones");
	}

	return S_OK;

}

CAnimation * CHod::Get_Animation(const char * pAnimationName)
{
	CAnimation*	m_pAnimation = nullptr;

	m_pAnimation = m_pModelCom->Get_AnimationFromName(pAnimationName);

	return m_pAnimation;
}

void CHod::CreateUI_Hod()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pUI = CUI_Fade_White::Create(m_pDevice, m_pContext);

	if (FAILED(m_pUI->LoadUIImage(TEXT("Prototype_Component_Texture_hod_Title"), LEVEL_SHOP)))
	{
		MSG_BOX("UI 호드 이미지 생성 실패");
		return;
	}

	m_pUI->Set_UIName(TEXT("fade_White"));
	m_pUI->Set_UIType(UI_TYPE::UI_DIALOG_BUTTON);
	m_pUI->Set_Size(_float3(1280.f, 107.f, 1.f));
	m_pUI->Set_Pos(_float3(0.f, -250.f, 0.f));
	m_pUI->Set_ThrowPos(_float2(0.f, 0.f));
	m_pUI->Set_UILevel(LEVEL_SHOP);
	m_pUI->Initialization();

	static_cast<CUI_Fade_White*>(m_pUI)->Set_Alpha(true);
	static_cast<CUI_Fade_White*>(m_pUI)->Set_AlphaValue(0.f);

	if (FAILED(pGameInstance->Add_UI(LEVEL_SHOP, m_pUI)))	//받아온레벨에다 생성
	{
		MSG_BOX("UI호드 생성 실패");
		return;
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CHod::BattlePosSet()
{
	_vector _vPos =	m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float4	 _fPos;
	XMStoreFloat4(&_fPos, _vPos);
	_fPos.x += 12.f;
	_fPos.y += 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&_fPos));
	m_pTransformCom->Rotation(XMVectorSet(0, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));

	static_cast<CHod_CutScene_Cam*>(m_pCutSceneCam)->BattlePosSet();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Get_GameObjects(LEVEL_SHOP, TEXT("Layer_CutSceneStage_")).front()->Free();

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP,
		TEXT("Layer_Stage"), TEXT("Prototype_GameObject_Stage_Hod"), TEXT("Prototype_Component_Model_Stage_Hod"))))
	{
		MSG_BOX("배틀지형생성실패");
		return;
	}

	if (pGameInstance->Get_CurrentLevelID() == LEVEL::LEVEL_SHOP)
	{
		m_pHpBar = CHodHpBar::Create(m_pDevice, m_pContext);
		m_pHpBar->Set_UILevel(LEVEL_SHOP);
		if (FAILED(pGameInstance->Add_UI(LEVEL_SHOP, m_pHpBar)))
		{
			MSG_BOX("실패호두체력바");
			return;
		}
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SHOP,
		TEXT("Layer_Chara_Aru"), TEXT("Prototype_GameObject_Character_Aru"))))
	{
		MSG_BOX("아루생성실패");
		return;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CHod::StartSet(_float& fTimeDelta)
{
	if (m_bCutSceneOnce)
	{
		if (m_pCutSceneAnimation->IsFinished())
		{
			m_bCutSceneOnce = false;
			m_pUI->Dead();
			BattlePosSet();
			m_pModelCom->Set_CurrentAnimation(6);
			return;
		}
		m_pModelCom->Play_Animation(fTimeDelta * 0.7f);
		static_cast<CHod_CutScene_Cam*>(m_pCutSceneCam)->Get_Model()->Play_Animation(fTimeDelta * 0.7f);

		_float _frame = static_cast<CHod_CutScene_Cam*>(m_pCutSceneCam)->Get_Model()->Get_AnimationFromName("HOD_Original_BattleReady_Cam")->Get_TimeAcc();
		_tchar temp[MAX_PATH];
		_stprintf_s(temp, MAX_PATH, L"frame = %.2f", _frame);
		SetWindowText(g_hWnd, temp);

		if (KEY(TAB, TAP))
		{
			m_bCutSceneOnce = false;
			BattlePosSet();
			m_pModelCom->Set_CurrentAnimation(6);
		}

		if (10.f < _frame && m_bUIOnce)
		{
			CreateUI_Hod();
			m_bUIOnce = false;
		}
		return;
	}
	auto DeathAni = m_pModelCom->Get_AnimationFromName("HOD_Original_Vital_Death");
	if (4.7f < DeathAni->Get_TimeAcc())
	{
		DeathAni->Pause();
		return;
	}
	m_pModelCom->Play_Animation(fTimeDelta);

}

void CHod::DamageAction(_float _Damage)
{
	if (m_bDie)
		return;

	m_desc.fHp -= _Damage;
	m_pHpBar->Minus_WidthSize(_Damage);
	
	if (0.f >= m_desc.fHp)
	{
		m_pModelCom->Set_CurrentAnimation(10);
		m_bDie = true;
	}

	m_fgDamageEffcet = 1.f;
}

HRESULT CHod::SetUp_Components()				//Com_Transform
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_SHOP, TEXT("Prototype_Component_Model_Hod"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHod::SetUp_ShaderResource()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_DamageEffcet", &m_fgDamageEffcet, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;


}

CHod * CHod::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHod*		pInstance = new CHod(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHod");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHod::Clone(void * pArg)
{
	CHod*		pInstance = new CHod(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHod");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHod::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}