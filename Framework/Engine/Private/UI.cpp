#include "..\Public\UI.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Texture.h"

CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
	

}

CUI::CUI(const CUI & rhs)
	:CGameObject(rhs)

{

}


void CUI::Tick(_float fTimeDelta)
{

}

void CUI::LateTick(_float fTimeDelta)
{
}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::OnLButtonDown(void * pArg)
{
}

void CUI::OnLButtonUp(void * pArg)
{
}

void CUI::OnLButtonClicked(void * pArg)
{
}

void CUI::OnEvent(_uint iEventNum)
{
}

HRESULT CUI::LoadUIImage(const _tchar * TextureTag)
{
	CGameInstance* instance = GET_INSTANCE(CGameInstance);

	if (FAILED(__super::Add_Component(0, TEXT("TextureTag"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
}


