#include "stdafx.h"
#include "..\Public\Chara_Aru.h"

#include "GameInstance.h"
#include "BoneNode.h"
#include "PipeLine.h"
#include "MeshContainer.h"
#include "Animation.h"

#include "Hod_CutScene_Cam.h"
#include "UI_Fade_White.h"

CChara_Aru::CChara_Aru(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharater(pDevice, pContext)
{
}

CChara_Aru::CChara_Aru(const CChara_Aru & rhs)
	: CCharater(rhs)
{
}

HRESULT CChara_Aru::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChara_Aru::Initialize(void * pArg)
{
	__super::Initialize(TEXT("Prototype_Component_Model_Aru_Original"));

	return S_OK;
}

void CChara_Aru::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CChara_Aru::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CChara_Aru::Render()
{
	__super::Render();

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
}