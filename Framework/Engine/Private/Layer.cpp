#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);	
}

HRESULT CLayer::Initialize()
{

	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
		{
			if (pGameObject->Get_Delete())
			{
				pGameObject->Free();
				m_GameObjects.remove(pGameObject);
				break;
			}
			pGameObject->Tick(fTimeDelta);
		}
	}
}

void CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->LateTick(fTimeDelta);
	}
}

HRESULT CLayer::Start_Level()
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Start_Level()))
				return E_FAIL;
		}
	}
	return S_OK;
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}

