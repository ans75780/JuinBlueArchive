#include "..\Public\UIBase.h"



CUIBase::CUIBase(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	:CGameObject(pDevice, pContext)
{
}

CUIBase::CUIBase(const CUIBase & Prototype)
	: CGameObject(Prototype)
{

}

CUIBase::~CUIBase()
{
}

HRESULT CUIBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIBase::Initialize(void * pArg)
{


	return S_OK;
}

void CUIBase::Tick(_float fTimeDelta)
{
}

void CUIBase::LateTick(_float fTimeDelta)
{
}

HRESULT CUIBase::Render()
{

	return S_OK;
}

void CUIBase::Set_Pos_LeftTop_Standard(_float3 _fPosition)
{
	m_fPosition = _fPosition;

	m_pTransformCom->Set_State(
	CTransform::STATE_TRANSLATION, 
	XMVectorSet(m_fPosition.x, m_fPosition.y, 0.f, 1.f));

}

void CUIBase::OnLButtonDown(void * arg)
{
}

void CUIBase::OnLButtonUp(void * arg)
{
}

void CUIBase::OnLButtonClicked(void * arg)
{
}
