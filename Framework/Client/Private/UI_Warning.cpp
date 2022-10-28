#include "stdafx.h"
#include "..\Public\UI_Warning.h"
#include "UI.h"
#include "UI_Default.h"
#include "Engine_Defines.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Collider.h"
#include "Actor.h"
#include "Transform.h"
#include "Layer.h"
CUI_Warning::CUI_Warning(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

HRESULT CUI_Warning::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Warning::Initialize(void * pArg)
{
	CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);


	m_pWarning = CUI_Default::Create(m_pDevice, m_pContext);

	m_pWarning->Set_Pos(_float3(0.f, 0.f, 0.f));
	m_pWarning->Set_Size(_float3(512.f, 256.f, 1.f));
	m_pWarning->Set_UIType(UI_TYPE::UI_POST);
	m_pWarning->LoadUIImage(L"Prototype_Component_Texture_FX_TEX_Warning_Font_01");
	m_pWarning->SetPath(2);

	pInstance->Add_UI(LEVEL_GAMEPLAY, m_pWarning);


	m_pWarning->Set_Enable(false);

	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
		return E_FAIL;

	CCollider::COLLIDERDESC	ColliderDesc;

	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pCollider, &ColliderDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);


	m_bWarning = false;

	m_fAlphaStart = 0.7f;
	m_fAlphaEnd = 1.f;
	m_fAlphaTickForAmount = 0.2f;
	m_fAlphaCurrent = m_fAlphaStart;


	m_fWarningTime = 2.5f;

	return S_OK;
}

void CUI_Warning::Tick(_float fTimeDelta)
{
	if (m_bEnable == false)
		return;
	m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_bWarning == true)
	{
		if (m_fAlphaCurrent <= m_fAlphaStart)
		{
			m_fAlphaTickForAmount *= -1.f;
		}
		else if (m_fAlphaCurrent >= m_fAlphaEnd)
		{
			m_fAlphaTickForAmount *= -1.f;
		}
		if (m_fAlphaCurrent < 0.f)
			m_fAlphaCurrent = m_fAlphaStart;

		else if (m_fAlphaCurrent > 1.f)
			m_fAlphaCurrent = m_fAlphaEnd;


		m_fAlphaCurrent += m_fAlphaTickForAmount * fTimeDelta;

		m_fWarningTick += fTimeDelta;

		if (m_fWarningTime <= m_fWarningTick)
		{
			m_bEnable = false;
			m_pWarning->Set_Enable(false);
			m_pRenderer->Reset_DeferredColor();
		}
	}
	else
	{
		//CollisionToPlayer;
		CGameInstance*			pInstance = GET_INSTANCE(CGameInstance);

		//학생 불러오기
		list<CGameObject*>	m_Actors;

		map<const _tchar*, CLayer*> layer = pInstance->Get_Layer(LEVEL_GAMEPLAY);
		//캐릭터 리스트 가져오기.
		m_Actors = find_if(layer.begin(), layer.end(), CTag_Finder(TEXT("Layer_Student")))->second->Get_GameObjects();


		for (auto elem : m_Actors)
		{
			if (m_pCollider->Collision((CCollider*)elem->Get_Component(L"Com_BodyCollider")))
			{
				int a = 10;
				m_pWarning->Set_Enable(true);
				m_bWarning = true;
				m_pRenderer->Set_DeferredColor(_float4(0.9f, 0.9f, 0.9f, 1.f));
			}
		}
		RELEASE_INSTANCE(CGameInstance);
	}
	
}

void CUI_Warning::LateTick(_float fTimeDelta)
{
	
	CShader*	pShader=  (CShader*)m_pWarning->Get_Component(L"Com_Shader");
	pShader->Set_RawValue("g_Color", &_float4(1.f, 0.7f, 0.f, 1.f), sizeof(_float4));
	pShader->Set_RawValue("g_AlphaColor", &_float4(m_fAlphaCurrent, m_fAlphaCurrent, m_fAlphaCurrent, 1.f), sizeof(_float4));

}

CUI_Warning * CUI_Warning::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Warning*	pInstance = new CUI_Warning(pDevice, pContext);


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		return nullptr;
	}


	return pInstance;
}

void CUI_Warning::Free()
{

}

CGameObject * CUI_Warning::Clone(void * pArg)
{
	CUI_Warning*		pInstance = new CUI_Warning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Warning");
		Safe_Release(pInstance);
	}
	return pInstance;
};
